/*
 * =====================================================================================
 *
 *       Filename:  main.c
 *
 *    Description:  Elesdee main loop
 *
 *        Version:  0.06
 *        Created:  07/19/2011 01:09:10 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Spyros Pettas, 
 *        Company:  Gray Alien Resistance
 *
 * =====================================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "structures.h"
#include "gl3.h"
#include "obj_loader.h"
#include "matrix.h"
#include "procdl.h"
#include "road.h"
#include "background.h"
#include "control.h"
#include "fbo.h"

#define pi 3.14159
#define radians(deg)  (deg * (pi/180))
glarray NOarray = {NULL, 0};

void create_window (int width, int height)
{
    if (!glfwInit()) {
	printf ("Cannot Init Glfw\n");
    }

    glfwOpenWindowHint (GLFW_OPENGL_VERSION_MAJOR,3);
    glfwOpenWindowHint (GLFW_OPENGL_VERSION_MINOR,2);
    glfwOpenWindowHint (GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
    glfwOpenWindowHint (GLFW_OPENGL_FORWARD_COMPAT, GL_FALSE);
    glfwOpenWindowHint (GLFW_WINDOW_NO_RESIZE, GL_TRUE);
    if (!glfwOpenWindow( width, height, 0, 0, 0, 0, 0, 0, GLFW_WINDOW)) {
	printf ("Cannot open window\n");
    }
    glfwSetWindowTitle ("elesdee");

}

int main()
{   
    
    GLfloat identity[16] = { 1.0f, 0.0f, 0.0f, 0.0f,
			     0.0f, 1.0f, 0.0f, 0.0f,
			     0.0f, 0.0f, 1.0f, 0.0f,
			     0.0f, 0.0f, 0.0f, 1.0f, };

    create_window (800, 600);

    GLfloat* projection = mk_prjmatrix (60, (GLfloat) 800 / 600 , 1.0f, 100.0f);
    mesh monkey;
    mesh* obj = malloc (sizeof(mesh));
    obj_loader (obj,"monkey.obj");

    int i;
    GLfloat* model_matrix = mk_mdlmatrix (0, 0, 0, mk_quatrn (1,0,0,0));
    GLfloat* check_model_matrix = mk_mdlmatrix (-1, 0.1, 0, mk_quatrn (1, 0, 0, 0));

    GLint running = GL_TRUE;

    GLfloat z=0;
    GLfloat y=0;

    quatrn quarterion;
    quatrn *qrot = mk_quatrn (1, 0, 0, 0);

    GLfloat* camera = mk_camera (0,0,0, qrot);

    quatrn quarterion2;
    quatrn *qrot2 = mk_quatrn (1, 0, 0, 0);
   
    glres* monkey_res = mk_glres_from_obj (obj, "shaders/monkey.vert", "shaders/monkey.frag", "in_Position", NULL, NOarray, check_model_matrix, camera, projection, GL_TRIANGLES);
    /*mk_glres_from_ply (obj, "shaders/monkey.vert", "shaders/monkey.frag", "in_Position",
					   "in_Color", check_model_matrix, camera, projection, GL_TRIANGLES);*/
    



    GLfloat hcp[24] = {  0.0f,  -2.0f, -0.0f, 
			 0.0f,  -2.0f, -20.0f, 
			 10.0f, -6.0f, -40.0f, 
			-5.0f,  -6.0f, -60.0f, 
			 5.0f,  -2.0f, -80.0f, 
			 0.0f,  -2.0f, -100.0f,
			 1.0f,  -6.0f,  -150.0f,
			 5.0f,  -2.0f, -180.0f};
	
    glres** level = mk_level (8, 1000, 70, hcp, model_matrix, camera, projection);
    glres* back = background_gradient ();

    GLfloat s[3], n[3],t,tmp;
    int ai = 0;
    glfwSetKeyCallback (&key_press);

    render_list rlist;
    rlist.count = 6;
    rlist.resource = malloc (rlist.count*sizeof(glres));
    rlist.resource[0] = level[0];
    rlist.resource[1] = level[1];
    rlist.resource[2] = level[2];
    rlist.resource[3] = level[3];
    rlist.resource[4] = level[4];
    rlist.resource[5] = monkey_res;

    fbo* fbuffer = mk_fbo (GL_FRAMEBUFFER);
       glres* fbo_res = mk_fbo_res ();
    
    

    while (running) {

    	/*if ( glfwGetKey (GLFW_KEY_UP) ){
	
		cubic_hermite_spline( hcp, ai,8, t, s);
		cubic_hermite_spline_normal(hcp, ai,8, t, n);
		update_quatrn (qrot2, n[1], n[0], n[2], 0);
		cam_position (camera, s[0],s[1] + 1.2f, s[2], qrot2);
		t += 1e-2f;
		if (t >= 1.0f){ 
		    ai += 1;
		    t = 0;
		}
	}*/

	translate (monkey_res, key.x*0.1, 0, key.z*0.1);
	s[0] = monkey_res->model_matrix[12];
	s[1] = monkey_res->model_matrix[13];
	s[2] = monkey_res->model_matrix[14];
	cam_position(camera, s[0], s[1] + 0.1, s[2] + 3.1, qrot);

        /*if ( glfwGetKey (GLFW_KEY_DOWN) ){
	    rotate (monkey_res, qrot);
	    printf ("%f %f\n",tmp, qrot->angle);
	    tmp +=0.01;
	    update_quatrn (qrot, 1, 0, 0, tmp);
	}*/
	/*if ( glfwGetKey(GLFW_KEY_LEFT) )
	    cam_transform (camera, 0.001, 0, 0, qrot);
        if ( glfwGetKey(GLFW_KEY_RIGHT) )
	    translate (monkey_res, 0, 0, -0.1); 
	*/
        if ( glfwGetKey(GLFW_KEY_ESC) ) {
	    running = GL_FALSE;
	}
       	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	render_to_fbo (monkey_res, fbuffer);

	background_render (back);

	for (i=0; i<rlist.count; i++)
	    render (rlist.resource[i]);
	fbo_render (fbo_res, fbuffer);

	glfwSwapInterval(1);
	glfwSwapBuffers();
    }
    
 
    return 0;
}
