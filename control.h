/*
 * =====================================================================================
 *
 *       Filename:  control.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  11/20/2011 10:26:19 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *        Company:  
 *
 * =====================================================================================
 */

 #ifndef _CONTROL_H
 #define _CONTROL_H

struct struct_key {
    int x;
    int z;
};

extern struct struct_key key;

void key_press (int code, int state);

#endif
