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

void  display()
{
    glClearColor(1, 1, 1, 1);
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glutSwapBuffers();
}

void idle()
{
    display();
}

int main(int argc, char * argv[]) {
    // insert code here...
    glutInit(&argc, argv);
    
    glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
    
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
