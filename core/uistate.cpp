#include "uistate.h"

UIState::UIState():
    QObject(),
    _position( 0,-5.5,1)
{
    _action_done=false;
    for (int i=0;i<NUMBER_OF_ACTIONS;i++) _actions[i]=false;
    _paused=false;
    loadPreviousState();
    _zoom_targeted=_zoom_level;
}

void UIState::loadPreviousState(){
    _quaternion = _quaternion.fromAxisAndAngle(
                GlobalConfig::get_float("quaternion_x"),
                GlobalConfig::get_float("quaternion_y"),
                GlobalConfig::get_float("quaternion_z"),
                GlobalConfig::get_float("quaternion_w"));
    qDebug()<<"angle :";
    qDebug()<<_quaternion.x();
    qDebug()<<_quaternion.y();
    qDebug()<<_quaternion.z();
    qDebug()<<_quaternion.scalar();
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
    if(_zoom_level+delta > 10000)     _zoom_level = 10000 ;
    else if (_zoom_level+delta < 0) _zoom_level = 0 ;
    else _zoom_level = _zoom_level+delta;
}


void UIState::rotate(QPoint mouse_coordinates){
    float xangle = 0.0;
    float yangle = 0.0;
    float xangle2 = 0.0;
    float yangle2 = 0.0;
    xangle2 = - mouse_coordinates.x();
    yangle2 = mouse_coordinates.y();
    xangle = (float)(mouse_coordinates.x());
    yangle = (float)(mouse_coordinates.y());
    _mouse_pos = mouse_coordinates;
    float angle = 0.0;
//    angle = - qSin(yangle2);
    QVector3D vector(xangle2,yangle2,1);
    qDebug()<<"vector"<<vector.x()<<" "
              <<vector.y()<<" "
                <<vector.z()<<" ";
    vector = QVector3D::crossProduct(vector,QVector3D(0,0,1));
    QQuaternion quaternion = QQuaternion::fromAxisAndAngle(vector,5);
//    QQuaternion quaternion(deg2rad(yangle),QVector3D(1,0,0));
//    quaternion.normalize();
//    quaternion = QQuaternion::fromAxisAndAngle(1,0,0,yangle);
//     QQuaternion quaternion2 = QQuaternion::fromAxisAndAngle(0,1,0,xangle);
//    _quaternion = quaternion * quaternion2 * _quaternion ;
    _quaternion = quaternion *_quaternion ;
}

void UIState::updateState(){
    actionProcess();
}

void UIState::actionProcess(){
    QMatrix4x4 rotation;
    for (int i=0;i<NUMBER_OF_ACTIONS;i++){
        if(_actions[i]==true){
            switch (i){
            case left:
                _position+=Point3df(-1,0,0);
                break;
            case right:
                _position+=Point3df(1,0,0);
                break;
            case up:
                _position+=Point3df(0,-1,0);
                break;
            case down:
                _position+=Point3df(0,1,0);
                break;
            case forward:
                _position+=Point3df(0,0,-1);
                break;
            case backward:
                _position+=Point3df(0,0,1);
                break;
            }
        }
    }
}


bool UIState::action_going (int action){
    return _actions[action];
}
