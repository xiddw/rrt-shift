#ifndef GLPOLY_H
#define GLPOLY_H

#include <vector>
#if defined(__APPLE__) || defined(MACOSX)
    #include <GLUT/freeglut.h>
#else
    #include <GL/freeglut.h>
#endif

using namespace std;

class glPoly {

private:
    GLint n;    
    GLfloat padding;

    void init(pair<GLfloat, GLfloat>, pair<GLfloat, GLfloat>);


    static GLboolean onSegment(
            pair<GLfloat, GLfloat>,
            pair<GLfloat, GLfloat>,
            pair<GLfloat, GLfloat>);

    static GLfloat direction(
            pair<GLfloat, GLfloat>,
            pair<GLfloat, GLfloat>,
            pair<GLfloat, GLfloat>);

public:
    GLfloat w, h, x, y;

    vector< pair<GLfloat, GLfloat> > puntos;
    vector< pair<GLfloat, GLfloat> > bordes;

    glPoly(GLfloat pad);
    glPoly(pair<GLfloat, GLfloat>, pair<GLfloat, GLfloat>, GLfloat pad);

    void draw();
    void drawOutline();


    static pair<GLfloat, GLfloat> randomPoint(GLfloat min, GLfloat max);
    static GLboolean intersects(
            pair<GLfloat, GLfloat>, pair<GLfloat, GLfloat>,
            pair<GLfloat, GLfloat>, pair<GLfloat, GLfloat>);
};

#endif // GLPOLY_H
