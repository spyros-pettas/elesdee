/*
 * =====================================================================================
 * TODO:
 *    
 *
 * =====================================================================================
 */


#include <stdlib.h>
#include <math.h>
#include "structures.h"
#define pi 3.14159
#define radians(deg)  (deg * (pi/180))

GLfloat mdlmatrix[16] = { 1.0f, 0.0f, 0.0f, 0.0f,  
			  0.0f, 1.0f, 0.0f, 0.0f, 
			  0.0f, 0.0f, 1.0f, 0.0f, 
			  0.0f, 0.0f, 0.0f, 1.0f };

GLfloat* mk_prjmatrix (GLfloat fovy, GLfloat aspect, GLfloat znear, GLfloat zfar)
{
    GLfloat* matrix = malloc (16*sizeof(GLfloat));

    int i;
    for (i=0;i<16;i++){
	matrix[i] = 0.0f;
    }

    GLfloat range  = tan (radians(fovy/2))*znear;
    GLfloat left   = -range*aspect;
    GLfloat right  = range*aspect;
    GLfloat bottom = -range;
    GLfloat top    = range;

    matrix[0]  =       (2*znear) / (right - left);
    matrix[5]  =       (2*znear) / (top - bottom);
    matrix[10] =  -(zfar + znear) / (zfar -znear);
    matrix[14] =			       -1;
    matrix[11] = -(2*zfar*znear) / (zfar - znear);

    return matrix;
}

/* QUATERNIONS
   ----------- */

void normalize_quatrn (quatrn* quaternion)
{
    GLfloat norm = 1/sqrt( quaternion->angle*quaternion->angle
			 + quaternion->img.x*quaternion->img.x
			 + quaternion->img.y*quaternion->img.y
			 + quaternion->img.z*quaternion->img.z);

    quaternion->img.x     *= norm;
    quaternion->img.y     *= norm;
    quaternion->img.z     *= norm;
    quaternion->angle     *= norm;
}


quatrn* mk_quatrn (GLfloat x, GLfloat y, GLfloat z, GLfloat w)
{
    
    quatrn* quaternion = malloc( sizeof(quatrn));
    
    GLfloat sin_a = sin (w / 2);
    GLfloat cos_a = cos (w / 2);

    quaternion->img.x = x*sin_a;
    quaternion->img.y = y*sin_a;
    quaternion->img.z = z*sin_a;
    quaternion->angle = cos_a;

    normalize_quatrn(quaternion);

    return quaternion;
}

void update_quatrn (quatrn* quaternion, GLfloat x, GLfloat y, 
					GLfloat z, GLfloat w)
{   
    GLfloat sin_a = sin (w / 2);
    GLfloat cos_a = cos (w / 2);
    
    quaternion->img.x = x*sin_a;
    quaternion->img.y = y*sin_a;
    quaternion->img.z = z*sin_a;
    quaternion->angle = cos_a;

    normalize_quatrn(quaternion);
}

quatrn* multiply_quatrn (quatrn* q1, quatrn* q2)
{
    quatrn* result = malloc(sizeof(quatrn));
    GLfloat x1 = q1->img.x;
    GLfloat y1 = q1->img.y;
    GLfloat z1 = q1->img.z;
    GLfloat w1 = q1->angle;

    GLfloat x2 = q2->img.x;
    GLfloat y2 = q2->img.y;
    GLfloat z2 = q2->img.z;
    GLfloat w2 = q2->angle;

    result->img.x = w1*x2 + x1*w2 + y1*z2 - z1*y2;
    result->img.y = w1*y2 + x1*z2 + y1*w2 + z1*x2;
    result->img.z = w1*z2 + x1*y2 + y1*x2 + z1*w2;
    result->angle = w1*w2 + x1*x2 + y1*y2 - z1*y2;

    return result;
}


void localize_quatrn(quatrn* quaternion)
{
    GLfloat sin_a = sin (quaternion->angle / 2);
    GLfloat cos_a = cos (quaternion->angle / 2);

    quaternion->img.x = quaternion->img.x * sin_a;
    quaternion->img.y = quaternion->img.y * sin_a;
    quaternion->img.z = quaternion->img.z * sin_a;
    quaternion->angle = cos_a;

    //quatrn_normalize(quaternion);
}

/*------------------------------------------------------------*/
    

GLfloat* mk_mdlmatrix (GLfloat xi, GLfloat yi, GLfloat zi,
				       quatrn* quaternion)
{
    GLfloat* matrix = malloc(16*sizeof(GLfloat));

    GLfloat x = quaternion->img.x;
    GLfloat y = quaternion->img.y;
    GLfloat z = quaternion->img.z;
    GLfloat w = quaternion->angle;

    matrix[0]  = 1 - 2*y*y - 2*z*z;
    matrix[1]  =     2*x*y - 2*w*z;
    matrix[2]  =     2*x*z + 2*w*y;
    matrix[3]  =		 0;
    
    matrix[4]  =     2*x*y + 2*w*z;
    matrix[5]  = 1 - 2*x*x - 2*z*z;
    matrix[6]  =     2*y*z - 2*w*x;
    matrix[7]  =		 0;

    matrix[8]  =     2*x*z - 2*w*y;
    matrix[9]  =     2*y*z + 2*w*x;
    matrix[10] = 1 - 2*x*x - 2*y*y;
    matrix[11] =		 0;

    matrix[12] =		xi;
    matrix[13] =		yi;
    matrix[14] =		zi;
    matrix[15] =	      1.0f;

    return matrix;

}

void translate (glres* resource, 
		GLfloat dx, GLfloat dy, GLfloat dz)
{
    resource->model_matrix[12]  += dx;
    resource->model_matrix[13]  += dy;
    resource->model_matrix[14]  += dz;
}

void rotate (glres* resource, quatrn* quaternion)
	     
{

    GLfloat x = quaternion->img.x;
    GLfloat y = quaternion->img.y;
    GLfloat z = quaternion->img.z;
    GLfloat w = quaternion->angle;

    resource->model_matrix[0]  = 1 - 2*y*y - 2*z*z;
    resource->model_matrix[1]  =     2*x*y - 2*w*z;
    resource->model_matrix[2]  =     2*x*z + 2*w*y;
    
    resource->model_matrix[4]  =     2*x*y + 2*w*z;
    resource->model_matrix[5]  = 1 - 2*x*x - 2*z*z;
    resource->model_matrix[6]  =     2*y*z - 2*w*x;

    resource->model_matrix[8]  =     2*x*z - 2*w*y;
    resource->model_matrix[9]  =     2*y*z + 2*w*x;
    resource->model_matrix[10] = 1 - 2*x*x - 2*y*y;
}

void transform (glres* resource,
		GLfloat dx, GLfloat dy, GLfloat dz,
		quatrn* quaternion)
{

    GLfloat x = quaternion->img.x;
    GLfloat y = quaternion->img.y;
    GLfloat z = quaternion->img.z;
    GLfloat w = quaternion->angle;

    resource->model_matrix[0]  = 1 - 2*y*y - 2*z*z;
    resource->model_matrix[1]  =     2*x*y - 2*w*z;
    resource->model_matrix[2]  =     2*x*z + 2*w*y;
    
    resource->model_matrix[4]  =     2*x*y + 2*w*z;
    resource->model_matrix[5]  = 1 - 2*x*x - 2*z*z;
    resource->model_matrix[6]  =     2*y*z - 2*w*x;

    resource->model_matrix[8]  =     2*x*z - 2*w*y;
    resource->model_matrix[9]  =     2*y*z + 2*w*x;
    resource->model_matrix[10] = 1 - 2*x*x - 2*y*y;

    resource->model_matrix[12]  += dx;
    resource->model_matrix[13]  += dy;
    resource->model_matrix[14]  += dz;
}

GLfloat* mk_inverse_matrix ( GLfloat* mat)
{
    GLfloat* ret = malloc (16*sizeof(GLfloat));

    ret[0] = mat[0]; ret[1] = mat[4]; ret[2]  = mat[8];  ret[3]  = 0.0f;
    ret[4] = mat[1]; ret[5] = mat[5]; ret[6]  = mat[9];  ret[7]  = 0.0f;
    ret[8] = mat[2]; ret[9] = mat[6]; ret[10] = mat[10]; ret[11] = 0.0f;
    
    ret[12] = ret[0]*mat[12] + ret[4]*mat[13] + ret[8]*mat[14];
    ret[13] = ret[1]*mat[12] + ret[5]*mat[13] + ret[9]*mat[14];
    ret[14] = ret[2]*mat[12] + ret[6]*mat[13] + ret[10]*mat[14];

    ret[15] = 1.0f;

    return ret;
}
    
GLfloat* mk_camera (GLfloat xi, GLfloat yi, GLfloat zi,
				    quatrn* quaternion)
{
    quaternion->img.x = -(quaternion->img.x);
    quaternion->img.y = -(quaternion->img.y);
    quaternion->img.z = -(quaternion->img.z);

    GLfloat* view = mk_mdlmatrix (-xi, -yi, -zi, quaternion);

    return view;
}

void cam_transform (GLfloat* camera, 
		    GLfloat xi, GLfloat yi, GLfloat zi, 
		    quatrn* quaternion)
{
    GLfloat x = -quaternion->img.x;
    GLfloat y = -quaternion->img.y;
    GLfloat z = -quaternion->img.z;
    GLfloat w =  quaternion->angle;
    
    camera[0]  = 1 - 2*y*y - 2*z*z;
    camera[1]  =     2*x*y - 2*w*z;
    camera[2]  =     2*x*z + 2*w*y;
    camera[3]  =	      0.0f;
    
    camera[4]  =     2*x*y + 2*w*z;
    camera[5]  = 1 - 2*x*x - 2*z*z;
    camera[6]  =     2*y*z - 2*w*x;
    camera[7]  =	      0.0f;

    camera[8]  =     2*x*z - 2*w*y;
    camera[9]  =     2*y*z + 2*w*x;
    camera[10] = 1 - 2*x*x - 2*y*y;
    camera[11] =	      0.0f;

    camera[12] =   camera[12] - xi;
    camera[13] =   camera[13] - yi;
    camera[14] =   camera[14] - zi;
    camera[15] =	      1.0f;
}

void cam_position (GLfloat* camera, 
		   GLfloat xi, GLfloat yi, GLfloat zi, 
		   quatrn* quaternion)

{
    GLfloat x = -quaternion->img.x;
    GLfloat y = -quaternion->img.y;
    GLfloat z = -quaternion->img.z;
    GLfloat w =  quaternion->angle;
    
    camera[0]  = 1 - 2*y*y - 2*z*z;
    camera[1]  =     2*x*y - 2*w*z;
    camera[2]  =     2*x*z + 2*w*y;
    camera[3]  =	      0.0f;
    
    camera[4]  =     2*x*y + 2*w*z;
    camera[5]  = 1 - 2*x*x - 2*z*z;
    camera[6]  =     2*y*z - 2*w*x;
    camera[7]  =	      0.0f;

    camera[8]  =     2*x*z - 2*w*y;
    camera[9]  =     2*y*z + 2*w*x;
    camera[10] = 1 - 2*x*x - 2*y*y;
    camera[11] =	      0.0f;

    camera[12] =	       -xi;
    camera[13] =	       -yi;
    camera[14] =	       -zi;
    camera[15] =	      1.0f;
}

vertex cross_product (vertex b, vertex c)
{
    vertex a;

    a.x = b.y*c.z - b.z*c.y;
    a.y = b.z*c.x - b.x*c.z;
    a.z = b.x*c.y - b.y*c.x;

    return a;
}

void normalize_vector (vertex* a)
{
    GLfloat magnitude;
    magnitude = sqrt (a->x*a->x + a->y*a->y + a->z*a->z);

    a->x = a->x/magnitude;
    a->y = a->y/magnitude;
    a->z = a->z/magnitude;
}

void assign_vertices (vertex* a, vertex* b)
{
    a->x = b->x;
    a->y = b->y;
    a->z = b->z;
}

vertex subtract_vertices (vertex* a, vertex* b)
{
    vertex result;
    result.x = a->x - b->x;
    result.y = a->y - b->y;
    result.z = a->z - b->z;

    return result;
}

GLfloat vector_length (vertex* a)
{
    return sqrt(a->x*a->x + a->y*a->y + a->z*a->z);
}

GLuint compare_vertices (vertex* a, vertex* b)
{
    vertex result;
    result = subtract_vertices (a, b);

    if ( result.x == 0 &&
	 result.y == 0 &&
	 result.z == 0   )
	return 1;
    else return 0;
}

