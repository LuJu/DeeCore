#include "uistate.h"

UIState::UIState():QObject()
{
    _action_done=false;
    for (int i=0;i<NUMBER_OF_ACTIONS;i++) _actions[i]=false;
    _paused=false;
    loadPreviousState();
    _zoom_targeted=_zoom_level;
}

void UIState::loadPreviousState(){
    _rotate=Point3di(GlobalConfig::get_int("rotate_x"),
                     GlobalConfig::get_int("rotate_y"),
                     GlobalConfig::get_int("rotate_z"));
    _zoom_level=GlobalConfig::get_int("zoom");
}

UIState * UIState::instance(){
    static UIState _instance;
    return & _instance;
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
    xangle = mouse_coordinates.x();
    QMatrix4x4 rotation;
    qDebug()<<xangle;
    if( xangle < 0) rotation.rotate(-xangle,0,-1,0);
    else rotation.rotate(xangle,0,1,0);

    yangle = (float)mouse_coordinates.y()/2;
    if( yangle < 0) rotation.rotate(-yangle,-1,0,0);
    else rotation.rotate(yangle,1,0,0);

    _rotation = _rotation* rotation.transposed() ;
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
                _rotate.y(_rotate.y()+2);
                if(_rotate.y() >= 360 )_rotate.y(_rotate.y()-360);
                rotation.rotate(1,0,1,0);
                _quaternion = _quaternion * QQuaternion(1,0,1,0);
                break;
            case right:
                _rotate.y(_rotate.y()-2);
                if(_rotate.y() < 0)_rotate.y(_rotate.y()+360);
                rotation.rotate(-1,0,1,0);
                break;
            case up:
                _rotate.x(_rotate.x()+2);
                if(_rotate.x() >= 360 )_rotate.x(_rotate.x()-360);
                rotation.rotate(1,1,0,0);
                break;
            case down:
                _rotate.x(_rotate.x()-2);
                if(_rotate.x() < 0)_rotate.x(_rotate.x()+360);
                rotation.rotate(-1,1,0,0);
                break;
            case forward:
                _rotate.z(_rotate.z()+2);
                if(_rotate.z() >= 360 )_rotate.z(_rotate.z()-360);
                rotation.rotate(1,0,0,1);
                break;
            case backward:
                _rotate.z(_rotate.z()-2);
                if(_rotate.z() < 0)_rotate.z(_rotate.z()+360);
                rotation.rotate(-1,0,0,1);
                break;
            }
        }
    }
    _rotation =  _rotation * rotation;

//    _rotation=QMatrix4x4();
//    _rotation.rotate(_rotate.x(),1,0,0);
//    _rotation.rotate(_rotate.y(),0,1,0);
//    _rotation.rotate(_rotate.z(),0,0,1);
}

void UIState::saveState(){
    GlobalConfig::set_int("rotate_x",_rotate.x());
    GlobalConfig::set_int("rotate_y",_rotate.y());
    GlobalConfig::set_int("rotate_z",_rotate.z());
    GlobalConfig::set_int("zoom",_zoom_level);
}

bool UIState::action_going (int action){
    return _actions[action];
}
