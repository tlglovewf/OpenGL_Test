//
//  main.cpp
//  XCodeOpengl
//
//  Created by TuLigen on 15/9/22.
//  Copyright (c) 2015年 TuLigen. All rights reserved.
//

#include <stdio.h>
#include <OpenGL/gl.h>
#include <GLUT/glut.h>
#include <math.h>
#include <algorithm>
#include <vector>

const int windowSize = 512;

#pragma region  //frame
enum {Color, Depth, NumRenderbuffers};

GLuint framebuffer,renderbuffer[NumRenderbuffers];

void frameinit()
{
    glGenRenderbuffers(NumRenderbuffers, renderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer[Color]);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA, 256, 256);

    glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer[Depth]);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 256, 256);


    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, renderbuffer[Color]);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, renderbuffer[Depth]);
}

void  framedisplay()
{
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

   // glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glViewport(0, 0, 256, 256);
   
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(1.0, 1.0, 1.0, 1.0);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);

    glBegin(GL_POLYGON);
    glColor4f(1.0, 0, 0, 0.5);
    
    glVertex3f(0.1, 0.1, 0.0);
    glVertex3f(0.9, 0.1, 0.0);
    glVertex3f(0.9, 0.9, 0.0);
    glVertex3f(0.1, 0.9, 0.0);
    
    glEnd();

    glBindFramebuffer(GL_READ_FRAMEBUFFER, framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glViewport(0, 0, 256, 256);
    glClearColor(1, 1,1, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);

    glBegin(GL_POLYGON);
    glColor4f(0, 1.0, 0, 1.0);
    glVertex3f(0.25, 0.25, 0.0);
    glVertex3f(0.75, 0.25, 0.0);
    glVertex3f(0.75, 0.75, 0.0);
    glVertex3f(0.25, 0.75, 0.0);
    glEnd();
    
    glBlitFramebuffer(0, 0, 256, 256, 128, 128, 256, 256, GL_COLOR_BUFFER_BIT, GL_NEAREST);
    glutSwapBuffers();
}

#pragma unregion



#pragma region  //texture frame


GLsizei TexWidth =  128;
GLsizei TexHeight=  128;
GLuint  texframebuffer, rendertarget;



void texframeInit(void)
{
    // 创建纹理
    glGenTextures(1, &rendertarget);
    glBindTexture(GL_TEXTURE_2D, rendertarget);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, TexWidth, TexHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
    
    /*
     // 创建深度缓冲区
     glGenRenderbuffersEXT(1, &depthbuffer);
     glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, depthbuffer);
     glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT, TEXTURE_WIDTH, TEXTURE_HEIGHT);
     glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0);
     */
    
    // 创建FBO对象
    glGenFramebuffers(1, &texframebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, texframebuffer);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, rendertarget, 0);
    //glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, depthbuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if(status != GL_FRAMEBUFFER_COMPLETE)
    {
        
    }
}

void setcamera()
{
    
}
float dt = 0.01;
void renderToTexture()
{
    glBindTexture(GL_TEXTURE_2D, 0); // 取消绑定，因为如果不取消，渲染到纹理的时候会使用纹理本身
    
    // 绑定渲染目标
    glBindFramebuffer(GL_FRAMEBUFFER, texframebuffer);
    glViewport(0,0,TexWidth, TexHeight);
        glDisable(GL_CULL_FACE);
    // 渲染
    glClearColor( 1, 0, 0.1, 0.1 );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
//    gluPerspective(60 , TexWidth / TexHeight, 1, 2000);
//    gluLookAt(0, 0, 2 * windowSize, 0, 0, 0, 1, -1, 0);
    glOrtho(0, windowSize, 0 , windowSize , -10, 10);
    glBegin(GL_TRIANGLES);
    glColor4f(1, 0, 0, 0.5);
 
    //
    glVertex3d( windowSize,0, 0);
    glVertex3d(0, 0, 0);
    glVertex3d( 0, windowSize, 0);
    
    glEnd();
    
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

typedef unsigned char  u8;
#define MaxSize  1024
u8 data[MaxSize] = {0};
const int width = (MaxSize >> 2);
const int height = 1;

struct SColor4f
{
    SColor4f(float _r,float _g, float _b, float _a):r(_r),g(_g),b(_b),a(_a){}
    SColor4f(unsigned int color)
    {
        a = ((color & 0xff000000) >> 24) / 255.0;
        r = ((color & 0x00ff0000) >> 16) / 255.0;
        g = ((color & 0x0000ff00) >> 8)  / 255.0;
        b = (color & 0x000000ff)         / 255.0;
    }
    float a;
    float r;
    float g;
    float b;
    
    void display()const
    {
        printf(" a : %f,r : %f,g : %f,b : %f\n",a * 255,r * 255,g * 255,b * 255);
    }
};

class ColorBand
{
public:
#define BandLength  1024// 256 * 4
    struct Item
    {
        SColor4f _color;
        float    _pos;
        Item( const SColor4f &color, float pos):_color(color),_pos(pos){}
    };
    typedef std::vector<Item>       ClrVector;
    typedef ClrVector::const_iterator ClrVectorIter;
    
    ColorBand()
    {
        memset(data, 0, sizeof(u8) * 1024);
    }
    void addItem( const SColor4f &color, float pos)
    {
        colors.push_back(Item(color,pos));
    }
    
    ClrVectorIter first()const
    {
        return colors.begin();
        
    }
    
    ClrVectorIter end()const
    {
        return colors.end();
    }
    
    
    SColor4f  getColor( float pos )
    {
        int index = 0;
        const int sz = colors.size();
        for(int i = 0; i < sz; ++i)
        {
            if( pos < colors[i]._pos && pos > 0.0)
            {
                index = i;
                break;
            }
        }
        if( 0 == index )
        {
            return colors[0]._color;
        }
        else if (index < sz)
        {
            const int preindex = index - 1;
            const SColor4f &next = colors[index]._color;
            const SColor4f &now  = colors[preindex]._color;
            
            float dta = next.a - now.a;
            float dtr = next.r - now.r;
            float dtg = next.g - now.g;
            float dtb = next.b - now.b;
            
            float dt = ( pos - colors[preindex]._pos) / ( colors[index]._pos - colors[index - 1]._pos);
            float a = colors[preindex]._color.a + dt * dta ;
            float r = colors[preindex]._color.r + dt * dtr;
            float g = colors[preindex]._color.g + dt * dtg;
            float b = colors[preindex]._color.b + dt * dtb;
            return SColor4f(r,g,b,a);
        }
        else
        {
            return colors[sz - 1]._color;
        }
    }
    
    void generate()
    {
        std::sort(colors.begin(),colors.end(),[]( const Item &left, const Item &rigt)->bool{return left._pos < rigt._pos;});//排序
        for(int i = 0; i < BandLength; i+= 4)
        {
            SColor4f clr = getColor( i / (float)BandLength );
            clr.display();
            data[i + 0]  = floor(clr.r * 255);
            data[i + 1]  = floor(clr.g * 255);
            data[i + 2]  = floor(clr.b * 255);
            data[i + 3]  = floor(clr.a * 255);
        }
    }
    
    u8* getData()
    {
        return data;
    }
    
private:
    ClrVector colors;
    u8        data[BandLength];
};

void build()
{
    for(int i = 0; i < MaxSize; i+= 4)
    {
        data[i + 0] = floor((255 * (i /(float)MaxSize)));
        data[i + 1] = 0;
        data[i + 2] = 0;
        data[i + 3] = 255;
    }
}
void renderToScreen()
{
    build();
    // 绑定默认FBO（窗体帧缓冲区的ID是0）
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
    glViewport(0,0,windowSize, windowSize);
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glDisable(GL_DEPTH_TEST);
    
    // 渲染
    glClearColor( 1, 1, 1, 1 );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glEnable(GL_TEXTURE_2D);
//    glBindTexture(GL_TEXTURE_2D, rendertarget);
    
    glGenTextures(1, &rendertarget);
    glBindTexture(GL_TEXTURE_2D,rendertarget);
    ColorBand band;

    band.addItem(0xff0000ff, 0.1);
    band.addItem(0xff28c8c8, 0.2);
    band.addItem(0xff8cc820, 0.5);
    band.addItem(0xff00ff00, 0.6);
    band.addItem(0xffffff36, 0.8);
    band.addItem(0xffff0000, 1.0);
    
    band.generate();
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, band.getData());
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glBegin(GL_POLYGON);
    
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
void texframeDisplay(void)
{
    renderToTexture();
    renderToScreen();
}

#pragma unregion


void idle()
{
    if(dt > 360)
    {
        dt = 0;
    }
    else
    {
        dt += 1;
    }
    texframeDisplay();
}

int main(int argc, char *argv[] )
{
    printf(" Hello , World.\n");
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH|GLUT_SINGLE|GLUT_RGBA);
    glutInitWindowSize(windowSize, windowSize);
    glutInitWindowPosition(200, 200);
    glutCreateWindow("hello");
    texframeInit();
    glutDisplayFunc(texframeDisplay);
    //glutIdleFunc(idle);
    glutMainLoop();
    return 0;
}
