#include "mainwindow.h"
#include "GLViewport.h"
#include "cloth.h"
#include "button.h"
#include <QString>
#include <QDockWidget>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), d_numViewPorts(0)
{
    d_qSimTimer = new QTimer(this);
    connect(d_qSimTimer, SIGNAL(timeout()), this, SLOT(updateSim()));

    d_qDrawTimer = new QTimer(this);
    connect(d_qDrawTimer, SIGNAL(timeout()), this, SLOT(drawViewPorts()));
    d_qDrawTimer->start(30);

    QDockWidget *dock = new QDockWidget("Simulation Commands", this);
    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->setSizeConstraint(QLayout::SetFixedSize);

    // Create the simulation control buttons
    Button *startButton = createButton(tr("Start"), SLOT(startSim()));
    Button *stopButton = createButton(tr("Stop"), SLOT(stopSim()));
    Button *initButton = createButton(tr("Initialize"), SLOT(initializeSim()));

    QGroupBox *controlGroupBox = new QGroupBox(tr("Simulation Control"));
    QVBoxLayout *vControlBox = new QVBoxLayout;
    vControlBox->addWidget(startButton);
    vControlBox->addWidget(stopButton);
    vControlBox->addWidget(initButton);
    vControlBox->addStretch(1);
    controlGroupBox->setLayout(vControlBox);
    mainLayout->addWidget(controlGroupBox, 0, 0);

    // Add other buttons here

    // Add the mainlayout to the dock widget
    QWidget *wi = new QWidget;
    wi->setLayout(mainLayout);
    dock->setWidget(wi);
    addDockWidget(Qt::LeftDockWidgetArea, dock);

    setWindowTitle("Cloth Simulation");

    d_cloth = new C_Cloth();
    d_cloth->initialize(10.0f, 10.0f, 30, 30, 200, 550.0, 400.0, 0.005, ZAXIS);
    d_cloth->lockParticle(0, 0);
    d_cloth->lockParticle(0, 19);
    d_cloth->setGravity(vector3f(0.0, -32.0, 0));
}

MainWindow::~MainWindow()
{
    delete d_cloth;
}

QSize MainWindow::sizeHint() const
{
    return QSize(1000, 800);
}


void MainWindow::createViewPort()
{
    QString vpTitle = QString("ViewPort %1").arg(++d_numViewPorts);
    QDockWidget *dock = new QDockWidget(vpTitle, this);

    // The main layout for the dock
    QVBoxLayout *mainLayout = new QVBoxLayout;

    GLViewPort *glView = new GLViewPort(this);
    glView->setCloth(d_cloth);

    // Add the viewport on top
    mainLayout->addWidget(glView);

    // Set up the render options box
    QGroupBox *renderGroupBox = new QGroupBox(tr("Render Options"));
    renderGroupBox->setChecked(false);
    QCheckBox *checkBox1 = new QCheckBox(tr("Cloth"));
    QCheckBox *checkBox2 = new QCheckBox(tr("Springs"));
    QCheckBox *checkBox3 = new QCheckBox(tr("Particles"));
    connect(checkBox1, SIGNAL(toggled(bool)), glView, SLOT(setDrawCloth(bool)));
    connect(checkBox2, SIGNAL(toggled(bool)), glView, SLOT(setDrawSprings(bool)));
    connect(checkBox3, SIGNAL(toggled(bool)), glView, SLOT(setDrawParticles(bool)));

    QHBoxLayout *hbox = new QHBoxLayout;
    hbox->addWidget(checkBox1);
    hbox->addWidget(checkBox2);
    hbox->addWidget(checkBox3);
    checkBox1->setChecked(true);
    hbox->addStretch(1);
    renderGroupBox->setLayout(hbox);
    mainLayout->addWidget(renderGroupBox);

    QWidget *wi = new QWidget;
    wi->setLayout(mainLayout);
    dock->setWidget(wi);

    if(d_numViewPorts -1 == 0)
        setCentralWidget(dock);
    else
        addDockWidget(Qt::RightDockWidgetArea, dock);

    connect(this, SIGNAL(updateViewPorts()), glView, SLOT(updateGL()));
}

Button* MainWindow::createButton(const QString &text, const char *member)
{
    Button *button = new Button(text);
    connect(button, SIGNAL(clicked()), this, member);
    return button;
}


void MainWindow::startSim()
{
    d_qSimTimer->start(5);
}

void MainWindow::stopSim()
{
    d_qSimTimer->stop();
}

void MainWindow::updateSim()
{
    d_cloth->stepSimulation(0.005f);
}

void MainWindow::initializeSim()
{
    d_qSimTimer->stop();
    d_cloth->initialize(10.0f, 10.0f, 60, 60, 50, 550.0, 400.0, 0.0005, ZAXIS);
    d_cloth->lockParticle(0, 0);
    d_cloth->lockParticle(0, 59);
    d_cloth->lockParticle(59, 0);
    d_cloth->lockParticle(59, 59);
}

void MainWindow::drawViewPorts()
{
    emit updateViewPorts();
}
