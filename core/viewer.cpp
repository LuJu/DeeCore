#include "viewer.h"

using namespace std;
//using namespace qglviewer;


Viewer::Viewer(const QGLFormat &format) :
    QGLWidget(format),
    _ui(NULL),
    _input(NULL),
    _timer_fps(NULL),
    _timer_start(NULL),
    _initiated(false){
}
Viewer::Viewer(QGLContext * context) :
    QGLWidget(context),
    _ui(NULL),
    _input(NULL),
    _timer_fps(NULL),
    _timer_start(NULL),
    _initiated(false){
}
Viewer::Viewer(QWidget * parent, const QGLWidget * shareWidget, Qt::WindowFlags f) :
    QGLWidget(parent,shareWidget,f),
    _ui(NULL),
    _input(NULL),
    _timer_fps(NULL),
    _timer_start(NULL),
    _initiated(false){
}

Viewer::~Viewer(){
//    if (_main_viewer) {
//        _ui->saveState();
//        GlobalConfig::saveConfiguration();
//    }
    deleteData();
}

void Viewer::deleteData(){
    if (_initiated){
        if (_timer_fps) delete _timer_fps;
        if (_timer_start) delete _timer_start;
        if(GlobalConfig::is_enabled("shaders")){
            for (int i = 0; i < 3; ++i) {
                if (_shaders[i]) delete _shaders[i];
            }
        }
        if (_ui) delete _ui;
        if (_input) delete _input;
    }
}

void Viewer::draw()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    QMatrix4x4 V,P;
    float scale = (((float)_ui->get_zoom())/100)+1.0f;

    Camera& camera = _ui->get_camera();
    camera.set_scale(Point3df(scale,scale,scale));
    V = camera.get_view_matrix();

//    switch (camera.get_projection_type()) {
//    case Camera::orthographic:
//        P.ortho(-3,3,-1,5,-1000,1000);
//        break;
//    case Camera::perspective:
//        P.perspective(_ui->fov,4.0f/3.0f,0.1f,100.0f);
//    case Camera::two_dimensions:
//        P.ortho(-10,10,-10,10,-1,1);
//    default:
//        break;
//    }
//    camera.set_projection_matrix(P);

    updateProjection();
}

void Viewer::resizeGL(int width, int height){
//    QMatrix4x4 P;
//    P.perspective(_ui->fov,4.0f/3.0f,0.0001f,10000000.0f);
//    _ui->get_camera().set_projection_matrix(P);
//    int side = qMin(width, height);
//    glViewport(0,0, width, height);
    updateProjection();
}
void Viewer::updateProjection(){
    float window_width = width();
    float window_height = height();
    float frame_width = window_width / 100.f;
    float frame_height = window_height / 100.f;
    Camera& camera = _ui->get_camera();
    QMatrix4x4 P;
    switch (camera.get_projection_type()) {
    case Camera::orthographic:
        P.ortho(-frame_width/2,frame_width/2,1 - frame_height/2 ,1 + frame_height/2,-1000,1000);
        break;
    case Camera::perspective:
        P.perspective(_ui->fov,4.0f/3.0f,0.1f,100.0f);
    case Camera::two_dimensions:
        P.ortho(-10,10,-10,10,-1,1);
    default:
        break;
    }
    camera.set_projection_matrix(P);
}

void Viewer::startShaders(){
        _program = new QGLShaderProgram(this);

        _shaders[0] = compileShader(":/shaders/vshader.glsl",QGLShader::Vertex);
        _shaders[1] = compileShader(":/shaders/fshader.glsl",QGLShader::Fragment);
//      _shaders[2] = compileShader(":/shaders/gshader.glsl",QGLShader::Geometry);
        _shaders[2] = NULL;

        _program->addShader(_shaders[0]);
        _program->addShader(_shaders[1]);
//        _program->addShader(_shaders[2]);

        if(!_program->link()) qCritical()<<"Fail to link shader program";
        if(!_program->bind()) qCritical()<<"Fail to link shader program";
}

QGLShader * Viewer::compileShader(const char * path, QGLShader::ShaderType type){
    QGLShader * shader;
    QFile shaderf(path);
    if (shaderf.open(QIODevice::ReadOnly)){
        QByteArray qvs = shaderf.readAll();
        shaderf.close();
        shader = new QGLShader(type, this);
        shader->compileSourceCode(qvs.data());
        shaderf.close();
        return shader;
    } else {
        qCritical()<<"Error opening shader";
        return NULL;
    }
}

void Viewer::display3DObjects(){
    for(int i = 0; i< _3D_display_list.size(); i++){
        QMatrix4x4 M,V,P;
        V = _ui->get_camera().get_view_matrix();
        P = _ui->get_camera().get_projection_matrix();
        M = _3D_display_list[i]->get_matrix();
        updateMatrices(P,V,M);
        _3D_display_list[i]->render();
    }
}

void Viewer::bindProgram(){
    if (GlobalConfig::is_enabled("shaders_activated"))
        bindProgram();
}

void Viewer::releaseProgram(){
    if (GlobalConfig::is_enabled("shaders_activated"))
        releaseProgram();

}

void Viewer::updateModelMatrix(const QMatrix4x4 M){
    if (GlobalConfig::is_enabled("shaders")){
        const QMatrix4x4& V=_ui->get_camera().get_view_matrix();
        const QMatrix4x4& P=_ui->get_camera().get_projection_matrix();
        _model_matrix = M;
        _program->setUniformValue("M",M);
        _program->setUniformValue("pvm",P*V*M);
    }
}
void Viewer::updateProjectionMatrix(const QMatrix4x4 P){
    if (GlobalConfig::is_enabled("shaders")){
        const QMatrix4x4& V=_ui->get_camera().get_view_matrix();
        _ui->get_camera().set_projection_matrix(P);
        _program->setUniformValue("P",P);
        _program->setUniformValue("pvm",P*V*_model_matrix);
    }
}
void Viewer::updateViewMatrix(const QMatrix4x4 V){
    if (GlobalConfig::is_enabled("shaders")){
        const QMatrix4x4& P=_ui->get_camera().get_projection_matrix();
        _ui->get_camera().set_view_matrix(V);
        _program->setUniformValue("V",V);
        _program->setUniformValue("pvm",P*V*_model_matrix);
    }
}
//void Viewer::updateViewMatrix(){

//}
//void Viewer::updateProjectionMatrix(){
//    if (GlobalConfig::is_enabled("shaders")){
//        _program->setUniformValue("M",M);
//        _program->setUniformValue("V",V);
//        _program->setUniformValue("P",P);
//        _program->setUniformValue("pvm",P*V*M);
//    }
//}

void Viewer::updateMatrices(const QMatrix4x4& P,const QMatrix4x4& V,const QMatrix4x4& M){
    if (GlobalConfig::is_enabled("shaders")){
        _ui->get_camera().set_view_matrix(V);
        _ui->get_camera().set_projection_matrix(P);
        _model_matrix = M;
        _program->setUniformValue("M",M);
        _program->setUniformValue("V",V);
        _program->setUniformValue("P",P);
        _program->setUniformValue("pvm",P*V*M);
    } else {
        GLfloat matrix[16];
#ifdef QT_4_
        const qreal* data = (P).transposed().data();
#else
        const float* data = (P).transposed().data();
#endif
        for (int i = 0; i < 16; ++i) {
            matrix[i] = data[i];
        }
        debugDataGL(matrix);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glMultMatrixf(matrix);
        GLfloat m[16];
        glGetFloatv (GL_PROJECTION_MATRIX, m);
        glGetFloatv (GL_PROJECTION_MATRIX, m);
#ifdef QT_4_
        const qreal* data2= (M*V).data();
#else
        const float* data2= (M*V).data();
#endif
        for (int i = 0; i < 16; ++i) {
            matrix[i] = data2[i];
        }
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
    }
}

void Viewer::debugData(const qreal* data){
    for (int i = 0; i < 4; ++i) {
        qDebug()<<data[i*4]<<" "<<data[i*4 +1]<<" "<<data[i*4 +2]<<" "<<data[i*4 +3]<<" ";
    }
}
void Viewer::debugDataGL(const GLfloat* data){
    for (int i = 0; i < 4; ++i) {
        qDebug()<<data[i*4]<<" "<<data[i*4 +1]<<" "<<data[i*4 +2]<<" "<<data[i*4 +3]<<" ";
    }
}

void Viewer::displayFullTextured(int x, int y, int width, int height){
    glBegin(GL_QUADS); {
//        glTexCoord2f(0,0);
//        glVertex2d(startposition+_background_position,            height);
//        glTexCoord2f(1,0);
//        glVertex2d(startposition+_background_position+tex_size[0],height);
//        glTexCoord2f(1,1);
//        glVertex2d(startposition+_background_position+tex_size[0],0          );
//        glTexCoord2f(0,1);
//        glVertex2d(startposition+_background_position,            0          );
    } glEnd();
}

void Viewer::display2D(){
//    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
//    startScreenCoordinatesSystem();
//    camera()->getViewport(_viewport);
    glGetIntegerv( GL_VIEWPORT, _viewport );

    float height= _viewport[1];
    float width = _viewport[2];

//    glViewport(0,0,10,10);
    QMatrix4x4 proj;
//    glViewport(0,0,width,height);
    proj.ortho(0,width,0,height,0,0);
//    _program->setUniformValue("P",proj);
    QMatrix4x4 pvm = proj;
//    _program->setUniformValue("pvm",pvm);
    releaseProgram();
//    if(_background_activated) {
//        GLint tex_size[0];
//        glBindTexture(GL_TEXTURE_2D, _textures[0]);
//        glGetTexLevelParameteriv(GL_TEXTURE_2D,0,GL_TEXTURE_WIDTH,&tex_size[0]);
//        glGetTexLevelParameteriv(GL_TEXTURE_2D,0,GL_TEXTURE_HEIGHT,&tex_size[1]);
//        glBegin(GL_QUADS);{
//            glTexCoord2d(0,0);
//            glVertex2d(0,0);
//            glVertex2d(width,0);
//            glTexCoord2d(1,0);
//            glVertex2d(width,height);
//            glTexCoord2d(1,1);
//            glVertex2d(0,height);
//            glTexCoord2d(1,0);
//        } glEnd();


//            Mesh3d me
//            glBegin(GL_QUADS); {
//                int startposition = 0;
//                int i=0;
//                do {
//                    startposition=(i-1)*tex_size[0];
//                    glTexCoord2f(0,0);
//                    glVertex2d(startposition+_background_position,            height);
//                    glTexCoord2f(1,0);
//                    glVertex2d(startposition+_background_position+tex_size[0],height);
//                    glTexCoord2f(1,1);
//                    glVertex2d(startposition+_background_position+tex_size[0],0          );
//                    glTexCoord2f(0,1);
//                    glVertex2d(startposition+_background_position,            0          );
//                    i++;
//                } while(startposition<width);
//            } glEnd();
//            _background_position+=1;
//            if(_background_position>tex_size[0]){
//              _background_position=0;
//            }
//    }
//    stopScreenCoordinatesSystem();
    bindProgram();
    glEnable(GL_DEPTH_TEST);
//    glEnable(GL_LIGHTING);
}

void Viewer::init()
{
    deleteData();
    setCursor(Qt::OpenHandCursor);
    _input = new InputManager();
    _ui = new UIState();
    _input->set_ui(_ui);

    glClearColor(0.2,0.2,0.2,1);
    if(GlobalConfig::is_enabled("shaders"))
        startShaders();
    _timer_fps= new QTimer();
    _timer_fps->setInterval(1000);
    _timer_fps->connect(_timer_fps, SIGNAL(timeout()),this, SLOT(framepersecond()));
    _timer_fps->start();
    _frame=0;
    _current_fps = 0;
    _background_activated=false;

    _timer_start = new QTimer(this);
    connect(_timer_start, SIGNAL(timeout()), this, SLOT(animate()));
    _timer_start->start(Timing::fps_delta_time);

    _time.start();
    QMatrix4x4 P;
    P.ortho(-1,1,-1,1,-1,1);
    P.perspective(90,4/3,.1,1);
    _ui->get_camera().set_projection_matrix(P);
//    _program->setUniformValue("P",P);
    _initiated = true;
}

void Viewer::animate(){
    updateGL();
}

void Viewer::frameEnd(){
    Debugger::promptOpenGLError();
    _frame++;
    Timing::time_since_last_frame=_time.elapsed()-Timing::time_since_last_frame;
    _ui->updateState();
}

void Viewer::framepersecond()
{
    if(GlobalConfig::is_enabled("output_fps"))
        qDebug()<<"FPS : "<<_frame;
    _current_fps = _frame;
    _frame=0;
}

void Viewer::addTo3DDisplayList(GraphicObject3D * object){
    _3D_display_list.append(object);
}

void Viewer::mousePressEvent(QMouseEvent * event)
{
    QPoint position = event->pos();
    setCursor(Qt::ClosedHandCursor);
    updateLastMousePosition(event->pos());
}

void Viewer::mouseReleaseEvent(QMouseEvent * event){
    setCursor(Qt::OpenHandCursor);

}

void Viewer::mouseMoveEvent(QMouseEvent * event){
    QPoint diff = event->pos()-_last_position;
    updateLastMousePosition(event->pos());
    _input->mouseEvent(InputManager::Moved,diff);
}

void Viewer::wheelEvent(QWheelEvent * event)
{
    _input->wheelEvent(event->orientation(),event->delta(),event->pos());
}

void Viewer::keyPressEvent(QKeyEvent *keyEvent)
{
    if (keyEvent->key() ==  Qt::Key_Escape)
        close();
    _input->keyboardEvent(keyEvent->key(),InputManager::Key_Pressed);
}

void Viewer::keyReleaseEvent(QKeyEvent *keyEvent)
{
    if (keyEvent->key() ==  Qt::Key_Escape)
        close();
    _input->keyboardEvent(keyEvent->key(),InputManager::Key_Released);
}

void Viewer::closeEvent(QCloseEvent * event){


//    QGLViewer::closeEvent(event);
}

