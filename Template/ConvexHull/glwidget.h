// Convexe Hlle
// 
// Widget fr Interaktion und Kontrolle 
//
// (c) Georg Umlauf, 2014

#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>
#include <QFuture>
#include "mylist.h"

class GLWidget : public QGLWidget
{
    Q_OBJECT
public:
    GLWidget                  (QWidget *parent=0);
    ~GLWidget                 ();
signals: 
	void continueRequest      ();
public slots:
    void radioButton1Clicked  ();
    void radioButton2Clicked  ();
protected:
    void paintGL              ();
    void initializeGL         ();
    void resizeGL             (int width, int height);
    void keyPressEvent        (QKeyEvent   *event);
    void mousePressEvent      (QMouseEvent *event);
private:
    QPointF transformPosition (QPoint p);
	double  aspectx, aspecty;
    MyList * pointsList;
    MyList * convList;
    int method;
    float leftTurn(QPointF a, QPointF b, QPointF c);
    void grahamsScan();
    void doConvexHull();
    float getCos(QPointF a, QPointF b, QPointF c);
    void jarvisMarch();
    int getMinAngle(QPointF a, QPointF b, int ka, int kb);
    int getMaxAngle(QPointF a, QPointF b, int ka, int kb);
};



#endif // GLWIDGET_H
