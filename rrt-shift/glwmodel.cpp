#include "glwmodel.h"

inline GLfloat max(GLfloat a, GLfloat b) { return a > b ? a : b; }

void GLWModel::init() {
    obj = glmReadOBJ(_path);

    glmUnitize(obj);
    glmFacetNormals(obj);
    glmVertexNormals(obj, 90.0);

    glmScale(obj, _s);

    GLfloat td[3];
    glmDimensions(obj, td);

    _w = td[0];
    _h = td[1];
    _d = td[2];
}

GLWModel::GLWModel(const char* path) {
    _path = path;
    _s = 0.2;

    _incx = _incy = _incz = 0.02;
    _dirx = _diry = _dirz = true;
    _x = _y = _z = 0.0;

    if(_path != NULL) init();
}

GLWModel::~GLWModel() {
    _path = NULL;
    glmDelete(obj);
}

void GLWModel::draw() {
    if(obj == NULL && _path != NULL) init();
    if(obj == NULL) return;

    glPushMatrix();
    glTranslatef(_x, _y, _z);
    glmDraw(obj, GLM_SMOOTH);
    glPopMatrix();
}

void GLWModel::drawCircle() {
    if(obj == NULL) return;

    glLineWidth(0.001);
    glPushMatrix();
    glTranslatef(_x, _y, _z);
    glutWireSphere(radio(), 14, 14);
    glPopMatrix();
    glLineWidth(1.f);
}


void GLWModel::place(GLfloat x, GLfloat y, GLfloat z) {
    _x = x;
    _y = y;
    _z = z;
}

void GLWModel::animate() {
    _x += (_dirx?1:-1)*_incx;
    _y += (_diry?1:-1)*_incy;
    _z += (_dirz?1:-1)*_incz;
}

void GLWModel::changeXdir() { _dirx = !_dirx; }
void GLWModel::changeYdir() { _diry = !_diry; }
void GLWModel::changeZdir() { _dirz = !_dirz; }

void GLWModel::setXinc(GLfloat inc) { _incx = inc; }
void GLWModel::setYinc(GLfloat inc) { _incy = inc; }
void GLWModel::setZinc(GLfloat inc) { _incz = inc; }

const char* GLWModel::path() {
    return _path;
}

GLfloat GLWModel::x() const { return _x; }
GLfloat GLWModel::y() const { return _y; }
GLfloat GLWModel::z() const { return _z; }

GLfloat GLWModel::radio() const { return max(max(_w, _h), _d) / 2.0; }

GLfloat GLWModel::scaleFactor() const { return _s; }
void GLWModel::setScaleFactor(GLfloat s) {
    if(_s < 0) return;
    _s = s;
}
