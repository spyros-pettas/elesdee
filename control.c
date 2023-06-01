/*
 * =====================================================================================
 *
 *       Filename:  control.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  11/20/2011 09:50:51 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *        Company:  
 *
 * =====================================================================================
 */

#include "GL/glfw.h"
#include "control.h"

struct struct_key key;

void key_press (int code, int state)
{
    if (state == GLFW_PRESS) {
	switch (code) {
	    case GLFW_KEY_UP:
		key.z = -1;
		break;
	    case GLFW_KEY_DOWN:
		key.z = 1;
		break;
	    case GLFW_KEY_LEFT:
		key.x = -1;
		break;
	    case GLFW_KEY_RIGHT:
		key.x = 1;
		break;
	}
    }
    else {
	switch (code) {
	    case GLFW_KEY_UP:
		key.z = 0;
		break;
	    case GLFW_KEY_DOWN:
		key.z = 0;
		break;
	    case GLFW_KEY_LEFT:
		key.x = 0;
		break;
	    case GLFW_KEY_RIGHT:
		key.x = 0;
		break;
	}
    }

}


