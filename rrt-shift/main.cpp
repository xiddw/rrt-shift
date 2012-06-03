#include <QApplication>
#include <QDesktopWidget>

#include "window.h"
#include <stdlib.h>
#include <time.h>

#include "glpoly.h"
#include "kdtree.h"

int main(int argc, char *argv[]) {
    srand(time(NULL));

    QApplication app(argc, argv);
    Window window;
    window.show();

    return app.exec();
}

