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
        left, right, up, down,
        forward,backward,
        action, back, option, none
    };


    //! Get the zoom value of the view
    /*!
        The zoom value goes from 0 to 1000
    */
    inline int get_zoom(){return _zoom_level;}
    Point3d<float> get_position() const {return _position;}
    void set_position(Point3df position){_position = position;}
    Point3df get_previous_position() const {return _previous_position;}
    void set_previous_position(Point3df previous_position){_previous_position = previous_position;}
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

    Point3di get_rotate() const {return _rotate;}
    void set_rotate(Point3di rotate){_rotate = rotate;}

    void updateState();
//    static UIState * instance();
    void saveState();
    void changeZoom(int delta);


    const QMatrix4x4& get_projection() {return _camera.get_projection_matrix();}
    const QMatrix4x4& get_view() {return _camera.get_view_matrix();}
    void set_view(QMatrix4x4 view){_camera.set_view_matrix(view);}
    void set_projection(QMatrix4x4 projection){_camera.set_projection_matrix(projection);}
    QMatrix4x4 _rotation;
//    QMatrix4x4 _translation;
    QQuaternion _quaternion;
    void rotate(QPoint mouse_coordinates);
    void loadPreviousState();
    UIState();
    ~UIState();
private:


//    QMatrix4x4 _projection;
//    QMatrix4x4 _view;
    UIState(const UIState&);
    // Private methods
    void actionProcess();

    // Static constants
    static const quint8 NUMBER_OF_ACTIONS=11;

    Point3df _position;
    Point3df _previous_position;
    Point3di _rotate;

    int _zoom_targeted;
    int _zoom_level;
    bool _actions[NUMBER_OF_ACTIONS];
    int _action_done;
    QPoint _mouse_pos;
    bool _paused;

    Camera _camera;
};

#endif // UISTATE_H
