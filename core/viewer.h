/*
Copyright (c) 2013, Lucas Juliéron
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
this list of conditions and the following disclaimer in the documentation
and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS
BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.
*/
#ifndef VIEWER_H
#define VIEWER_H

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <qgl.h>
#include <QtOpenGL>
#include <QGLWidget>
#include <QGLContext>
#include <QGLShaderProgram>
#include <QGLShader>
#include <QVector3D>

//#include <QGLViewer/qglviewer.h>
//#include <QGLViewer/vec.h>

#include <string>
#include <iostream>
#include "core/core.h"

#include "3d/graphicobject3d.h"

#include "GL/glext.h"

class Viewer : public QGLWidget
{
    Q_OBJECT
public:
    Viewer(const QGLFormat &format);
    Viewer(QGLContext * context);
    Viewer(QWidget * parent = 0, const QGLWidget * shareWidget = 0, Qt::WindowFlags f = 0);
//    Viewer(QWidget * parent = 0, const QGLWidget * shareWidget = 0, Qt::WindowFlags f = 0 ) : QGLWidget ( parent, shareWidget,f ){__build();}
    ~Viewer();
    void deleteData();
//    bool _main_viewer;

    QVector<GLuint> textures() const;
    void setTextures(const QVector<GLuint> &textures);

public slots :
    void framepersecond();
    void animate();


protected :
    virtual void draw();
    virtual void init();

    virtual void mousePressEvent(QMouseEvent * mouseEvent);
    virtual void mouseReleaseEvent(QMouseEvent * event);
    virtual void wheelEvent(QWheelEvent * wheelEvent);
    virtual void mouseMoveEvent(QMouseEvent * event);
    virtual void keyPressEvent(QKeyEvent *keyEvent);
    virtual void keyReleaseEvent(QKeyEvent *keyEvent);
    virtual void closeEvent(QCloseEvent * event);

    virtual void addTo3DDisplayList(GraphicObject3D * object);
    virtual void display3DObjects();

    //! Draw the background
    void display2D();
    void displayFullTextured(int x, int y, int width, int height);

    void startShaders();
    virtual void resizeGL(int width, int height);




    //!Function called at the end of each frame
    void frameEnd();

    void paintGL(){draw();}


    void updateMatrices(const QMatrix4x4& P,const QMatrix4x4& V,const QMatrix4x4& M);
    void updateModelMatrix(const QMatrix4x4 M);

    QGLShaderProgram * _program;
    QGLShader * _shaders[3];

    //! List of 3D objects on the scene. Only the objects on this array will be rendered
    QVector<GraphicObject3D * > _3D_display_list;

    UIState * _ui;
    InputManager * _input;

    //! The count of frames passed since the beginning of the last second
    int _frame;

    //! Timer used for FPS count
    QTimer * _timer_fps;
    QTimer * _timer_start;
    QTime _time;
    int _background_position;
    bool _background_activated;
    void releaseProgram();
    void bindProgram();

private :
    void __build(){/*init();*/}
    GLint _viewport[4];
    QGLShader * compileShader(const char * path, QGLShader::ShaderType type);
    void initializeGL(){init();}

    QPoint _last_position;
    void updateLastMousePosition(const QPoint& position){
        _last_position = position;
    }


    bool _initiated;
    void debugData(const qreal* data);
    void debugDataGL(const GLfloat *data);

};
#endif // VIEWER_H
