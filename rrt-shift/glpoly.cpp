#include "glpoly.h"
#include <stdio.h>

GLfloat inline min(GLfloat a, GLfloat b) { return a < b ? a : b; }
GLfloat inline max(GLfloat a, GLfloat b) { return a > b ? a : b; }

GLfloat glPoly::direction(
        pair<GLfloat, GLfloat> a,
        pair<GLfloat, GLfloat> b,
        pair<GLfloat, GLfloat> c) {

    return (b.first - a.first)*(c.second - a.second) -
           (c.first - a.first)*(b.second - a.second);
}

GLboolean glPoly::onSegment(
        pair<GLfloat, GLfloat> a,
        pair<GLfloat, GLfloat> b,
        pair<GLfloat, GLfloat> c) {

    if( (min(a.first, b.first) <= c.first) &&
        (max(a.first, b.first) >= c.first) &&
        (min(a.second, b.second) <= c.second) &&
        (max(a.second, b.second) >= c.second)) {
        return true;
    } else {
        return false;
    }
}

GLboolean glPoly::intersects(
        pair<GLfloat, GLfloat> a1, pair<GLfloat, GLfloat> a2,
        pair<GLfloat, GLfloat> a3, pair<GLfloat, GLfloat> a4) {

    GLdouble d1 = glPoly::direction(a3, a4, a1);
    GLdouble d2 = glPoly::direction(a3, a4, a2);
    GLdouble d3 = glPoly::direction(a1, a2, a3);
    GLdouble d4 = glPoly::direction(a1, a2, a4);

    if(((d1 > 0 && d2 < 0) ||
        (d1 < 0 && d2 > 0)) &&
       ((d3 > 0 && d4 < 0) ||
        (d3 < 0 && d4 > 0)) ) {
        return true;
    } else if(d1 == 0 && glPoly::onSegment(a3, a4, a1)) {
        return true;
    } else if(d2 == 0 && glPoly::onSegment(a3, a4, a2)) {
        return true;
    } else if(d3 == 0 && glPoly::onSegment(a1, a2, a3)) {
        return true;
    } else if(d4 == 0 && glPoly::onSegment(a1, a2, a4)) {
        return true;
    }

    return false;
}

pair<GLfloat, GLfloat> glPoly::randomPoint(GLfloat min, GLfloat max) {
    GLfloat range = max - min;
    GLfloat a  = range * rand() / RAND_MAX - range / 2.0;
    GLfloat b  = range * rand() / RAND_MAX - range / 2.0;

    return make_pair(a, b);
}

glPoly::glPoly(GLfloat pad) {
    padding = pad;
    n = 4; //rand() % 6;

    pair<GLfloat, GLfloat> p = glPoly::randomPoint(-2.0, 2.0);
    pair<GLfloat, GLfloat> q = glPoly::randomPoint(-0.5, 0.5);

    pair<GLfloat, GLfloat> r = make_pair(
        p.first  + q.first,
        p.second + q.second
    );

    init(p, r);
}

glPoly::glPoly(pair<GLfloat, GLfloat> p, pair<GLfloat, GLfloat> q, GLfloat pad) {
    padding = pad;
    init(p, q);
}

void glPoly::init(pair<GLfloat, GLfloat> p, pair<GLfloat, GLfloat> q) {
    // Lógica para que p quede arriba izquierda, q abajo derecha.
    if(p.first > q.first && q.second > p.second) {
        GLfloat t1 = p.first ;
        GLfloat t2 = p.second;

        p.first = q.first;
        p.second = q.second;
        q.first = t1;
        q.second = t2;
    } else if(p.first > q.first && p.second > q.second) {
        GLfloat t = p.first;
        p.first = q.first;
        q.first = t;
    } else if(q.first > p.first && q.second > p.second) {
        GLfloat t = p.second;
        p.second = q.second;
        q.second = t;
    }

    puntos.push_back(p);
    puntos.push_back(make_pair(q.first, p.second));
    puntos.push_back(q);
    puntos.push_back(make_pair(p.first, q.second));

    x = (p.first + q.first) / 2.0f;
    y = (p.second + q.second) / 2.0f;

    w = (p.first - q.first);  // 2.0f;
    h = (p.second - q.second); // 2.0f;

    if(w < 0.0f) w *= -1;
    if(h < 0.0f) h *= -1;

    bordes.push_back(make_pair(p.first, p.second + padding));
    bordes.push_back(make_pair(q.first, p.second + padding));

    bordes.push_back(make_pair(q.first + padding, p.second));
    bordes.push_back(make_pair(q.first + padding, q.second));

    bordes.push_back(make_pair(q.first, q.second - padding));
    bordes.push_back(make_pair(p.first, q.second - padding));

    bordes.push_back(make_pair(p.first - padding, q.second));
    bordes.push_back(make_pair(p.first - padding, p.second));
}

void glPoly::draw() {
    glPushMatrix();

    // Dibujar bases
    for(int j=0; j<2; j++) {
        GLfloat h = (j==0?-1:1) * 0.2;

        glBegin(GL_POLYGON);
        for(int i=0; i<n; ++i) glVertex3f(puntos[i].first, h, puntos[i].second);
        glEnd();
    }

    // Dibujar caras
    for(GLint i=0; i<n; i++) {
        GLfloat h = 0.2;
        GLint a, b;

        a = i; b = i+1;
        if(a == n-1) b = 0;

        glBegin(GL_QUADS);
        glVertex3f(puntos[a].first, h, puntos[a].second);
        glVertex3f(puntos[b].first, h, puntos[b].second);
        glVertex3f(puntos[b].first, -h, puntos[b].second);
        glVertex3f(puntos[a].first, -h, puntos[a].second);
        glEnd();
    }

    glPopMatrix();
}

void glPoly::drawOutline() {
    // Dibujar contorno
    glPushMatrix();
    glBegin(GL_LINE_LOOP);
    for(int i=0; i<2*n; ++i) glVertex3f(bordes[i].first, 0.0, bordes[i].second);
    glEnd();
    glPopMatrix();
}
