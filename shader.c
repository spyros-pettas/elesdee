/*
 * =====================================================================================
 *
 *       Filename:  shader.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  07/05/2011 11:36:43 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *        Company:  
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include <stdio.h>
#include <GL/glfw.h>

char* shader2string (const char* filename)
{
    FILE* fd = fopen(filename, "r");
    long len, r;
    char* str;
   

    if (!fd){
	printf("Cannot open file '%s' for reading\n", filename);
    }
    fseek( fd, 0, SEEK_END);
    len = ftell(fd);

     printf("\033[0;40;33mFile '%s' is %ld long\n", filename, len);

    fseek(fd, 0, SEEK_SET);

    if( !(str=malloc(len * sizeof(char))) )
    {
	printf("Can't malloc space for '%s'\n", filename);
	return NULL;
    }

    r=fread( str, sizeof(char),len,fd );

    str[r-1]= '\0';
    printf("\033[0;40;33m=====================\n%s\n=====================\n",str);
    fclose(fd);
    
    return str;
}

GLuint sh_loader(const char *shader_source, GLenum type)
{
    char *Source = shader2string(shader_source);
    GLuint sh;
    GLint compiled_flag;
    GLsizei length;
    GLint maxLength = 0;
    GLchar* infoLog;

    sh = glCreateShader(type);
    glShaderSource(sh, 1, &Source, NULL);
    glCompileShader(sh);
    glGetShaderiv(sh, GL_COMPILE_STATUS, &compiled_flag);
    if(compiled_flag == GL_FALSE)
    {
	glGetShaderiv(sh,GL_INFO_LOG_LENGTH, &maxLength);
	printf("%i\n",maxLength);

	infoLog = (GLchar*) malloc(maxLength);

	glGetShaderInfoLog(sh, maxLength, &length , infoLog);
	printf("\033[0;40;31merror: %s\n",infoLog);
	free (infoLog);
    }
    return sh;
}

GLuint link_prg(GLuint vsh, GLuint fsh)
{
    GLint prg_flag;
    
    GLuint program = glCreateProgram();

    glAttachShader (program, vsh);
    glAttachShader (program, fsh);
    glLinkProgram  (program);

    glGetProgramiv(program, GL_LINK_STATUS, &prg_flag);
    if (prg_flag = GL_FALSE) 
    {
	printf("\033[0;40;31mFailed to link shader program:\n");
	GLint maxlength;
	glGetProgramiv(program,GL_INFO_LOG_LENGTH, &maxlength);
	char infolog[maxlength];
	GLsizei length;
	glGetProgramInfoLog(program, maxlength, &length, infolog);
	printf("\033[0;40;31m %s\n",infolog);
	glDeleteProgram(program);
	return 0;
    }
    return program;
}

