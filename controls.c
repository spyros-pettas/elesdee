/*
 * =====================================================================================
 *
 *       Filename:  controls.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  10/11/2011 07:13:53 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *        Company:  
 *
 * =====================================================================================
 */
#include "matrix.h"

void glres_move_forward (glres* res, step)
{
    translate (res, 0, 0, step);
    calculate_distance (
