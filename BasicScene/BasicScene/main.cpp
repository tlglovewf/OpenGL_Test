//
//  main.cpp
//  BasicScene
//
//  Created by TuLigen on 2018/5/14.
//  Copyright © 2018年 TuLigen. All rights reserved.
//

#include <iostream>
#include <OpenGL/gl.h>
#include <GLUT/glut.h>
#include "assert.h"
const int   windowsize = 600;

void keyboard(unsigned char ch,int x,int y)
{
    switch(ch)
    {
        case 'a':
            
            break;
        case 'd':
            
            break;
        case 'w':
            
            break;
        case 's':
            
            break;
        default:
            break;
    }
}

void mouse(int x1,int y1, int x2, int y2)
{
    
}


GLuint createProgram( const char *vp, const char *fp)
{
    static GLuint programid = 0;
    if( programid != 0)
    {
        return programid;
    }
    assert(vp);
    assert(fp);
    
    GLuint  vprogram = glCreateShader(GL_VERTEX_SHADER);
    
    assert(vprogram);
    
    glShaderSource(vprogram, 1, &vp, NULL);
    
    glCompileShader(GL_VERTEX_SHADER);
    
    GLint status;
    glGetShaderiv(vprogram, GL_COMPILE_STATUS, &status);
    
    if(!status)
    {
        int maxLength = 0;
        GLsizei length;
        glGetShaderiv(vprogram, GL_INFO_LOG_LENGTH, &maxLength);
        char *pInfoLog = new char[maxLength];
        glGetShaderInfoLog(vprogram, maxLength, &length, pInfoLog);
        printf("%s shader compile error is %s\n","vertex", pInfoLog);
        delete [] pInfoLog;
        glDeleteShader (vprogram);
        return 0;
    }
    
    GLuint  fprogram = glCreateShader(GL_FRAGMENT_SHADER);
    
    assert(fprogram);
    
    glShaderSource(fprogram, 1, &fp, NULL);
    
    glCompileShader(GL_FRAGMENT_SHADER);
    
    glGetShaderiv(vprogram, GL_COMPILE_STATUS, &status);
    
    if(!status)
    {
        int maxLength = 0;
        GLsizei length;
        glGetShaderiv(vprogram, GL_INFO_LOG_LENGTH, &maxLength);
        char *pInfoLog = new char[maxLength];
        glGetShaderInfoLog(vprogram, maxLength, &length, pInfoLog);
        printf("%s shader compile error is %s\n","fragment", pInfoLog);
        delete [] pInfoLog;
        glDeleteShader (vprogram);
        return 0;
    }

    programid = glCreateProgram();
    
    glAttachShader(programid, vprogram);
    
    glAttachShader(programid, fprogram);
    
    glLinkProgram(programid);
    
    
    glGetProgramiv(programid, GL_LINK_STATUS, &status);
    if ( !status )
    {
        printf( "GLSL shader program failed to link" );
        // check error message and log it
        int maxLength = 0;
        GLsizei length;
        
        glGetProgramiv(programid, GL_INFO_LOG_LENGTH, &maxLength);
        
        char *pInfoLog = new char[maxLength];
        glGetProgramInfoLog(programid, maxLength, &length, pInfoLog);
        printf("%s\n",pInfoLog);
        delete [] pInfoLog;
        
        glDeleteProgram (programid);
        return 0;
    }
    
    
    return programid;
}

const char *simplevp = "\
#version 110\
attribute vec2 Position;\
void main() {\
gl_Positioin = Position;\
}";

const char *simplefp = "\
varying vec2 vTexCoord;\
void main()\
{\
\
}";

void render()
{
    GLuint programid = createProgram(simplevp, simplefp);
    
    glUseProgram(programid);
}

void  display()
{
    GLint lightnum ;
    glGetIntegerv(GL_MAX_LIGHTS,&lightnum);
    
    printf("light number is %d\n",lightnum);
    
    glClearColor(1, 1, 1, 1);
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    render();
    
    glutSwapBuffers();
}

void idle()
{
    const char* versionString = (const char*) glGetString( GL_VERSION );
    
//    printf("%s\n",versionString);
    
    display();
}

int main(int argc, char * argv[]) {
    // insert code here...
    
    glutInit(&argc, argv);
    
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_STENCIL);
    
    glutInitWindowPosition(400, 100);
    
    glutInitWindowSize(windowsize, windowsize);
    
    glutCreateWindow("Basic Scene");
    
    glutDisplayFunc(display);
    
    
    
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);

    glutIdleFunc(idle);
    glutMainLoop();
    
    return 0;
}
