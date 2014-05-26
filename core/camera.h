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
#ifndef CAMERA_H
#define CAMERA_H

#include <QMatrix4x4>

#include "utils/typedefinitions.h"
#include "utils/maths.h"

class Camera
{
public:
    enum Projection{
        orthographic,
        perspective
    };

    Camera();

    //for the view
    Point3df _position;
    Point3df _lookat;
    Vector3df _up_vector;
    QQuaternion _rotation;



    // for projection types
    float _znear;
    float _zfar;
    float _field_of_view;
    float _aspect_ratio;


    QMatrix4x4 _projection;
    QMatrix4x4 _view;


    const QMatrix4x4& get_view_matrix(){return _view;}
    const QMatrix4x4& get_projection_matrix(){return _projection;}

    void set_view_matrix(const QMatrix4x4& view){_view = view;}
    void set_projection_matrix(const QMatrix4x4& projection){_projection = projection;}

    float get_zfar() const { return _zfar ;}
    float get_znear() const { return _znear; }

    void computePerspective(){
        _projection.perspective(rad2deg(_field_of_view),_aspect_ratio,_znear,_zfar);
    }
//    void computeView(){
//        _view.
//    }

    void set_rotation(const QQuaternion & rotation){
        _rotation = rotation;
    }

    const QQuaternion& get_rotation() const {
        return _rotation;
    }

    const Point3df& get_position() const {return _position;}
    void set_position(const Point3df& position){_position = position;}

    void move(Point3df distance){
        _position += distance;
    }

    void debug()const{
        qDebug()<<"["<<_view(0,0)<<" "<<_view(0,1)<<" "<<_view(0,2)<<" "<<_view(0,3);
        qDebug()<<"["<<_view(1,0)<<" "<<_view(1,1)<<" "<<_view(1,2)<<" "<<_view(1,3);
        qDebug()<<"["<<_view(2,0)<<" "<<_view(2,1)<<" "<<_view(2,2)<<" "<<_view(2,3);
        qDebug()<<"["<<_view(3,0)<<" "<<_view(3,1)<<" "<<_view(3,2)<<" "<<_view(3,3);
    }
    void debug(QMatrix4x4 _view)const{
        qDebug()<<"["<<_view(0,0)<<" "<<_view(0,1)<<" "<<_view(0,2)<<" "<<_view(0,3);
        qDebug()<<"["<<_view(1,0)<<" "<<_view(1,1)<<" "<<_view(1,2)<<" "<<_view(1,3);
        qDebug()<<"["<<_view(2,0)<<" "<<_view(2,1)<<" "<<_view(2,2)<<" "<<_view(2,3);
        qDebug()<<"["<<_view(3,0)<<" "<<_view(3,1)<<" "<<_view(3,2)<<" "<<_view(3,3);
    }

};

#endif // CAMERA_H
