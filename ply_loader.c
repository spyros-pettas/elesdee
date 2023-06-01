#include <stdio.h>
#include <stdlib.h>
#include "rply.h"
#include "structures.h"
#include "GL/glfw.h"

static mesh* obj;

static int vertex_x_cb (p_ply_argument argument) {
    long index;
    ply_get_argument_element (argument, NULL, &index);
    ply_get_argument_user_data(argument, NULL, NULL);
    obj->vrtx[index].x = ply_get_argument_value (argument);
    return 1;
}

static int vertex_y_cb (p_ply_argument argument) {
    long index;
    ply_get_argument_element (argument, NULL, &index);
    ply_get_argument_user_data(argument, NULL, NULL);
    obj->vrtx[index].y = ply_get_argument_value (argument);
    return 1;
}

static int vertex_z_cb (p_ply_argument argument) {
    long index;
    ply_get_argument_element (argument, NULL, &index);
    ply_get_argument_user_data(argument, NULL, NULL);
    obj->vrtx[index].z = ply_get_argument_value (argument);
    return 1;
}

static int color_r_cb (p_ply_argument argument) {
    long index;
    ply_get_argument_element (argument, NULL, &index);
    ply_get_argument_user_data (argument, NULL, NULL);
    obj->clr[index].r = ((GLfloat) ply_get_argument_value (argument))/255;
    return 1;
}

static int color_g_cb (p_ply_argument argument) {
    long index;
    ply_get_argument_element (argument, NULL, &index);
    ply_get_argument_user_data (argument, NULL, NULL);
    obj->clr[index].g = ((GLfloat) ply_get_argument_value (argument))/255;
    return 1;
}

static int color_b_cb (p_ply_argument argument) {
    long index;
    ply_get_argument_element (argument, NULL, &index);
    ply_get_argument_user_data (argument, NULL, NULL);
    obj->clr[index].b = ((GLfloat) ply_get_argument_value (argument))/255;
    obj->clr[index].a = 1.0f;
    return 1;
}



static int face_cb(p_ply_argument argument) {
    long length, value_index, index;
    ply_get_argument_property(argument, NULL, &length, &value_index);
    ply_get_argument_element (argument, NULL, &index);
    if (value_index >=0)
	obj->id[3*index + value_index] = ply_get_argument_value (argument);
    return 1;
}

mesh* ply_loader(char* filename) {

    mesh* mobj = malloc (sizeof(mesh));

    long nvertices, ntriangles, ncolors;
    p_ply ply = ply_open(filename, NULL, 0, NULL);
    if (!ply) return NULL;
    if (!ply_read_header(ply)) return NULL;
    nvertices = ply_set_read_cb(ply, "vertex", "x", vertex_x_cb, NULL, 0);
    ply_set_read_cb(ply, "vertex", "y", vertex_y_cb, NULL, 0);
    ply_set_read_cb(ply, "vertex", "z", vertex_z_cb, NULL, 1);

    ncolors = ply_set_read_cb (ply, "vertex", "red", color_r_cb, NULL, 0);
    ply_set_read_cb (ply, "vertex", "green", color_g_cb, NULL, 0);
    ply_set_read_cb (ply, "vertex", "blue", color_b_cb, NULL, 0);

    ntriangles = ply_set_read_cb(ply, "face", "vertex_indices", face_cb, NULL, 0);
    printf("%ld\n%ld\n", nvertices, ntriangles);
    
    mobj->vrtx_size = (size_t) nvertices*sizeof(vertex);
    mobj->id_size   = (size_t) ntriangles*3*sizeof(GLuint);
    mobj->clr_size  = (size_t) ncolors*sizeof(color);

    mobj->vrtx = malloc (mobj->vrtx_size);
    mobj->id = malloc (mobj->id_size);
    mobj->clr = malloc (mobj->clr_size);
    
    obj = mobj;

    if (!ply_read(ply)) return NULL;
    ply_close(ply);
    return mobj;
}

glres* mk_glres_from_ply (mesh* iobj, char* vshader, char* fshader, 
			  char* vrtx_attrb, char* clr_attrb,
			  GLfloat* model, GLfloat* view, GLfloat* projection,
			  GLenum mode)
{
    GLuint vbo, ibo;
    int i;

    glres* resource = malloc(sizeof(glres));
    resource->vbo   = malloc(2*sizeof(vertex_buffer)); 
    
    resource->vbo[0].attrb      = vrtx_attrb;
    resource->vbo[0].type       = GL_ARRAY_BUFFER;
    resource->vbo[0].components = 3;
    resource->vbo[0].count	= (GLuint) iobj->vrtx_size/sizeof(vertex);

    glGenBuffers (1, &resource->vbo[0].id);
    glBindBuffer (resource->vbo[0].type, resource->vbo[0].id);
    glBufferData (resource->vbo[0].type, iobj->vrtx_size, iobj->vrtx, GL_STATIC_DRAW);

    resource->vbo[1].attrb      = clr_attrb;
    resource->vbo[1].type       = GL_ARRAY_BUFFER;
    resource->vbo[1].components = 4;
    resource->vbo[1].count	= (GLuint) iobj->clr_size/sizeof(color);

    glGenBuffers (1, &resource->vbo[1].id);
    glBindBuffer (resource->vbo[1].type, resource->vbo[1].id);
    glBufferData (resource->vbo[1].type, iobj->clr_size, iobj->clr, GL_STATIC_DRAW);


    
    if (iobj->id != NULL) {
	glGenBuffers (1, &resource->ebo.id);
	glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, resource->ebo.id);
	glBufferData (GL_ELEMENT_ARRAY_BUFFER, iobj->id_size, iobj->id, GL_STATIC_DRAW);
    }
    
    resource->ebo.count       = (GLuint) iobj->id_size/sizeof(GLuint);
    resource->vertex_shader   = sh_loader (vshader, GL_VERTEX_SHADER);
    resource->fragment_shader = sh_loader (fshader, GL_FRAGMENT_SHADER);
    resource->program         = link_prg (resource->vertex_shader, 
					  resource->fragment_shader);
    resource->uniform.name    = NULL;
    resource->uniform.loc     = -1;
    
    resource->vbo[0].attrb_loc = glGetAttribLocation (resource->program, 
						   resource->vbo[0].attrb);
    resource->vbo[1].attrb_loc = glGetAttribLocation (resource->program,
						   resource->vbo[1].attrb);
    if (resource->vbo[0].attrb_loc == -1)
	printf ("\n \033[0;40;31m %s Attribute not found in shader program\n", 
							 resource->vbo[0].attrb);
    if (resource->vbo[1].attrb_loc == -1)
	printf ("\n \033[0;40;31m %s Attribute not found in shader program\n", 
							 resource->vbo[1].attrb);

   
    resource->vbo_count	      = 2;

    resource->model_matrix    = model;
    resource->view_matrix     = view;
    resource->prj_matrix      = projection;

    resource->mode = mode;

    return resource;
}


