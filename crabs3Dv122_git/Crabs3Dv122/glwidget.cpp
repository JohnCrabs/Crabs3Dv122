/****************/
/* glwidget.cpp */
/****************/

#include "glwidget.h"


GLWidget::GLWidget(QWidget *parent) : QGLWidget(parent)
{
    //Update Window every 30ms
    connect(&GLtimer, SIGNAL(timeout()), this, SLOT(updateGL()));
    GLtimer.start(30);
}

/* Same as init(void) */
void GLWidget::initializeGL(void) {
    mySystem.init();
}

/* Same as display(void) */
void GLWidget::paintGL(void) {
    mySystem.display();
}

/* Same as reshape(int w, int h) */
void GLWidget::resizeGL(int w, int h) {
    mySystem.reshape(w, h);
}
