#include "viewer.h"

using namespace std;
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
    deleteData();
}

void Viewer::deleteData(){
    if (_initiated){
        if (_timer_fps) delete _timer_fps;
        if (_timer_start) delete _timer_start;
        for (int i = 0; i < 3; ++i) {
            if (_shaders[i]) delete _shaders[i];
        }
        if (_ui) delete _ui;
        if (_input) delete _input;
    }
}

void Viewer::draw()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
}

void Viewer::resizeGL(int width, int height){
    float window_width = width;
    float window_height = height;
    float frame_width = window_width / 100.f;
    float frame_height = window_height / 100.f;
    float aspect_ratio = window_width / window_height;
    Camera& camera = _ui->get_camera();
    glViewport(0,0,width,height);
    QMatrix4x4 P;
    switch (camera.get_projection_type()) {
    case Camera::orthographic:
        P.ortho(-frame_width/2,frame_width/2,1 - frame_height/2 ,1 + frame_height/2,-1000,1000);
//        P.ortho(-100,100,-100,100,-1000,1000);
        break;
    case Camera::perspective:
        P.perspective(18.0f,aspect_ratio,.1f,10000.0f);
    case Camera::two_dimensions:
        P.ortho(-10,10,-10,10,-1,1);
    default:
        break;

    }
    camera.set_projection_matrix(P);
//    updateProjectionMatrix(P);
}
void Viewer::updateProjection(){
//    float window_width = width();
//    float window_height = height();
//    float frame_width = window_width / 100.f;
//    float frame_height = window_height / 100.f;
//    Camera& camera = _ui->get_camera();
//    QMatrix4x4 P;
//    switch (camera.get_projection_type()) {
//    case Camera::orthographic:
////        P.ortho(-frame_width/2,frame_width/2,-frame_height/2 ,frame_height/2,-1000,1000);
//        P.ortho(-10,10,-10,10,-1000,1000);
//        break;
//    case Camera::perspective:
//        P.perspective(75.0f,4.0f/3.0f,0.1f,10.0f);
//    case Camera::two_dimensions:
//        P.ortho(-10,10,-10,10,-1,1);
//    default:
//        break;
//    }
//    camera.set_projection_matrix(P);

//    updateProjectionMatrix(P);
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
    QMatrix4x4 M,V,P;
    V = _ui->get_camera().get_view_matrix();
    P = _ui->get_camera().get_projection_matrix();
    updateMatrices(P,V,M);
    for(int i = 0; i< _3D_display_list.size(); i++){
        M = _3D_display_list[i]->get_matrix();
        updateModelMatrix(M);
//        updateMatrices(P,V,M);
        _3D_display_list[i]->render();
    }
}

void Viewer::bindProgram(){
    _program->bind();
}

void Viewer::releaseProgram(){
    _program->release();
}

void Viewer::updateModelMatrix(const QMatrix4x4 M){
    const QMatrix4x4& V=_ui->get_camera().get_view_matrix();
    const QMatrix4x4& P=_ui->get_camera().get_projection_matrix();
    _model_matrix = M;
    _program->setUniformValue("M",M);
    _program->setUniformValue("pvm",P*V*M);
}
void Viewer::updateProjectionMatrix(const QMatrix4x4 P){
    const QMatrix4x4& V=_ui->get_camera().get_view_matrix();
    _ui->get_camera().set_projection_matrix(P);
    _program->setUniformValue("P",P);
    _program->setUniformValue("pvm",P*V*_model_matrix);
//    QVector3D position(_ui->get_camera().get_position().x(),
//                       _ui->get_camera().get_position().y(),
//                       _ui->get_camera().get_position().z());
//    _program->setUniformValue("_camera_position",position);
}
void Viewer::updateViewMatrix(const QMatrix4x4 V){
    const QMatrix4x4& P=_ui->get_camera().get_projection_matrix();
    _ui->get_camera().set_view_matrix(V);
    _program->setUniformValue("V",V);
    _program->setUniformValue("pvm",P*V*_model_matrix);
    QVector3D position(_ui->get_camera().get_position().x(),
                       _ui->get_camera().get_position().y(),
                       _ui->get_camera().get_position().z());
    _program->setUniformValue("_camera_position",position);
}

void Viewer::updateMatrices(const QMatrix4x4& P,const QMatrix4x4& V,const QMatrix4x4& M){
    _ui->get_camera().set_view_matrix(V);
    _ui->get_camera().set_projection_matrix(P);
    _model_matrix = M;
    _program->setUniformValue("M",M);
    _program->setUniformValue("V",V);
    _program->setUniformValue("P",P);
    _program->setUniformValue("pvm",P*V*M);
    QVector3D position(_ui->get_camera().get_position().x(),
                       _ui->get_camera().get_position().y(),
                       _ui->get_camera().get_position().z());
    _program->setUniformValue("_camera_position",position);
//    QVector3D direction = _ui->get_camera().get_looking_direction();
    QVector3D direction(_ui->get_camera().get_looking_direction().x(),
                       _ui->get_camera().get_looking_direction().y(),
                       _ui->get_camera().get_looking_direction().z());
    _program->setUniformValue("_camera_direction",direction);
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
    startShaders();
    _timer_fps= new QTimer();
    _timer_fps->setInterval(1000);
    _timer_fps->connect(_timer_fps, SIGNAL(timeout()),this, SLOT(framepersecond()));
    _timer_fps->start();
    _frame=0;
    _current_fps = 0;
    _background_activated=false;
//    resize(1024,768);

    _timer_start = new QTimer(this);
    connect(_timer_start, SIGNAL(timeout()), this, SLOT(animate()));
    _timer_start->start(Timing::fps_delta_time);

    _ui->get_camera().set_scale(Point3df(1,1,1));

    _time.start();
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
}

