#ifndef WINDOW_H
#define WINDOW_H

#include <QtGui>
#include <QWidget>

class GLWidget;

class Window : public QWidget {
    Q_OBJECT

public:
    Window();

public slots:
    void clickStart(void);
    void clickStop(void);
    void clickSolve(void);

    void clickReset(void);
    void clickAux(bool);

protected:
    void keyPressEvent(QKeyEvent *event);

private:
    GLWidget *glWidgetMain, *glWidgetSpin, *glWidgetTop;
    QSpinBox *spinObst, *spinPoints;

    QSplitter *bottomSplit, *mainSplit;
};

#endif
