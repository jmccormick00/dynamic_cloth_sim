#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>


class QTimer;
class C_Cloth;
class Button;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void createViewPort();
    QSize sizeHint() const;


private:
    Button *createButton(const QString &text, const char *member);

    unsigned short d_numViewPorts;
    QTimer *d_qSimTimer;
    QTimer *d_qDrawTimer;
    C_Cloth* d_cloth;

public slots:
    void startSim();
    void stopSim();
    void initializeSim();
    void updateSim();
    void drawViewPorts();

signals:
    void updateViewPorts();
};

#endif // MAINWINDOW_H
