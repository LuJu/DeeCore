#include "inputmanager.h"

const int InputManager::_default_keyboard_matching[NUMBER_OF_KEYS]={
    Qt::Key_Left,
    Qt::Key_Right,
    Qt::Key_Up,
    Qt::Key_Down,

    Qt::Key_PageUp,
    Qt::Key_PageDown,

    Qt::Key_0,
    Qt::Key_1,
    Qt::Key_2,

    Qt::Key_E,

    Qt::Key_Eacute,
    Qt::Key_U,
    Qt::Key_A,
    Qt::Key_I
};

InputManager::InputManager()
{
    _keyboard_activated=true;
    _mouse_activated=true;
    _mouse_left_dragging=false;
    resetAssignment();
}


void InputManager::keyboardEvent(int key, int eventType)
{
    if(_keyboard_activated){
        int action=-1;

        switch(key)
        {
        case Qt::Key_Left:
        case Qt::Key_Right:
        case Qt::Key_Up:
        case Qt::Key_Down:
        case Qt::Key_PageUp:
        case Qt::Key_PageDown:
        case Qt::Key_0:
        case Qt::Key_1:
        case Qt::Key_2:
        case Qt::Key_A:
        case Qt::Key_E:
        case Qt::Key_U:
        case Qt::Key_I:
        case Qt::Key_Eacute:
            action = get_key_action(key);
            if(action!=-1){ //Means that the action is recorded and has an effect
                if(eventType==Key_Pressed){
                    _keys_activated[action]=true;
                    _ui->set_action(action,true);
                }
                else if(eventType==Key_Released){
                    _keys_activated[action]=false;
                    _ui->set_action(action,false);
                }
            }
            break;
        case Qt::Key_B:
            GlobalConfig::switchState("display_bezier");
            break;
        case Qt::Key_L:
            GlobalConfig::switchState("display_light");
            break;
        case Qt::Key_V:
            if(eventType==Key_Pressed)
                GlobalConfig::switchState("display_second_viewport");
            break;
        case Qt::Key_Space:
            if (_ui->is_paused()) _ui->set_paused(false);
            else _ui->set_paused(true);

        }
    }
}

void InputManager::mouseEvent( int eventType, QPoint pos, int key)
{
    if (eventType==Key_Pressed)       qDebug()<<"mouse key pressed  : "<<key;
    else if (eventType==Key_Released) qDebug()<<"mouse key released : "<<key;
    _ui->set_mouse_position(pos);

    switch (eventType){
    case Key_Pressed:
        switch (key){
        case Qt::LeftButton:
            break;
        case Qt::RightButton:
            break;
        }
        break;
    case Moved:
        switch (_ui->get_camera().get_projection_type()) {
        case Camera::two_dimensions:
            _ui->get_camera().move(Point3df(-pos.x(),pos.y(),0));
            break;
        case Camera::orthographic:
        case Camera::perspective:
            _ui->rotate(pos);
            break;
        default:
            break;
        }
        break;
    default:
        break;
    }
}

void InputManager::wheelEvent(int orientation, int delta, QPoint pos)
{

    if (orientation==Qt::Vertical){
        _ui->changeZoom(delta/5);
    } else if (orientation==Qt::Horizontal){
    }
//        if(abs(delta)>=100)
////            _ui->requireLevelChange((delta>0)?1:-1);
////        _ui->set_z(_ui->get_z()+delta/15);
//    }
    _ui->set_mouse_position(pos);
    QString orient;
//    if (orientation==Qt::Horizontal) orient="horizontal";
//    if (orientation==Qt::Vertical)   orient="vertical  ";
//    qDebug()<<"wheel orientation : "<<orient<<"; delta: "<<delta;

}

int InputManager::get_key_action(int key){
    for (int i = 0; i < NUMBER_OF_KEYS; ++i) {
        if(_keyboard_matching[i]==key) return i;
    }
    return -1; //not found
}

bool InputManager::changeAssignment(int old_key_id, int new_key){
    _keyboard_matching[old_key_id] = new_key;
    return true;
}

void InputManager::resetAssignment(){
    for (int i = 0; i < NUMBER_OF_KEYS; ++i) {
        _keyboard_matching[i]=_default_keyboard_matching[i];
        _keys_activated[i]=false;
    }
}
