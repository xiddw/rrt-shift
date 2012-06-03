#ifndef GLWMODEL_H
#define GLWMODEL_H

#include <GL/glu.h>
#include "glm.h"

class GLWModel {
private:
    const char *_path;

    GLfloat     _x, _y, _z;
    GLfloat     _w, _h, _d;

    GLfloat     _s;
    GLfloat     _incx, _incy, _incz;
    GLboolean   _dirx, _diry, _dirz;

    void init();
public:
    GLMmodel *obj;

    GLWModel(const char*);
    ~GLWModel();

    void place(GLfloat x, GLfloat y, GLfloat z);
    void draw(void);
    void drawCircle(void);

    void animate(void);

    void changeXdir(void);
    void changeYdir(void);
    void changeZdir(void);

    void setXinc(GLfloat inc);
    void setYinc(GLfloat inc);
    void setZinc(GLfloat inc);

    const char* path();

    void setScaleFactor(GLfloat s);
    GLfloat scaleFactor(void) const;

    GLfloat x() const;
    GLfloat y() const;
    GLfloat z() const;

    GLfloat radio() const;
};

#endif // GLWMODEL_H
