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

};

#endif // CAMERA_H
