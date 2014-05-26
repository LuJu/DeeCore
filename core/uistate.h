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
#ifndef UISTATE_H
#define UISTATE_H

#include <Qt>
#include <QDebug>
#include <QTimer>
#include <QObject>
#include <QPoint>

#include "utils/maths.h"
#include "utils/typedefinitions.h"

#include "timing.h"
#include "globalconfig.h"
#include "camera.h"

//! class defining the state of the UI (zoom, x, y...)
/*!
    \n Status  1 : not implemented
*/
class UIState : public QObject
{
    Q_OBJECT
public slots:
public:
    enum Actions {
        left=0, right, up, down,
        forward,backward,
        action, back, option,
        e_,
        ru,rd,rl,rr,
        NUMBER_OF_ACTIONS,
        none
    };

    enum Types {
        camera2D,
        camera3D
    };

    Types _type;


    //! Get the zoom value of the view
    /*!
        The zoom value goes from 0 to 1000
    */
    inline int get_zoom(){return _zoom_level;}
    inline void set_zoom(int zoom){_zoom_level=zoom;}

    inline QPoint get_mouse_position(){return _mouse_pos;}
    inline void set_mouse_position(QPoint mouse_pos){ _mouse_pos=mouse_pos;}
    inline int recordAction(){int ret = _action_done; _action_done=none; return ret;}
    inline void set_action(quint8 id, bool action){if(id<NUMBER_OF_ACTIONS)_actions[id]=action;}

    //! Change the zoom value
    /*!
        Changes the zoom value in function of the delta given in parameter
        \n Status 1 : not implemented
        \param  delta The change (negative or positive) asked
    */
    void requireLevelChange(int delta);
    bool action_going (int action);

    bool is_paused(){ return _paused;}
    void set_paused(bool paused){_paused=paused;}

    void updateState();
    void saveState();
    void changeZoom(int delta);

//    void insertMatrixes(QGLShaderProgram program){
//        program->setUniformValue("P",_camera.get_projection_matrix());
//        program->setUniformValue("",_camera.get_projection_matrix());
//    }

    QQuaternion _quaternion;
    void rotate(QPoint mouse_coordinates);
    void loadPreviousState();
    UIState();
    ~UIState();

    void activateProgressiveZoom(float speed){
        _progressive_zoom._activated = true;
        _progressive_zoom._speed= speed;
        _progressive_zoom._targeted = _zoom_level;
        if (speed == 0) qDebug()<<"Invalid speed value";
    }

    void stopProgressiveZoom(){
        _progressive_zoom._activated = false;
    }

    Camera& get_camera(){return _camera;}
    void displayCameraInformation();
    float fov;
private:

    UIState(const UIState&);
    void actionProcess();


//    Point3df _position;
    Point3df _previous_position;

    float _zoom_level;
    bool _actions[NUMBER_OF_ACTIONS];
    int _action_done;
    QPoint _mouse_pos;
    bool _paused;

    struct {
        float _speed;
        float _targeted;
        bool _activated;

    }_progressive_zoom;

    Camera _camera;
};

#endif // UISTATE_H
