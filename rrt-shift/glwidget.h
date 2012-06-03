#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>

#include <GL/gl.h>
#include <GL/glut.h>

#include "glpoly.h"
#include "kdtree.h"

#include "glm.h"
#include "glwmodel.h"

class GLWidget : public QGLWidget {
    Q_OBJECT

public:
    GLWidget(QWidget *parent = 0);
    ~GLWidget();

    enum ViewType {
        PERSPECTIVE,
        SPINNING,
        TOPVIEW
    } view;

    static GLWModel *a, *b;

    static GLint numObst;
    static glPoly **arrObst;

    static GLint numPoints;
    //static pair<GLfloat, GLfloat>* arrNodes;
    static kdnode** arrNodes;

    static kdnode *ini, *fin;

    static vector<int>** g;
    static bool* gVisited;
    static int*  gParents;

    static GLboolean showAux;

    void setView(enum ViewType);

    void StartAnimation();
    void StopAnimation();
    void RefreshAnimation();
    void ResetAnimation();

public slots:
    void setXRotation(GLint angle);
    void setYRotation(GLint angle);
    void setZRotation(GLint angle);
    void setZoom(GLfloat factor);
    void timeOut();

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(GLint width, GLint height);

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);

private:
    GLint xRot;
    GLint yRot;
    GLint zRot;
    GLfloat factorZ;

    QPoint lastPos;
    QTimer *timer;
};     

#endif
