//
//  FBO_Texture.cpp
//  XCodeOpengl
//
//  Created by TuLigen on 2018/4/25.
//  Copyright © 2018年 TuLigen. All rights reserved.
//

#include <stdio.h>
#include <OpenGL/gl.h>
#include <GLUT/glut.h>

/****************************************************************************************************
 * 全局变量定义
 *****************************************************************************************************/
const int SCREEN_WIDTH = 512;
const int SCREEN_HEIGHT = 512;
const int TEXTURE_WIDTH = 512;
const int TEXTURE_HEIGHT = 512;
const double NEAR_PLANE = 1.0f;
const double FAR_PLANE = 1000.0f;

GLuint fbo = 0;        // FBO对象的句柄
GLuint depthbuffer = 0;
GLuint rendertarget = 0;        // 纹理对象的句柄


/****************************************************************************************************
 * 全局函数定义
 *****************************************************************************************************/
void SetupWindow(void)
{
    int argc = 0; char* argv[] = {0};
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH|GLUT_SINGLE|GLUT_RGBA);
    
    glutInitWindowSize(800, 600);
    glutCreateWindow("Framebuffer Sample");

}

// 初始化摄像机
void SetupCamera(void)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //gluPerspective(45, (double)SCREEN_WIDTH/(double)SCREEN_HEIGHT, NEAR_PLANE, FAR_PLANE);
    //gluLookAt(5, 5, 5, 0, 0, 0, 0, 1, 0);
    
    // 各种变换应该在GL_MODELVIEW模式下进行
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    // Z-buffer
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    
    // 启用2D贴图
    glEnable(GL_TEXTURE_2D);
}

// 初始化几何形体
void SetupResource(void)
{
    // 创建纹理
    glGenTextures(1, &rendertarget);
    glBindTexture(GL_TEXTURE_2D, rendertarget);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, TEXTURE_WIDTH, TEXTURE_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
    
    /*
     // 创建深度缓冲区
     glGenRenderbuffersEXT(1, &depthbuffer);
     glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, depthbuffer);
     glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT, TEXTURE_WIDTH, TEXTURE_HEIGHT);
     glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0);
     */
    
    // 创建FBO对象
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);
    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, rendertarget, 0);
    //glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, depthbuffer);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    
    GLenum status = glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER);
    if(status != GL_FRAMEBUFFER_COMPLETE)
    {
        
    }
}

// 渲染到窗体
void Render(void)
{
    // 绑定默认FBO（窗体帧缓冲区的ID是0）
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

    glViewport(0,0,SCREEN_WIDTH, SCREEN_HEIGHT);
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glDisable(GL_DEPTH_TEST);
    
    // 渲染
    glClearColor( 1, 1, 1, 1 );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glBindTexture(GL_TEXTURE_2D, rendertarget);
    glBegin(GL_POLYGON);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glColor3f(1, 1, 1);

    glTexCoord2f(1, 1);
    glVertex3d( 1,  1, 0);

    glTexCoord2f(0, 1);
    glVertex3d(-1,  1, 0);

    glTexCoord2f(0, 0);
    glVertex3d(-1, -1, 0);

    glTexCoord2f(1, 0);
    glVertex3d( 1, -1, 0);

    glEnd();
    
    glBindTexture(GL_TEXTURE_2D, 0);
    
    glBegin(GL_LINES);
    glLineWidth(10.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    glColor4f(1.0,0.0,0.0,1.0);
    
    glVertex3d(-1, -1, 0);
    glVertex3d(1, 1, 0);
    
    glVertex3d(-1, 1, 0);
    glVertex3d(1, -1, 0);
    
    glEnd();
    
    glutSwapBuffers();
}


// 渲染到纹理
void RenderToTarget(void)
{
    glBindTexture(GL_TEXTURE_2D, 0); // 取消绑定，因为如果不取消，渲染到纹理的时候会使用纹理本身
    
    // 绑定渲染目标
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);
    glViewport(0,0,TEXTURE_WIDTH, TEXTURE_HEIGHT);
 
    // 渲染
    glClearColor( 0, 0, 0.1, 0.0 );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
  
    glBegin(GL_TRIANGLES);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glColor4f(1, 0, 0, 0.5);
    glVertex3d( 0,  1, 0);
    glVertex3d(-1, -1, 0);
    glVertex3d( 1, -1, 0);

    glEnd();
    
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Clear(void)
{
    
}

void renderScene()
{
    RenderToTarget();
    Render();
    //Render1();
}

/****************************************************************************************************
 * 主程序入口
 *****************************************************************************************************/
int main(int argc, char* argv[])
{
    SetupWindow();
    SetupCamera();
    SetupResource();
    glutDisplayFunc(renderScene);
    glutMainLoop();
    return 0;
}

