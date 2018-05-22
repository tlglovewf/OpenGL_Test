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
#include "math.h"
const int   windowsize = 600;

float _a = 0.0;
void keyboard(unsigned char ch,int x,int y)
{
    switch(ch)
    {
        case 'a':
            _a += 1/4000.0;
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

//
//GLuint createProgram( const char *vp, const char *fp)
//{
//    static GLuint programid = 0;
//    if( programid != 0)
//    {
//        return programid;
//    }
//    assert(vp);
//    assert(fp);
//
//    GLuint  vprogram = glCreateShader(GL_VERTEX_SHADER);
//
//    assert(vprogram);
//
//    glShaderSource(vprogram, 1, &vp, NULL);
//
//    glCompileShader(GL_VERTEX_SHADER);
//
//    GLint status;
//    glGetShaderiv(vprogram, GL_COMPILE_STATUS, &status);
//
//    if(!status)
//    {
//        int maxLength = 0;
//        GLsizei length;
//        glGetShaderiv(vprogram, GL_INFO_LOG_LENGTH, &maxLength);
//        char *pInfoLog = new char[maxLength];
//        glGetShaderInfoLog(vprogram, maxLength, &length, pInfoLog);
//        printf("%s shader compile error is %s\n","vertex", pInfoLog);
//        delete [] pInfoLog;
//        glDeleteShader (vprogram);
//        return 0;
//    }
//
//    GLuint  fprogram = glCreateShader(GL_FRAGMENT_SHADER);
//
//    assert(fprogram);
//
//    glShaderSource(fprogram, 1, &fp, NULL);
//
//    glCompileShader(GL_FRAGMENT_SHADER);
//
//    glGetShaderiv(vprogram, GL_COMPILE_STATUS, &status);
//
//    if(!status)
//    {
//        int maxLength = 0;
//        GLsizei length;
//        glGetShaderiv(vprogram, GL_INFO_LOG_LENGTH, &maxLength);
//        char *pInfoLog = new char[maxLength];
//        glGetShaderInfoLog(vprogram, maxLength, &length, pInfoLog);
//        printf("%s shader compile error is %s\n","fragment", pInfoLog);
//        delete [] pInfoLog;
//        glDeleteShader (vprogram);
//        return 0;
//    }
//
//    programid = glCreateProgram();
//
//    glAttachShader(programid, vprogram);
//
//    glAttachShader(programid, fprogram);
//
//    glLinkProgram(programid);
//
//
//    glGetProgramiv(programid, GL_LINK_STATUS, &status);
//    if ( !status )
//    {
//        printf( "GLSL shader program failed to link" );
//        // check error message and log it
//        int maxLength = 0;
//        GLsizei length;
//
//        glGetProgramiv(programid, GL_INFO_LOG_LENGTH, &maxLength);
//
//        char *pInfoLog = new char[maxLength];
//        glGetProgramInfoLog(programid, maxLength, &length, pInfoLog);
//        printf("%s\n",pInfoLog);
//        delete [] pInfoLog;
//
//        glDeleteProgram (programid);
//        return 0;
//    }
//
//
//    return programid;
//}
//
//const char *simplevp = "\
//#version 110\
//attribute vec2 Position;\
//void main() {\
//gl_Positioin = Position;\
//}";
//
//const char *simplefp = "\
//varying vec2 vTexCoord;\
//void main()\
//{\
//\
//}";

//void render()
//{
//    GLuint programid = createProgram(simplevp, simplefp);
//
//    glUseProgram(programid);
//}


#pragma region   模板测试

/********    Functions ********/
 
//// 设置模板缓冲区的写入掩码:当为false时禁止在Stencil Buffer中写入(默认0xff)
//glStencilMask(0xFF);
//// 清除Stencil Buffer的值默认为0
//glClearStencil(clearStencilValue);


//哈哈：来我这儿考试吧，通过的和不通过的都去glStencilOp那儿看看下一步的操作吧
//void glStencilFunc (GLenum func, GLint ref, GLuint mask);
//　　func：
//   GL_NEVER 从来不能通过
//　　GL_ALWAYS 永远可以通过（默认值)
//　　GL_LESS 小于参考值可以通过
//　　GL_LEQUAL 小于或者等于可以通过
//　　GL_EQUAL 等于通过
//　　GL_GEQUAL 大于等于通过
//　　GL_GREATER 大于通过
//　　GL_NOTEQUAL 不等于通过
//　　ref:  参考值
//   mask: 掩码值(会与参考值和模板缓冲区内的值先作与操作，在参考func中的参数测试是否通过测试)

//测试后处理函数
//void glStencilOp (GLenum fail, GLenum zfail, GLenum zpass);
//　　fail模板测试未通过时该如何变化；zfail表示模板测试通过，但深度测试未通过时该如何变化；zpass表示模板测试和深度测试或者未执行深度测试均通过时该如何变化
//　　GL_KEEP（不改变，这也是默认值）
//　　GL_ZERO（回零）
//　　GL_REPLACE（使用测试条件中的设定值来代替当前模板值）
//　　GL_INCR（增加1，但如果已经是最大值，则保持不变），
//　　GL_INCR_WRAP（增加1，但如果已经是最大值，则从零重新开始）
//　　GL_DECR（减少1，但如果已经是零，则保持不变），
//　　GL_DECR_WRAP（减少1，但如果已经是零，则重新设置为最大值）
//　　GL_INVERT（按位取反）

/********    Functions ********/

void  renderStencil()
{
    
    static bool bol = false;
    
    if(!bol)
    {
        _a += 1;
        if( _a > 400)
        {
            bol = true;
        }
    }
    else
    {
        _a -= 1;
        if(_a < 3)
        {
            bol = false;
        }
    }
    
    glClearColor(1, 1, 1, 1);
    
    glClearStencil(0);
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    
    glEnable(GL_STENCIL_TEST);
    
    glStencilFunc(GL_ALWAYS, 1, 1);
    
    glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);
    
    const float innercircle = 0.6;
    glColor3f(0.6,1.0,0.0);
    glRectf( -innercircle, -innercircle, innercircle, innercircle);
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glStencilFunc(GL_EQUAL, 0x1, 0x1);
    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
    
    glColor4f(1.0, 0.0, 0.0,0.5);
    glRectf( -0.8, -0.4, 0.8, 0.8);
    
    glutSwapBuffers();
}
#pragma unregion 模板测试


//绘制动画
void renderAnimate()
{
    glClearColor(1, 1, 1, 1);
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    
    glEnable(GL_STENCIL_TEST);
    
    glStencilFunc(GL_NEVER, 0, 0);
    
    //glStencilOp(GL_INCR, GL_INCR, GL_INCR);
    
    glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);
    
    glColor3f(1.0, 1.0, 1.0);
    
    GLdouble dradius = 1.0;
    
    glLineWidth(1.5);
    
    glBegin(GL_LINE_STRIP);
    glColor3f(1.0, 0.0, 0.0);
    for (double angle = _a; angle < 400.0 + _a; angle += 1)
    {
        glVertex3d(dradius * cos(angle), dradius * sin(angle), 0.0);
        dradius -= 1 / 400.0;
    }
    
    glEnd();
    
    //现在与颜色缓冲区在模板缓冲区对应处有线的地方不会绘制
    glStencilFunc(GL_EQUAL, 0x1, 0x1);
    //glStencilOp(GL_ZERO, GL_ZERO, GL_ZERO);
    
    glColor3f(0.0, 1.0, 1.0);
    glRectf( -0.8, -0.8, 0.8, 0.8);
    
    glutSwapBuffers();
}

void idle()
{
 //   const char* versionString = (const char*) glGetString( GL_VERSION );
    
   
    //renderAnimate();
    
    renderStencil();
}

int main(int argc, char * argv[]) {
    // insert code here...
    
    glutInit(&argc, argv);
    
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_STENCIL);
    
    glutInitWindowPosition(400, 100);
    
    glutInitWindowSize(windowsize, windowsize);
    
    glutCreateWindow("Basic Scene");
    
    glutDisplayFunc(idle);
    
    
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);

    glutIdleFunc(idle);
    glutMainLoop();
    
    return 0;
}
