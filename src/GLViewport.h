#ifndef GLVIEWPORT_H
#define GLVIEWPORT_H

#include <QtGui>
#include <QGLWidget>

class Camera3D;
class C_Cloth;

class GLViewPort : public QGLWidget
{
    Q_OBJECT

public:
    GLViewPort(QWidget *parent);
    ~GLViewPort();

    void setCloth(C_Cloth* c) { d_cloth = c; }
    QSize sizeHint() const;

    // Define the Scene in which the display will show
    //void setScene();

public slots:
    void setDrawParticles(bool b);
    void setDrawCloth(bool b);
    void setDrawSprings(bool b);

signals:
    void clicked();

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    //void mouseReleaseEvent(QMouseEvent *event);

private:

    bool d_bDrawSpring, d_bDrawCloth, d_bDrawParticles;
    int d_qMouseDeltaPosX, d_qMouseDeltaPosY;
    QPoint d_qMouselastPos;
    C_Cloth* d_cloth;
    Camera3D* d_camera;
};


#endif // GLVIEWPORT_H
