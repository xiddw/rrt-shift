#include "kdtree.h"

int kdnode::n = 0;

kdnode::kdnode() {
    l[0] = l[1] = NULL;
}

kdnode::kdnode(pair<GLfloat, GLfloat> a) {
    p = a;
    l[0] = l[1] = NULL;
    id = n++;
}

kdtree::kdtree() {
    root = NULL;
    x = false;
}

void kdtree::walk() {
    walkR(root);
}

void kdtree::walkR(kdnode *n) {
    if(n == NULL) return;

    walkR(n->l[0]);

    cout << n->p.first << ", " << n->p.second << endl;

    walkR(n->l[1]);
}

void kdtree::insert(pair<GLfloat, GLfloat> a) {
    insertR(root, a);
}

kdnode* kdtree::search(pair<GLfloat, GLfloat> a) {
    return searchR(root, a);
}

void kdtree::insertR(kdnode *&n, pair<GLfloat, GLfloat> a) {
    x = !x;

    if(n == NULL) {
        n = new kdnode(a);
        return;
    }

    bool p = false;
    if(x) {
        p = a.first >= n->p.first;
    } else {
        p = a.second >=  n->p.second;
    }

    insertR(n->l[p], a);
}

kdnode* kdtree::searchR(kdnode *&n, pair<GLfloat, GLfloat> a) {
    x = !x;

    if(n == NULL) return NULL;

    bool p = false;
    if(x) {
        p = a.first >= n->p.first;
    } else {
        p = a.second >=  n->p.second;
    }

    kdnode *t = searchR(n->l[p], a);

    if(t == NULL) return n;
    return t;
}

