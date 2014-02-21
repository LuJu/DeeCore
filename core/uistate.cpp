#include "uistate.h"

UIState::UIState():
    QObject()/*,
    _position( 0,-5.5,1)*/
{
    _action_done=false;
    for (int i=0;i<NUMBER_OF_ACTIONS;i++) _actions[i]=false;
    _paused=false;
    loadPreviousState();
    _progressive_zoom._targeted=_zoom_level;
    _progressive_zoom._activated = false;
    _progressive_zoom._speed = 1;
    _camera.set_position(Point3df(-9,-8.5,92));
    fov=90;
}

void UIState::loadPreviousState(){
    _quaternion = _quaternion.fromAxisAndAngle(
                GlobalConfig::get_float("quaternion_x"),
                GlobalConfig::get_float("quaternion_y"),
                GlobalConfig::get_float("quaternion_z"),
                GlobalConfig::get_float("quaternion_w"));
    _zoom_level=GlobalConfig::get_int("zoom");
}

void UIState::saveState(){
    _quaternion.normalize();
    GlobalConfig::set_float("quaternion_x",_quaternion.x());
    GlobalConfig::set_float("quaternion_y",_quaternion.y());
    GlobalConfig::set_float("quaternion_z",_quaternion.z());
    GlobalConfig::set_float("quaternion_w",_quaternion.scalar());
    GlobalConfig::set_int("zoom",_zoom_level);
}

UIState::~UIState(){
}

void UIState::changeZoom(int delta){
    if (!_progressive_zoom._activated){
        if(_zoom_level+delta > 20000)     _zoom_level = 20000 ;
        else if (_zoom_level+delta < 0) _zoom_level = 0 ;
        else _zoom_level = _zoom_level+delta;
    } else {
        if(_progressive_zoom._targeted+delta > 20000)     _progressive_zoom._targeted = 20000 ;
        else if (_progressive_zoom._targeted+delta < 0) _progressive_zoom._targeted = 0 ;
        else _progressive_zoom._targeted = _progressive_zoom._targeted+delta;
    }
        displayCameraInformation();
}

void UIState::displayCameraInformation(){
//    qDebug()<<"Position :"<<_camera.get_position().x()<<" "<<_camera.get_position().y()<<" "<<_camera.get_position().z();
    qDebug()<<"zoom :"<<_zoom_level;
}


void UIState::rotate(QPoint mouse_coordinates){
    float xangle,yangle;
    xangle = - mouse_coordinates.x();
    yangle = mouse_coordinates.y();
//    _mouse_pos = mouse_coordinates;
    QVector3D vector = QVector3D::crossProduct(QVector3D(xangle,yangle,0),QVector3D(0,0,1));

    QQuaternion quaternion = QQuaternion::fromAxisAndAngle(vector,mouse_coordinates.manhattanLength());

    _quaternion = quaternion *_quaternion ;
}

void UIState::updateState(){
    actionProcess();
}

void UIState::actionProcess(){
    QQuaternion q ;
    for (int i=0;i<NUMBER_OF_ACTIONS;i++){
        if(_actions[i]==true){
            switch (i){
            case left:
                _camera.move(Point3df(-3,0,0));
                break;
            case right:
                _camera.move(Point3df(3,0,0));
                break;
            case up:
                _camera.move(Point3df(0,3,0));
                break;
            case down:
                _camera.move(Point3df(0,-3,0));
                break;
            case forward:
                _camera.move(Point3df(0,0,-3));
                break;
            case backward:
                _camera.move(Point3df(0,0,3));
                break;
//            case a_:
//                fov++;
//                qDebug()<<fov;
//                break ;
            case ru:
                q = QQuaternion::fromAxisAndAngle(QVector3D(1,0,0),1);
                _camera.set_rotation( _camera.get_rotation()* q);
                break;
            case rd:
                q = QQuaternion::fromAxisAndAngle(QVector3D(1,0,0),1);
                _camera.set_rotation( q*_camera.get_rotation());
                break;
            case e_:
                fov--;
                qDebug()<<fov;
                break ;
            }
        }
    }
    if (_progressive_zoom._activated){
        if (_progressive_zoom._targeted != _zoom_level) {
            if (absolute_value( _zoom_level - _progressive_zoom._targeted ) <= _progressive_zoom._speed) _zoom_level = _progressive_zoom._targeted;
            else if (_zoom_level < _progressive_zoom._targeted) _zoom_level += _progressive_zoom._speed;
            else if (_zoom_level > _progressive_zoom._targeted) _zoom_level -= _progressive_zoom._speed;
        }
    }
}


bool UIState::action_going (int action){
    return _actions[action];
}
