#include "uistate.h"

UIState::UIState():
    QObject()
{
    _action_done=false;
    for (int i=0;i<NUMBER_OF_ACTIONS;i++) _actions[i]=false;
    _paused=false;
    _progressive_zoom._targeted=_zoom_level;
    _progressive_zoom._activated = false;
    _progressive_zoom._speed = 1;
    _camera.set_position(Point3df(0,0,-1.0));
    QQuaternion quat = QQuaternion::fromAxisAndAngle(0,1,0,0);
    _camera.set_rotation(quat);
}
UIState::~UIState(){
}

void UIState::changeZoom(int delta){
    int max_zoom = 150;

    if (!_progressive_zoom._activated){
        if(_zoom_level+delta > max_zoom)     _zoom_level = max_zoom ;
        else if (_zoom_level+delta < 0) _zoom_level = 0 ;
        else _zoom_level = _zoom_level+delta;
    } else {
        if(_progressive_zoom._targeted+delta > max_zoom)     _progressive_zoom._targeted = max_zoom ;
        else if (_progressive_zoom._targeted+delta < 0) _progressive_zoom._targeted = 0 ;
        else _progressive_zoom._targeted = _progressive_zoom._targeted+delta;
    }
}

void UIState::rotate(QPoint mouse_coordinates){
    float xangle,yangle;
    xangle = - mouse_coordinates.x();
    yangle =   mouse_coordinates.y();

    QQuaternion qq1,qq2;
    QVector3D axis1,axis2;
    axis1 = _camera.get_rotation().conjugate().rotatedVector(QVector3D(0,1,0));
    axis2 = _camera.get_rotation().conjugate().rotatedVector(QVector3D(-1,0,0));
    qq1 = QQuaternion::fromAxisAndAngle(axis1,-mouse_coordinates.x());
    qq2 = QQuaternion::fromAxisAndAngle(axis2,-mouse_coordinates.y());
//    qq1 = QQuaternion::fromAxisAndAngle(QVector3D(0,1,0),-mouse_coordinates.x());
//    qq2 = QQuaternion::fromAxisAndAngle(QVector3D(1,0,0),-mouse_coordinates.y());
    QVector3D vector = QVector3D::crossProduct(QVector3D(xangle,yangle,0),QVector3D(0,0,-1));

    QQuaternion quaternion = QQuaternion::fromAxisAndAngle(vector,mouse_coordinates.manhattanLength());
//    QQuaternion quaternion = qq1 * qq2;
//    qDebug()<<"move : "<<mouse_coordinates.x()<< " "<<mouse_coordinates.y()<<" "<<mouse_coordinates.manhattanLength();
    QQuaternion _quaternion;
    quaternion.normalize();
    _quaternion=_camera.get_rotation();

//    _quaternion = _quaternion * quaternion * _quaternion.conjugate();
    _quaternion =  quaternion * _quaternion;
    _camera.set_rotation(_quaternion);
}

void UIState::updateState(){
    actionProcess();
}

void UIState::actionProcess(){
    QQuaternion q ;
    QVector3D axis;
//    QVector3D axis2 = _camera.get_rotation().rotatedVector();
    Point3df move;
    for (int i=0;i<NUMBER_OF_ACTIONS;i++){
        if(_actions[i]==true){
            switch (i){
            case left:

                axis = _camera.get_rotation().conjugate().rotatedVector(QVector3D(1,0,0)/10.0f);
                break;
            case right:

                axis = _camera.get_rotation().conjugate().rotatedVector(QVector3D(-1,0,0)/10.0f);
                break;
            case up:

                axis = _camera.get_rotation().conjugate().rotatedVector(QVector3D(0,0,-1)/10.0f);
                break;
            case down:

                axis = _camera.get_rotation().conjugate().rotatedVector(QVector3D(0,0,1)/10.0f);
                break;
            case forward:

                axis = _camera.get_rotation().conjugate().rotatedVector(QVector3D(0,-1,0)/10.0f);

                break;
            case backward:
                axis = _camera.get_rotation().conjugate().rotatedVector(QVector3D(0,1,0)/10.0f);



                break;
            }
        }
    }
    _camera.move(Point3df(axis.x(),axis.y(),axis.z()));
    move = Point3df(axis.x(),axis.y(),axis.z());
    QVector3D rotation = _camera.get_rotation().vector();
    float angle = _camera.get_rotation().scalar();
    Point3df position = _camera.get_position();
//    _camera.move(move);
//    qDebug()<<"axis:"<<axis.x()<<" "<<axis.y()<<" "<<axis.z();
//    qDebug()<<"rotation :"<<rotation.x()<<" "<<rotation.y()<<" "<<rotation.z();
//    qDebug()<<"rotation :"<<rotation.x()<<" "<<rotation.y()<<" "<<rotation.z();
//    qDebug()<<"angle :"<<angle ;
//    for (int i=0;i<NUMBER_OF_ACTIONS;i++){
//        if(_actions[i]==true){
//            switch (i){
//            case left:
//                _camera.move(Point3df(.1,0,0));
//                break;
//            case right:
//                _camera.move(Point3df(-.1,0,0));
//                break;
//            case up:
//                _camera.move(Point3df(0,-.1,0));
//                break;
//            case down:
//                _camera.move(Point3df(0,.1,0));
//                break;
//            case forward:
//                _camera.move(Point3df(0,0,-.1));

//                axis = _camera.get_rotation().rotatedVector(QVector3D(0,0,-1)/10.0f);
//                qDebug()<<"axis :"<<axis.x()<<" "<<axis.y()<<" "<<axis.z();
//                break;
//            case backward:
//                _camera.move(Point3df(0,0,.1));
//                break;
//            case ru:
//                q = QQuaternion::fromAxisAndAngle(QVector3D(1,0,0),1);
//                _camera.set_rotation( _camera.get_rotation()* q);
//                break;
//            case rd:
//                q = QQuaternion::fromAxisAndAngle(QVector3D(1,0,0),1);
//                _camera.set_rotation( q*_camera.get_rotation());
//                break;
//            }
//        }
//    }
    if (_progressive_zoom._activated){
        if (_progressive_zoom._targeted != _zoom_level) {
            if (absolute_value( _zoom_level - _progressive_zoom._targeted ) <= _progressive_zoom._speed) _zoom_level = _progressive_zoom._targeted;
            else if (_zoom_level < _progressive_zoom._targeted) _zoom_level += _progressive_zoom._speed;
            else if (_zoom_level > _progressive_zoom._targeted) _zoom_level -= _progressive_zoom._speed;
        }
    }
}


bool UIState::hasActionGoing (int action){
    return _actions[action];
}
