#include <QtGui>
#include <QtOpenGL>
#include <math.h>

#include <stdlib.h>
#include <time.h>

#include "glwidget.h"
#include "kdtree.h"

#define MAX_ZOOM 3.0
#define MIN_ZOOM 0.1

GLWModel *GLWidget::a = NULL;
GLWModel *GLWidget::b = NULL;

int       GLWidget::numObst = 0;
glPoly  **GLWidget::arrObst = NULL;

int       GLWidget::numPoints = 0;

//pair<GLfloat, GLfloat>* GLWidget::arrNodes = NULL;
kdnode** GLWidget::arrNodes = NULL;

vector<int>** GLWidget::g = NULL;

bool* GLWidget::gVisited = NULL;
int*  GLWidget::gParents = NULL;

kdnode* GLWidget::ini = NULL;
kdnode* GLWidget::fin = NULL;


GLboolean GLWidget::showAux = false;

void GLWidget::StartAnimation() {
    timer->start(40);
}

void GLWidget::StopAnimation() {
    timer->stop();
}

void GLWidget::RefreshAnimation() {
    updateGL();
}

void GLWidget::ResetAnimation() {
//    timer->stop();

//    if(arrObst != NULL) delete[] arrObst;

//    a->place(-1.8, 0.0, -1.8);
//    a->setYinc(0);

//    b->place(1.8, 0, 1.8);
//    b->setYinc(0);
//    updateGL();
}

void trace(int v) {
    //if(v == 0) return;
    if(GLWidget::arrNodes[v] == GLWidget::ini) return;

    trace(GLWidget::gParents[v]);

    kdnode *n = GLWidget::arrNodes[v];
    glColor3f(n->p.first, 0.0, n->p.second);
}

GLWidget::GLWidget(QWidget *parent) : QGLWidget(QGLFormat(QGL::SampleBuffers), parent) {
    cout << QDir::currentPath().append("/../data/tief.obj").toAscii().data() << endl;

    a = new GLWModel(QDir::currentPath().append("/../data/tief.obj").toAscii().data());
    b = new GLWModel(QDir::currentPath().append("/../data/death2.obj").toAscii().data());

    view = GLWidget::PERSPECTIVE;
    factorZ = MAX_ZOOM;

    a->place(-1.8, 0.0, -1.8);
    a->setYinc(0);

    b->place(1.8, 0, 1.8);
    b->setYinc(0);

    timer = new QTimer(this);
    timer->start(40);
    connect(timer, SIGNAL(timeout()), this, SLOT(timeOut()));
    timer->stop();
}

GLWidget::~GLWidget(){
}

void GLWidget::setView(enum GLWidget::ViewType type) {
    if(type == GLWidget::TOPVIEW) {
        this->view = GLWidget::TOPVIEW;
    } else if(type == GLWidget::SPINNING) {
        this->view = GLWidget::SPINNING;
    } else {
        this->view = GLWidget::PERSPECTIVE;
    }
}

void GLWidget::setZoom(GLfloat factorZ) {
    this->factorZ = (factorZ<MIN_ZOOM?MIN_ZOOM: (factorZ>MAX_ZOOM?MAX_ZOOM:factorZ));
    updateGL();
}

void GLWidget::setXRotation(GLint angle) {
    if (angle != xRot) {
        xRot = angle;
        updateGL();
    }
}

void GLWidget::setYRotation(GLint angle) {
    if (angle != yRot) {
        yRot = angle;
        updateGL();
    }
}

void GLWidget::setZRotation(GLint angle) {
    if (angle != zRot) {
        zRot = angle;
        updateGL();
    }
}

void GLWidget::initializeGL() {
    // OpenGL setup
    glShadeModel(GL_SMOOTH);
    glClearColor(0, 0, 0, 0.0);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_NORMALIZE);

    // change to model view for drawing
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // clear framebuffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // set up lighting
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    //glEnable(GL_LIGHT1);

    GLfloat lightPos1[4] = { 1, 5.0, 7.0, 1.0 };
    //GLfloat lightPos2[4] = { 0.75, 0.75, 5.75, 1.0 };
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos1);
    //glLightfv(GL_LIGHT1, GL_POSITION, lightPos2);
}

void GLWidget::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    switch (this->view) {
        case GLWidget::TOPVIEW: {
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();

            GLfloat w = this->width();
            GLfloat h = this->height();
            GLfloat r = (GLfloat)w/(GLfloat)h;

            glOrtho(-factorZ*r, factorZ*r, -factorZ, factorZ, 0, 10);

            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            gluLookAt(0.00001, 0.8, 0.0,
                      0.0, 0.0, 0.0,
                      0.0, 0.0, 2.0);

            break;
        }

        case GLWidget::SPINNING: {
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();

            glTranslatef(0.0, 0.0, -10.0);

            glRotatef(xRot / 16.0, 1.0, 0.0, 0.0);
            glRotatef(yRot / 16.0, 0.0, 1.0, 0.0);
            glRotatef(zRot / 16.0, 0.0, 0.0, 1.0);

            break;
        }


        case GLWidget::PERSPECTIVE: {
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();

            GLfloat w = this->width();
            GLfloat h = this->height();
            GLfloat r = (GLfloat)w/(GLfloat)h;

            gluPerspective(30.0, r, 1.0, 10.0);

            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();

            GLfloat x0 = 0.0 + a->x();
            GLfloat y0 = 0.4 + a->y();
            GLfloat z0 = -1.9 + a->z();

            GLfloat x1 = x0 + 0;
            GLfloat y1 = y0 + 0.1;
            GLfloat z1 = z0 + 10;

            gluLookAt(x0, y0, z0,
                      x1, y1, z1,
                      0.0, 2.0, 0.0);

            glRotatef(xRot / 16.0, 1.0, 0.0, 0.0);
            glRotatef(yRot / 16.0, 0.0, 1.0, 0.0);
            glRotatef(zRot / 16.0, 0.0, 0.0, 1.0);

            break;
        }

        default: {
            break;
        }
    }

    glColor3f(0.8, 0.6, 0.2);
    a->draw();
    if(GLWidget::showAux) a->drawCircle();

    glColor3f(0.6, 0.2, 0.8);
    b->draw();
    if(GLWidget::showAux) b->drawCircle();

    glColor3f(0.2, 0.8, 0.6);
    glutWireCube(4);

    glColor3f(0.8, 0.8, 0.9);
    for(int i=0; i<numObst; ++i) {
        arrObst[i]->draw();
    }

    if(GLWidget::showAux) {
        glColor3f(0.9, 0.4, 0.4);
        for(int i=0; i<numObst; ++i) {
            arrObst[i]->drawOutline();
        }
    }

    //kdnode *fin = arbolB->search(make_pair(1.8, 1.8));

    glPointSize(4.0);
    glColor3f(1, 1, 1);
    glBegin(GL_POINTS);
    int tt = GLWidget::numPoints*3;

    for(int i=0; i<tt; ++i) {
        kdnode *na = GLWidget::arrNodes[i];
        if(na == NULL) continue;
        glVertex3f(na->p.first, 0, na->p.second);
    }
    glEnd();
    glPointSize(1.0);

    glLineWidth(2.0);
    glColor3f(1, 1, 1);
    glBegin(GL_LINES);


    for(int i=0; i<tt; ++i) {
        int k = GLWidget::g[i]->size();

        kdnode *na = GLWidget::arrNodes[i];
        if(na == NULL) continue;

        glVertex3f(na->p.first, 0, na->p.second);
        for(int j=0; j<k; ++j) {
            kdnode *nb = GLWidget::arrNodes[j];
            if(nb == NULL) continue;
            glVertex3f(nb->p.first, 0, nb->p.first);
        }
    }
    glEnd();
    glLineWidth(1.0);

    swapBuffers();
}

void GLWidget::resizeGL(GLint width, GLint height) {
    // set up for perspective drawing
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    if (width < 2) { width = 2; }
    if (height < 2) { height = 2; }

    glViewport(0, 0, width, height);
    gluPerspective(40.0, (GLfloat)width/(GLfloat)height, 0.1, 1000.0);

    // change to model view for drawing
    glMatrixMode(GL_MODELVIEW);

}

void GLWidget::wheelEvent(QWheelEvent *event) {
    switch (this->view) {
        case GLWidget::TOPVIEW: {
            int numDegrees = event->delta() / 8;
            int numSteps = numDegrees / 15;

            setZoom(this->factorZ - numSteps / 10.0);

            event->accept();
            break;
        }

        default: {
            break;
        }

    }
}

void GLWidget::timeOut() {
    GLfloat ra = a->radio();
    GLfloat rb = b->radio();
    GLfloat universo = 1.6;

    universo -= ra;
    if(fabs(a->x()) > universo) a->changeXdir();
    if(fabs(a->y()) > universo) a->changeYdir();
    if(fabs(a->z()) > universo) a->changeZdir();
    universo += ra;

    universo -= rb;
    if(fabs(b->x()) > universo) b->changeXdir();
    if(fabs(b->y()) > universo) b->changeYdir();
    if(fabs(b->z()) > universo) b->changeZdir();
    universo += rb;

    a->animate();
    b->animate();

    updateGL();
}


void GLWidget::mousePressEvent(QMouseEvent *event) {
    lastPos = event->pos();
}

void GLWidget::mouseMoveEvent(QMouseEvent *event) {
    GLint dx = event->x() - lastPos.x();
    GLint dy = event->y() - lastPos.y();

    if (event->buttons() & Qt::LeftButton) {
        setXRotation(xRot + 8 * dy);
        setYRotation(yRot + 8 * dx);
    } else if (event->buttons() & Qt::RightButton) {
        setXRotation(xRot + 8 * dy);
        setZRotation(zRot + 8 * dx);
    }

    lastPos = event->pos();
}
