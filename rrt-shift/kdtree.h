#ifndef KDTREE_H
#define KDTREE_H

#include <iostream>
#if defined(__APPLE__) || defined(MACOSX)
    #include <GLUT/freeglut.h>
#else
    #include <GL/freeglut.h>
#endif

using namespace std;

class kdnode {
private:

public:
    static int n;

    kdnode();
    kdnode(pair<GLfloat, GLfloat>);

    kdnode *l[2];

    pair<GLfloat, GLfloat> p;
    int id;
};

class kdtree {
private:
    kdnode *root;
    GLboolean x;

    void    insertR(kdnode*&, pair<GLfloat, GLfloat>);
    kdnode* searchR(kdnode*&, pair<GLfloat, GLfloat>);
    void    walkR(kdnode *n);
public:
    kdtree();

    void    walk(void);
    void    insert(pair<GLfloat, GLfloat>);
    kdnode* search(pair<GLfloat, GLfloat>);
};

#endif // KDTREE_H

// Sample code :)

//kdtree *arbol = new kdtree();

//for(int i=0; i<100; ++i) {
//    arbol->insert(glPoly::randomPoint(-5, 5));
//}

//pair<GLfloat, GLfloat> m = glPoly::randomPoint(-5, 5);
//pair<GLfloat, GLfloat> n = arbol->search(m)->p;

//cout << m.first << ", " << m.second << endl;
//cout << n.first << ", " << n.second << endl;
