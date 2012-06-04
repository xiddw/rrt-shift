#include <QtGui>

#include "glwidget.h"
#include "kdtree.h"
#include "window.h"

void Window::clickStart(void) {
    //clickSolve();

    glWidgetMain->StartAnimation();
    glWidgetTop->StartAnimation();
    glWidgetSpin->StartAnimation();

    spinObst->setDisabled(true);
    spinPoints->setDisabled(true);
}

void Window::clickStop(void) {
    glWidgetMain->StopAnimation();
    glWidgetTop->StopAnimation();
    glWidgetSpin->StopAnimation();
}

//void dfs(int a) {
//    GLWidget::gVisited[a] = true;
//    int v = GLWidget::g[];

//    kdnode* p = GLWidget::arrNodes[v];

//    while( p != NULL) {
//        if(!GLWidget::gVisited[p->id]) {
//            GLWidget::gVisited[p->id] = true;
//            GLWidget::gParents[p->id] = v;

//            dfs(p->id);
//        }

//        p++;
//    }
//}

void Window::clickSolve(void) {

    GLWidget::numObst = spinObst->value();
    GLWidget::numPoints = spinPoints->value();

    // Calcular obstaculos en el escenario
    if(GLWidget::arrObst == NULL) {
        GLfloat radio = GLWidget::a->radio();

        GLWidget::arrObst = new glPoly*[GLWidget::numObst];
        for(GLint i=0; i < GLWidget::numObst; ++i) {
            GLWidget::arrObst[i] = new glPoly(radio);
        }
    }

    pair<GLfloat, GLfloat> pa, pb, pc;

    // Crear arreglo para la lista de puntos que estarán conectados
    //GLWidget::arrPoints = new vector< pair<GLfloat, GLfloat> >();

    //GLWidget::arrNodes = new pair<GLfloat, GLfloat>[GLWidget::numPoints] ();
    int tt = GLWidget::numPoints * 3;
    GLWidget::arrNodes = new kdnode*[tt] ();

    GLWidget::g = new vector<int>*[tt];
    for(int i=0; i<tt; ++i) {
        GLWidget::g[i] = new vector<int>(); //GLWidget::numPoints);
    }

    // Crear kd-trees que irán almacenando de cada lado los puntos

    kdtree *arbolA = new kdtree();
    kdtree *arbolB = new kdtree();

    arbolA->insert(make_pair(-1.8, -1.8));
    arbolB->insert(make_pair(1.8, 1.8));

    // Obtener puntos que sean válidos (no estén dentro de un obstáculo, o
    // se crucen con uno) e irlos agregando al kdtree como a un grafo para
    // ir manejando sus relaciones

    kdnode *nodea, *nodeb;
    kdnode *last;

    for(int k=0; k<GLWidget::numPoints; ++k) {

        GLboolean intsA, intsB;
        do {
            intsA = false;
            intsB = false;

            pc = glPoly::randomPoint(-2, 2);

            nodea = arbolA->search(pc);
            pa = nodea->p;

            nodeb = arbolB->search(pc);
            pb = nodeb->p;

            for(int i=0; i<GLWidget::numObst; ++i) {
                for(vector< pair<GLfloat, GLfloat> >::iterator
                        it = GLWidget::arrObst[i]->bordes.begin();
                        it != (GLWidget::arrObst[i]->bordes.end() -1);
                        ++it) {

                    if(glPoly::intersects(pa, pc, *it, *(it+1)))  intsA = true;
                    if(glPoly::intersects(pb, pc, *it, *(it+1)))  intsB = true;

                    if(intsA && intsB) break;
                }
                if(intsA && intsB) break;
            }

        } while(intsA || intsB);


        if(!intsA) {
            arbolA->insert(pc);
            last = arbolA->search(pc);
            GLWidget::g[nodea->id]->push_back(last->id);
        }
        GLWidget::arrNodes[last->id] = last;

        if(!intsB) {
            arbolB->insert(pc);
            last = arbolB->search(pc);
            GLWidget::g[nodeb->id]->push_back(last->id);
        }
        GLWidget::arrNodes[last->id] = last;

//        GLWidget::arrNodes[last->id] = last;

//        GLboolean intsB;
//        do {
//            intsB = false;
//            pc = glPoly::randomPoint(-2, 2);

//            nodeb = arbolB->search(pc);
//            pb = nodeb->p;

//            for(int i=0; i<GLWidget::numObst; ++i) {
//                for(vector< pair<GLfloat, GLfloat> >::iterator
//                        it = GLWidget::arrObst[i]->bordes.begin();
//                        it != (GLWidget::arrObst[i]->bordes.end() - 1);
//                        ++it) {

//                    if(glPoly::intersects(pb, pc, *it, *(it+1)))  intsB = true;

//                    if(intsB) break;
//                }
//                if(intsB) break;
//            }
//        } while(intsB);

//        if(!intsB) {
//            arbolB->insert(pc);
//            last = arbolB->search(pc);
//            GLWidget::g[nodeb->id]->push_back(last->id);
//        }
//        GLWidget::arrNodes[last->id] = last;


    }


    glWidgetMain->RefreshAnimation();
    glWidgetTop->RefreshAnimation();
    glWidgetSpin->RefreshAnimation();
}

void Window::clickReset(void) {
    glWidgetMain->ResetAnimation();
    glWidgetTop->ResetAnimation();
    glWidgetSpin->ResetAnimation();

    delete glWidgetMain;
    delete glWidgetTop;
    delete glWidgetSpin;

    delete[] GLWidget::arrNodes;
    delete[] GLWidget::g;

    glWidgetMain = new GLWidget(this);
    glWidgetMain->setView(GLWidget::PERSPECTIVE);
    glWidgetMain->setMinimumSize(200, 200);

    glWidgetSpin = new GLWidget(this);
    glWidgetSpin->setView(GLWidget::SPINNING);
    glWidgetSpin->setMinimumSize(200, 200);

    glWidgetTop = new GLWidget(this);
    glWidgetTop->setView(GLWidget::TOPVIEW);
    glWidgetTop->setMinimumSize(200, 200);

    bottomSplit->addWidget(glWidgetSpin);
    bottomSplit->addWidget(glWidgetTop);

    mainSplit->addWidget(glWidgetMain);
    mainSplit->addWidget(glWidgetMain);

    GLWidget::numObst = 0;
    GLWidget::numPoints = 0;

    spinObst->setDisabled(false);
    spinPoints->setDisabled(false);
}

void Window::clickAux(bool a) {
    GLWidget::showAux = a;
}

Window::Window() {
    QVBoxLayout *mainLayout = new QVBoxLayout();

    mainSplit = new QSplitter(Qt::Vertical);
    mainSplit->setChildrenCollapsible(false);

    bottomSplit = new QSplitter(Qt::Horizontal);
    bottomSplit->setChildrenCollapsible(false);

    // No sé por que necesito esta linea :(
    GLWidget *glWidgetTemp = new GLWidget(); delete glWidgetTemp;

    glWidgetMain = new GLWidget(this);
    glWidgetMain->setView(GLWidget::PERSPECTIVE);
    glWidgetMain->setMinimumSize(200, 200);

    glWidgetSpin = new GLWidget(this);
    glWidgetSpin->setView(GLWidget::SPINNING);
    glWidgetSpin->setMinimumSize(200, 200);

    glWidgetTop = new GLWidget(this);
    glWidgetTop->setView(GLWidget::TOPVIEW);
    glWidgetTop->setMinimumSize(200, 200);

    QGroupBox *groupBox = new QGroupBox(tr("Opciones de configuración"));
    QFormLayout *opcLayout = new QFormLayout();

    QCheckBox *btnAuxiliar = new QCheckBox(tr("Mostrar lineas auxiliares"));
    connect(btnAuxiliar, SIGNAL(toggled(bool)), this, SLOT(clickAux(bool)));

    QLabel *label1 = new QLabel(tr("Número de obstáculos: "));
    spinObst = new QSpinBox();
    spinObst->setAlignment(Qt::AlignRight);
    spinObst->setMinimum(1);
    spinObst->setMaximum(80);
    spinObst->setValue(10);

    QLabel *label2 = new QLabel(tr("Número de puntos aleatorios: "));
    spinPoints = new QSpinBox();
    spinPoints->setAlignment(Qt::AlignRight);
    spinPoints->setMinimum(1);
    spinPoints->setMaximum(1000);
    spinPoints->setValue(80);

    QPushButton *btnSolve = new QPushButton(tr("&Resolver"));
    connect(btnSolve, SIGNAL(clicked()), this, SLOT(clickSolve()));

    QPushButton *btnStart = new QPushButton(tr("&Iniciar simulación"));
    connect(btnStart, SIGNAL(clicked()), this, SLOT(clickStart()));

    QPushButton *btnStop = new QPushButton(tr("&Detener simulación"));
    connect(btnStop, SIGNAL(clicked()), this, SLOT(clickStop()));

    QPushButton *btnReset = new QPushButton(tr("Volver a valores por de&fault"));
    connect(btnReset, SIGNAL(clicked()), this, SLOT(clickReset()));

    QFrame* line1 = new QFrame();
    line1->setFrameShape(QFrame::HLine);
    line1->setFrameShadow(QFrame::Sunken);
    QFrame* line2 = new QFrame();
    line2->setFrameShape(QFrame::HLine);
    line2->setFrameShadow(QFrame::Sunken);

    opcLayout->addRow(btnAuxiliar);

    opcLayout->addRow(line1);
    opcLayout->addRow(label1, spinObst);
    opcLayout->addRow(label2, spinPoints);
    opcLayout->addRow(line2);

    opcLayout->addRow(btnSolve);
    opcLayout->addRow(btnStart);
    opcLayout->addRow(btnStop);
    //opcLayout->addRow(btnReset);

    groupBox->setLayout(opcLayout);
    groupBox->setMinimumSize(200, 200);
    groupBox->setMaximumWidth(300);

    bottomSplit->addWidget(groupBox);
    bottomSplit->addWidget(glWidgetSpin);
    bottomSplit->addWidget(glWidgetTop);    

    mainSplit->addWidget(bottomSplit);
    mainSplit->addWidget(glWidgetMain);
    mainSplit->addWidget(glWidgetMain);

    mainLayout->addWidget(mainSplit);
    setLayout(mainLayout);

    setWindowTitle(trUtf8("Examen Final - Programacion Avanzada - RJRJ"));
    setMinimumSize(800, 640);
    resize(800, 640);
}

void Window::keyPressEvent(QKeyEvent *e) {
    if (e->key() == Qt::Key_Escape) {
        close();        
    } else {
        QWidget::keyPressEvent(e);
    }
}
