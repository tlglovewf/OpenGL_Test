//
//  main.cpp
//  camera_oper
//
//  Created by TuLigen on 2018/5/10.
//  Copyright © 2018年 TuLigen. All rights reserved.
//

#include <iostream>
#include <stdio.h>
#include <OpenGL/gl.h>
#include <GLUT/glut.h>
#include <math.h>
float radius = 5;
float size   = 20;
int   angle  = 0;

const int   Earth_Radius = 6378137;
const float Earth_Length = 20037508.34;


const int windowsize = 500;

struct Vector2d
{
    float x;
    float y;
};

Vector2d  lnglatToPt( float lon, float lat)
{
    Vector2d pt;
    pt.x =  lon * Earth_Length / 180.0;
    lat  = log(tan((90 + lat) * M_PI / 360)) / (M_PI / 180.0 );
    pt.y = lat * Earth_Length / 180.0;
    return pt;
}

Vector2d lnglatToPt( const Vector2d &geo)
{
    return lnglatToPt(geo.x, geo.y);
}

Vector2d ptToLnglat(const Vector2d &pt)
{
    Vector2d geo;
    geo.x = pt.x * 180.0 / Earth_Length;
    geo.y = pt.y * 180.0 / Earth_Length;

    geo.y =  180 / M_PI * ( 2 * atan(exp(geo.y * M_PI / 180.0)) - M_PI / 2.0);
    return geo;
}
#if 0
void myDisplay(void)

{
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    
    glColor3f(1.0, 0.0, 0.0);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho( -size, size, -size, size, -1, 10);
    
    glRotated(angle, 0, 0, 1);
    glRectd(-radius, -radius, radius, radius);
    
    glRotated(-angle, 0, 0, 1);
    glBegin(GL_LINES);
 
    glColor3f(0.0,1.0,0.0);
    glVertex3f(0, 0, 0);
    glVertex3f(size, 0, 0);
    
    glColor3f(0.0, 0.0, 1.0);
    glVertex3f(0, 0, 0);
    glVertex3f(0, size, 0);

    glEnd();
    
    glutSwapBuffers();
}
#else
const float lon = 116;
const float lat = 34;
void setCamera()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective( 45.0, 1, 1, 100);
    
    gluLookAt( 10, 10, 10, 0, 0, 0, 0, 1, 0);
}
const float axislen = 0.6;
void renderAxis()
{
    glPushMatrix();
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective( 45.0, 1, 1, 100);
    gluLookAt( 1, 1, 1, 0, 0, 0, 0, 1, 0);
    
    glLineWidth(3);
    glBegin(GL_LINES);

    glColor3f(1.0, 0.0, 0.0);
    glVertex3f(0, 0, 0);
    glVertex3f(axislen, 0, 0);
    
    glColor3f(0.0, 1.0, 0.0);
    glVertex3f(0, 0, 0);
    glVertex3f(0, axislen, 0);
    
    glColor3f(0.0, 0.0, 1.0);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 0, axislen);
    
    glEnd();
    
    
    
    glPointSize(5.0);
    glBegin(GL_POINTS);
    
    glColor3f(1.0, 0.0, 0.0);
    glVertex3f(axislen, 0, 0);
    
    glColor3f(0.0, 1.0, 0.0);
    glVertex3f(0, axislen, 0);
    
    glColor3f(0.0, 0.0, 1.0);
    glVertex3f(0, 0, axislen);
    
    glEnd();
    
    glPopMatrix();
}

void myDisplay(void)
{
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    
    setCamera();
    
    renderAxis();
    
    glDisable(GL_DEPTH);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glBegin(GL_TRIANGLES);
    
    glColor4f(1.0, 0.0, 0.0,0.5);
    
    glVertex3f(5, 5, 5);
    glVertex3f(5, 0, 0);
    glVertex3f(0, 5, 0);
    
    glEnd();
    
    glutSwapBuffers();
}

#endif



void keyboard(unsigned char ch,int x,int y)
{
   switch(ch)
    {
        case 'a':
            radius+=1.0;
            break;
        case 'd':
            radius-=1.0;
            break;
        case 'w':
            angle+=10;
            break;
        case 's':
            angle-=10;
            break;
        default:
            break;
    }
}

void mouse(int x1,int y1, int x2, int y2)
{
    
}

void idle()
{
    myDisplay();
}

int main(int argc, char * argv[]) {
    
    glutInit(&argc, argv);
    
    glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
    
    glutInitWindowPosition(200, 200);
    
    glutInitWindowSize(windowsize, windowsize);
    
    glutCreateWindow("第一个OpenGL程序");
    
    glutDisplayFunc(&myDisplay);
    
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    
    glutIdleFunc(idle);
    glutMainLoop();
    
    return 0;
}
