#include "GLViewport.h"
#include "Camera3D.h"
#include "cloth.h"

GLViewPort::GLViewPort(QWidget *parent)
    : QGLWidget(parent)
{
    d_camera = new Camera3D(vector3f(10.0f, 10.0f, 0.0f),
                            vector3f(0.0f, 0.0f, 0.0f),
                            vector3f(0.0f, 1.0f, 0.0f),
                            vector3f(0.0f, 1.0f, 0.0f));
    d_bDrawSpring = false;
    d_bDrawCloth = true;
    d_bDrawParticles = false;
}

GLViewPort::~GLViewPort()
{

}

QSize GLViewPort::sizeHint() const
{
    return QSize(600, 700);
}


void GLViewPort::initializeGL()
{
    //glEnable(GL_TEXTURE_2D);
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}


// Update viewport and projection matrix
void GLViewPort::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (GLfloat) w/(GLfloat) h, 0.0, 1000.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}


void GLViewPort::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPushMatrix();
    d_camera->updateCamera();
    if(d_bDrawCloth)
        d_cloth->draw();
    if(d_bDrawSpring)
        d_cloth->drawMesh();
    if(d_bDrawParticles)
        d_cloth->drawParticles();
    glPopMatrix();
}

void GLViewPort::mousePressEvent(QMouseEvent *event)
{
    d_qMouselastPos = event->pos();
}

void GLViewPort::mouseMoveEvent(QMouseEvent *event)
{
    d_qMouseDeltaPosX = event->x() - d_qMouselastPos.x();
    d_qMouseDeltaPosY = event->y() - d_qMouselastPos.y();
    if(event->buttons() & Qt::LeftButton)
    {
        d_camera->orbitX(d_qMouseDeltaPosX*(-.005));
        d_camera->orbitY(d_qMouseDeltaPosY*.005);
    }
    if(event->buttons() & Qt::RightButton)
    {
        d_camera->panHorizontal(d_qMouseDeltaPosX*(-.01));
        d_camera->panVertical(d_qMouseDeltaPosY*.01);
    }
    if(event->buttons() & Qt::MiddleButton)
        d_camera->zoomCamera(d_qMouseDeltaPosY*(-.01));

    d_qMouselastPos = event->pos();
}


void GLViewPort::setDrawCloth(bool b)
{
    d_bDrawCloth = b;
}

void GLViewPort::setDrawParticles(bool b)
{
    d_bDrawParticles = b;
}

void GLViewPort::setDrawSprings(bool b)
{
    d_bDrawSpring = b;
}
