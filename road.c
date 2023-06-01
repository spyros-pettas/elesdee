/*
 * =====================================================================================
 *
 *       Filename:  road2.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  10/15/2011 05:54:29 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *        Company:  
 *
 * =====================================================================================
 */
#include "structures.h"
#include "procdl.h"
#include <stdlib.h>
#include "shader.h"
#include "multifractal.h"
#include "gradient.h"
#include "matrix.h"
#include "gl3.h"
#include <math.h>
#include <string.h>


spline* mk_spline (int cp_count, int subdivision, GLfloat* cp)
{
    GLuint point_count  = cp_count*subdivision;
    spline spln;
    
    spln.v = malloc (point_count*sizeof(vertex));
    spln.n = malloc (point_count*sizeof(vertex));

    GLfloat n[3], t;
    GLfloat step = 1 / (GLfloat) subdivision;
    unsigned long add = 0;
    int i;
    
    GLfloat s[3] = {0.0f, 0.0f, 0.0f};
    GLfloat s2[3] = {1.0f, 1.0f, 1.0f};
    
    for (i=0; i<(cp_count -1); i++) {
	for (t=0.0f; t<1.0f; t+= step) {
	    cubic_hermite_spline (cp, i, cp_count,t, s);
	    if ((abs ((GLfloat) length (s,s2)) > 0.5f)) {
		s2[0] = s[0];
		s2[1] = s[1];
		s2[2] = s[2];

		cubic_hermite_spline_normal (cp, i,cp_count, t, n);

		spln.v[add].x = s[0];
		spln.v[add].y = s[1];
		spln.v[add].z = s[2];

		spln.n[add].x = n[0];
		spln.n[add].y = n[1];
		spln.n[add].z = n[2];

		add++;
	    }
	}
    }
    
    spline* result = malloc (sizeof(spline));
    size_t size	   = (size_t) add*sizeof(vertex);
    result->v	   = malloc (size);
    result->n	   = malloc (size);
    result->count  = add;
    
    memcpy (result->v, spln.v, size);
    memcpy (result->n, spln.n, size);

    free(spln.v);
    free(spln.n);

    return result;
}

    

mesh* mk_road_mesh (spline* spln, GLfloat width)
{
    mesh* road = malloc (sizeof (mesh));
    road->vrtx_size = (size_t) (2*spln->count*sizeof(vertex));
    road->vrtx	    = malloc(road->vrtx_size);
    
    int i,add = 0, j=0;
    for (i=0; i<spln->count; i++) {
	road->vrtx[2*i].x    = spln->v[i].x - spln->n[i].x*width;
	road->vrtx[2*i].y    = spln->v[i].y;
	road->vrtx[2*i].z    = spln->v[i].z - spln->n[i].z*width;

	road->vrtx[2*i +1].x = spln->v[i].x + spln->n[i].x*width;
	road->vrtx[2*i +1].y = spln->v[i].y;
	road->vrtx[2*i +1].z = spln->v[i].z + spln->n[i].z*width;
    }
    
    road->id_size    = (size_t) (2*add*sizeof(GLuint));
    road->id = malloc (road->id_size); 
    for (i=0; i<(2*add); i++)
	road->id[i] = i;

    road->clr_size = (size_t) (2*spln->count*sizeof(color));
    road->clr = malloc(road->clr_size);
    
    i=0;
    while (i<2*spln->count-2) {

	road->clr[i].r = 0.0f;
	road->clr[i].g = 0.0f;
	road->clr[i].b = 0.0f;
	road->clr[i].a = 1.0f;



	road->clr[i+1].r = 3.14f;
	road->clr[i+1].g = 3.14f;
	road->clr[i+1].b = 3.14f;
	road->clr[i+1].a = 1.0f;

	i = i + 2;
    }

    return road;
}

void mk_terrain_indices (GLuint tp, mesh* terrain)
{
    GLuint count = terrain->vrtx_size / sizeof(vertex);
    GLuint i=0, l= 0, k=0, j=0, coef=0, counter = 0;
    GLuint coef_count = (count / tp) -1;
    GLuint tmp[3*count];
    //GLuint* tmp= malloc(3*count*sizeof(GLuint));
   
    while (coef< coef_count){
	for (i=0; i<2*tp; i +=2){
	    tmp[j +i] = k + l;
	    tmp[j +i +1] = tp + k +l;
	    //printf ("id %i, %i %i\n", tp, tmp[j+i], tmp[j+i+1]);
	    l++;
	}
	tmp[j+2*tp] = tmp[j + 2*tp -1]; // degenarate triangle
	tmp[j+2*tp +1] = tmp[j + 1];  // degenerate triangle
	j = j + 2*tp +2;
	k = k + tp;
	l = 0;
	counter = counter + 2*tp +2;
	coef++;
    }
 
    terrain->id_size = counter*sizeof(GLuint);
    terrain->id	     = malloc (terrain->id_size);
  
    for(i=0;i<counter;i++)
	terrain->id[i] = tmp[i];
   
}


mesh* mk_terl_mesh (mesh* road, spline* spln, GLuint tp)
{
    mesh* tl = malloc(sizeof(mesh));
    
    tl->vrtx_size = (size_t) tp*spln->count*sizeof(vertex);
    tl->vrtx = malloc(tl->vrtx_size);

    int i,k;
    GLfloat h[3];
    GLuint vcount = spln->count;

    for (i=0; i<vcount; i++) {
	for (k =0; k<tp; k++) {
	    tl->vrtx[tp*i+k].x = road->vrtx[2*i].x - spln->n[i].x*k;
	    tl->vrtx[tp*i+k].y = road->vrtx[2*i].y;
	    tl->vrtx[tp*i+k].z = road->vrtx[2*i].z;
	    
	    h[0] = 0.125*tl->vrtx[tp*i+k].x;
	    h[1] = 0.125*tl->vrtx[tp*i+k].y;
	    h[2] = 0.125*tl->vrtx[tp*i+k].z;
	    
	    tl->vrtx[tp*i+k].y = tl->vrtx[tp*i+k].y + ((20*k)/(k+20))*HybridMultiFractal (h, 0.25f, 5, 6, 0.1f);

	}
    }
    
    mk_terrain_indices (tp, tl);

    return tl;
}

mesh* mk_terr_mesh (mesh* road, spline* spln, GLuint tp)
{
    mesh* tr = malloc(sizeof(mesh));
    
    tr->vrtx_size = (size_t) tp*spln->count*sizeof(vertex);
    tr->vrtx = malloc(tr->vrtx_size);

    int i,k;
    GLfloat h[3];
    GLuint vcount = spln->count;
    GLuint lol = tp -1;

    for (i=0; i<vcount; i++) {
	for (k =0; k<tp; k++) {
	    tr->vrtx[tp*i+k].x = road->vrtx[2*i+1].x + spln->n[i].x*(lol-k);
	    tr->vrtx[tp*i+k].y = road->vrtx[2*i+1].y;
	    tr->vrtx[tp*i+k].z = road->vrtx[2*i+1].z;
	    
	    h[0] = 0.055*tr->vrtx[tp*i+k].x;
	    h[1] = 0.055*tr->vrtx[tp*i+k].y;
	    h[2] = 0.055*tr->vrtx[tp*i+k].z;
	    
	    tr->vrtx[tp*i+k].y = tr->vrtx[tp*i+k].y + ((40*(lol-k))/((lol-k)+20))*HybridMultiFractal (h, 0.25f, 4, 8, 0.1f);

	}
    }
    
    mk_terrain_indices (tp, tr);

    return tr;
}

mesh* mk_left_pavement (mesh* road)
{
    mesh* pav = malloc (sizeof(mesh));
    GLuint count = road->vrtx_size/sizeof(vertex);
    pav->vrtx_size = 2*count*sizeof(vertex);
    pav->vrtx = malloc (pav->vrtx_size);
    GLuint i=0,k=0;

    for (i=0; i<(count); i+=2) {
	assign_vertices (&(pav->vrtx[4*k]), &(road->vrtx[i]));
	
	assign_vertices (&(pav->vrtx[4*k +1]), &(pav->vrtx[4*k]));
	pav->vrtx[4*k+1].y += 0.1f;

	assign_vertices (&(pav->vrtx[4*k + 2]), &(pav->vrtx[4*k +1]));
	pav->vrtx[4*k +2].x -= 0.2f;

	assign_vertices (&(pav->vrtx[4*k +3]), &(pav->vrtx[4*k +2]));
	pav->vrtx[4*k +3].y -=0.2f;
	k++;
    }

    pav->id_size = (2*2*count-8)*sizeof(GLuint);
    pav->id = malloc (pav->id_size);


    for (i=0; i<(2*count-4); i++) {
	pav->id[2*i]    = i;
	pav->id[2*i +1] = i+4;
    }

    return pav;
}

mesh* mk_right_pavement (mesh* road)
{
    mesh* pav = malloc (sizeof(mesh));
    GLuint count = road->vrtx_size/sizeof(vertex);
    pav->vrtx_size = 2*count*sizeof(vertex);
    pav->vrtx = malloc (pav->vrtx_size);
    GLuint i=0,k=0;

    for (i=0; i<(count); i=2*k+1) {
	assign_vertices (&(pav->vrtx[4*k]), &(road->vrtx[i]));
	
	assign_vertices (&(pav->vrtx[4*k +1]), &(pav->vrtx[4*k]));
	pav->vrtx[4*k+1].y += 0.1f;

	assign_vertices (&(pav->vrtx[4*k + 2]), &(pav->vrtx[4*k +1]));
	pav->vrtx[4*k +2].x += 0.2f;

	assign_vertices (&(pav->vrtx[4*k +3]), &(pav->vrtx[4*k +2]));
	pav->vrtx[4*k +3].y -=0.2f;
	k++;
    }

    pav->id_size = (2*2*count-8)*sizeof(GLuint);
    pav->id = malloc (pav->id_size);


    for (i=0; i<(2*count-4); i++) {
	pav->id[2*i]    = i;
	pav->id[2*i +1] = i+4;
    }

    return pav;
}



glres* mk_lpav_res (mesh* obj, GLfloat* model, GLfloat* view, GLfloat* projection)
{
    glarray NOarray = {NULL, 0};
    glres* pav = mk_glres_from_obj (obj, "shaders/pav.vert", "shaders/pav.frag", "in_Position", 
				    NULL, NOarray, model, view, projection, GL_TRIANGLE_STRIP);
    return pav;
}


void addsign_vectors (vertex* a, vertex b)
{
    a->x += b.x;
    a->y += b.y;
    a->z += b.z;
}
    

norm* mk_normals (mesh* obj)
{
    GLuint i =0;
    GLuint k =0, l=0;
    GLuint j =0, j0 =0, j1 =0, j2=0, j3=0;
    vertex a,b, result, result2;
    GLuint count = (GLuint) obj->id_size / (sizeof(GLuint));
    GLuint vrtx_count = (GLuint) obj->vrtx_size / (sizeof(vertex));
    norm* vertex_normal = malloc(sizeof(norm));
    vertex_normal->vrtx = malloc(vrtx_count*sizeof(vertex));

    GLfloat percent = 100.0f/vrtx_count;

     for (i=0; i<(vrtx_count); i++) {
	vertex_normal->vrtx[i].x =0;
	vertex_normal->vrtx[i].y =0;
	vertex_normal->vrtx[i].z =0;
    }


    i=0;   
    while (i<(count-3)) {
	j0 = obj->id[i];
	j1 = obj->id[i+1];
	j2 = obj->id[i+2];
	j3 = obj->id[i+3];
		
	a.x = obj->vrtx[j1].x - obj->vrtx[j0].x;
	a.y = obj->vrtx[j1].y - obj->vrtx[j0].y;
	a.z = obj->vrtx[j1].z - obj->vrtx[j0].z;

	b.x = obj->vrtx[j1].x - obj->vrtx[j2].x;
	b.y = obj->vrtx[j1].y - obj->vrtx[j2].y;
	b.z = obj->vrtx[j1].z - obj->vrtx[j2].z;

	result = cross_product (a,b);
	addsign_vectors ( &vertex_normal->vrtx[j0], result);
	addsign_vectors ( &vertex_normal->vrtx[j1], result);
	addsign_vectors ( &vertex_normal->vrtx[j2], result);
	
	a.x = obj->vrtx[j2].x - obj->vrtx[j1].x;
	a.y = obj->vrtx[j2].y - obj->vrtx[j1].y;
	a.z = obj->vrtx[j2].z - obj->vrtx[j1].z;

	b.x = obj->vrtx[j2].x - obj->vrtx[j3].x;
	b.y = obj->vrtx[j2].y - obj->vrtx[j3].y;
	b.z = obj->vrtx[j2].z - obj->vrtx[j3].z;

 	result2 = cross_product (b,a);
	addsign_vectors ( &vertex_normal->vrtx[j1], result2);
	addsign_vectors ( &vertex_normal->vrtx[j2], result2);
	addsign_vectors ( &vertex_normal->vrtx[j3], result2);

	i+=2;
	//printf("%i\n",i);

    }

    i=0;
    for (i=0; i<(vrtx_count); i++)
	normalize_vector(&vertex_normal->vrtx[i]);

   
    vertex_normal->vrtx_size = obj->vrtx_size;
    return vertex_normal;
}

glres* mk_terrain_res (mesh* terrain_mesh, gradient_cp* gcp,
		       GLfloat* model, GLfloat* view,
		       GLfloat* projection)
{
    glres* terrain = malloc (sizeof(glres));
 
    norm* normals = mk_normals (terrain_mesh);

    terrain->vbo       = malloc (2*(sizeof(vertex_buffer)));
    terrain->vbo_count = 2;

    terrain->vbo[0].attrb = "Position";
    terrain->vbo[1].attrb = "Normal";

    terrain->vbo[0].components = 3;
    terrain->vbo[1].components = 3;

    GLuint vshad = sh_loader ("shaders/mountain.vert", GL_VERTEX_SHADER);
    GLuint fshad = sh_loader ("shaders/mountain.frag", GL_FRAGMENT_SHADER);

    GLuint prg   = link_prg (vshad,fshad);

    glGenBuffers (1, &(terrain->vbo[0].id));
    glBindBuffer (GL_ARRAY_BUFFER, terrain->vbo[0].id);
    glBufferData (GL_ARRAY_BUFFER, terrain_mesh->vrtx_size, terrain_mesh->vrtx, GL_STATIC_DRAW);

    glGenBuffers (1, &(terrain->vbo[1].id));
    glBindBuffer (GL_ARRAY_BUFFER, terrain->vbo[1].id);
    glBufferData (GL_ARRAY_BUFFER, normals->vrtx_size, normals->vrtx, GL_STATIC_DRAW);

    glGenBuffers (1, &(terrain->ebo.id));
    glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, terrain->ebo.id);
    glBufferData (GL_ELEMENT_ARRAY_BUFFER, terrain_mesh->id_size, terrain_mesh->id, GL_STATIC_DRAW);

    terrain->ebo.count    = (GLuint) terrain_mesh->id_size / sizeof (GLuint);
    terrain->vbo[0].count = (GLuint) terrain_mesh->vrtx_size / sizeof(vertex);
    terrain->vbo[1].count = (GLuint) normals->vrtx_size / sizeof(vertex);

    terrain->vertex_shader   = vshad;
    terrain->fragment_shader = fshad;
    terrain->program	     = prg;

    terrain->vbo[0].attrb_loc = glGetAttribLocation (prg, terrain->vbo[0].attrb);
    terrain->vbo[1].attrb_loc = glGetAttribLocation (prg, terrain->vbo[1].attrb);

    terrain->uniform.name  = "gradient";
    terrain->uniform.array = mk_gradient (gcp,1000,3);
    terrain->uniform.loc   = glGetUniformLocation (terrain->program, terrain->uniform.name);

    terrain->model_matrix  = model;
    terrain->view_matrix   = view;
    terrain->prj_matrix	   = projection;

    terrain->mode = GL_TRIANGLE_STRIP;

    return terrain;
}

glres* mk_road_res (mesh* obj, GLfloat* model, GLfloat* view, GLfloat* projection)
{
    glarray NOarray = {NULL, 0};
    glres* road = malloc (sizeof(glres));
    road->vbo = malloc (2*sizeof(vertex_buffer));
    road->vbo[0].attrb = "in_Position";
    road->vbo[1].attrb = "in_Color";

    road->vbo[0].components = 3;
    road->vbo[1].components = 4;

    
    glGenBuffers (1, &road->vbo[0].id);
    glBindBuffer (GL_ARRAY_BUFFER, road->vbo[0].id);
    glBufferData (GL_ARRAY_BUFFER, obj->vrtx_size, obj->vrtx, GL_STATIC_DRAW);
    
    glGenBuffers (1, &road->vbo[1].id);
    glBindBuffer (GL_ARRAY_BUFFER, road->vbo[1].id);
    glBufferData (GL_ARRAY_BUFFER, obj->clr_size, obj->clr, GL_STATIC_DRAW);
    
    glGenBuffers (1, &road->ebo.id);
    glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, road->ebo.id);
    glBufferData (GL_ELEMENT_ARRAY_BUFFER, obj->id_size, obj->id);
    
    road->ebo.count    = (GLuint) obj->id_size/ sizeof (GLuint);
    road->vbo[0].count = obj->vrtx_size / sizeof (vertex);
    road->vbo[1].count = obj->clr_size / sizeof (color);
    
    road->vertex_shader = sh_loader ("shaders/road.vert", GL_VERTEX_SHADER);
    road->fragment_shader = sh_loader ("shaders/road.frag", GL_FRAGMENT_SHADER);
    road->program = link_prg (road->vertex_shader, road->fragment_shader);
    
    road->vbo[0].attrb_loc = glGetAttribLocation (road->program, road->vbo[0].attrb);
    road->vbo[1].attrb_loc = glGetAttribLocation (road->program, road->vbo[1].attrb);
    
    road->vbo_count = 2;
    
    road->model_matrix = model;
    road->view_matrix = view;
    road->prj_matrix = projection;

    road->mode = GL_TRIANGLE_STRIP;
    
    return road;
}

gradient_cp* level_gradient()
{
    gradient_cp* cp =malloc(sizeof(gradient_cp));
    cp->clr = malloc(3*sizeof(color));
    cp->pos = malloc(3*sizeof(GLfloat));

    cp->clr[0].r = 1.0f;
    cp->clr[0].g = 0.5f;
    cp->clr[0].b = 0.0f;
    cp->clr[0].a = 0.0f;
    cp->pos[0]   = 0.0f;

    cp->clr[1].r = 1.0f;
    cp->clr[1].g = 0.0f;
    cp->clr[1].b = 0.2f;
    cp->clr[1].a = 1.0f;
    cp->pos[1]   = 0.5f;

    cp->clr[2].r = 0.1f;
    cp->clr[2].g = 0.0f;
    cp->clr[2].b = 0.2f;
    cp->clr[2].a = 1.0f;
    cp->pos[2]   = 1.0f;

    return cp;
}

glres** mk_level (int cp_count, int subdivision, 
		 int terrain_points, GLfloat *cp,
		 GLfloat* model, GLfloat* view, 
		 GLfloat* projection)
{
    gradient_cp* gcp = level_gradient ();
    
    spline* spln = mk_spline (cp_count, subdivision, cp);

    mesh* road     = mk_road_mesh (spln, 2.5f);
    glres* road_res = mk_road_res (road, model, view, projection);

    mesh* tol	  = mk_terl_mesh (road, spln, terrain_points);
    glres* tl_res = mk_terrain_res  (tol, gcp, model, view, projection);
    
    mesh* tr      = mk_terr_mesh (road, spln, terrain_points);
    glres* tr_res = mk_terrain_res  (tr, gcp, model, view, projection);

    mesh* pavl	   = mk_left_pavement(road);
    glres* pavl_res = mk_lpav_res(pavl, model, view, projection); 
    
    mesh* pavr	  = mk_right_pavement(road);
    glres* pavr_res = mk_lpav_res (pavr, model, view, projection);
    glres** resource = malloc (5*sizeof(glres));

    resource[0] = road_res;
    resource[1] = tl_res;
    resource[2] = tr_res;
    resource[3] = pavl_res;
    resource[4] = pavr_res;

    return resource;
}

