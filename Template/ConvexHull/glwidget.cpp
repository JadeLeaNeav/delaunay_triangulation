// Convexe Hülle
// (c) Georg Umlauf, 2015

#include "glwidget.h"
#include <QtGui>
#include <GL/glu.h>
#include "mainwindow.h"


GLWidget::GLWidget(QWidget *parent) : QGLWidget(parent)
{	

    this->pointsList = new MyList();
    this->convList = new MyList();
    this->method = -1;
}

GLWidget::~GLWidget()
{
}

void GLWidget::paintGL()
{
    // clear
    glClear(GL_COLOR_BUFFER_BIT);

    // Koordinatensystem
    glColor3f(0.5,0.5,0.5);
    glBegin(GL_LINES);
    glVertex2f(-1.0, 0.0);
    glVertex2f( 1.0, 0.0);
    glVertex2f( 0.0,-1.0);
    glVertex2f( 0.0, 1.0);
    glEnd();

	// Konvexe Hülle zeichnen
	// TODO: draw convex hull using glBegin(GL_LINE_STRIP); ... glEnd();


    glPointSize(5);
    glBegin(GL_POINTS);
    glColor3f(1.0, 0.0, 0.0);


    for (int k = 0 ; k < this->pointsList->getPointsNumber() ; k++) {
        glVertex2f( this->pointsList->getPoint(k).x() , this->pointsList->getPoint(k).y() );

    }
    glEnd();

    if (this->pointsList->getPointsNumber() > 5) {
        glBegin(GL_POINTS);
        glColor3f(0.0, 0.0, 1.0);
        qDebug() << "coucou";
        int k = this->pointsList->minPoint();
        glVertex2f( this->pointsList->getPoint(k).x() , this->pointsList->getPoint(k).y() );
        glEnd();
    }

    if (this->method != -1) {
        glBegin(GL_LINE_STRIP);
        glColor3f(0.0, 1.0, 0.0);
        for (int k = 0 ; k < this->convList->getPointsNumber() ; k++) {
            glVertex2f( this->convList->getPoint(k).x() , this->convList->getPoint(k).y() );
        }
        if (this->convList->getPointsNumber() > 0 ) {
            glVertex2f( this->convList->getPoint(0).x() , this->convList->getPoint(0).y() );
        }
        glEnd();
    }



}


void GLWidget::initializeGL()
{
    resizeGL(width(),height());
}

void GLWidget::resizeGL(int width, int height)
{
    aspectx=1.0;
    aspecty=1.0;
    if (width>height) aspectx = float(width) /height;
    else              aspecty = float(height)/ width;
    glViewport    (0,0,width,height);
    glMatrixMode  (GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D    (-aspectx,aspectx,-aspecty,aspecty);
    glMatrixMode  (GL_MODELVIEW);
    glLoadIdentity();
}

QPointF GLWidget::transformPosition(QPoint p)
{
    return QPointF( (2.0*p.x()/ width() - 1.0)*aspectx,
		           -(2.0*p.y()/height() - 1.0)*aspecty);
}

void GLWidget::keyPressEvent(QKeyEvent * event)
{
	switch (event->key()) {
	default:
		break;
	}
	update();
}

void GLWidget::mousePressEvent(QMouseEvent *event)
{
	QPointF posF = transformPosition(event->pos());
	if (event->buttons() & Qt::LeftButton ) {
		// TODO: add point to point list and recompute convex hull
        this->pointsList->addPoint(posF);
        this->doConvexHull();
	}
    update(); 
}


void GLWidget::radioButton1Clicked()
{
	// TODO: toggle to Jarvis' march
    this->method = 1;
    update();
}

void GLWidget::radioButton2Clicked()
{
	// TODO: toggle to Graham's scan
    this->method = 0;
    update();
}

float GLWidget::leftTurn(QPointF a, QPointF b, QPointF c) {
    return (b.y() - a.y()) * (c.x() - b.x()) - (c.y() - b.y()) * (b.x() - a.x());
}

void GLWidget::grahamsScan() {
   int np = this->pointsList->getPointsNumber();
   MyList * upper = new MyList();

       this->pointsList->mergeSort();
       upper->addPoint(this->pointsList->getPoint(0));
       upper->addPoint(this->pointsList->getPoint(1));
       int nupper = 2;
       float lt;
       for (int i = 2 ; i < np ; i++) {
            upper->addPoint(this->pointsList->getPoint(i));
            nupper++;

            lt = leftTurn(upper->getPoint(nupper - 3), upper->getPoint(nupper - 2), upper->getPoint(nupper - 1));

            while ( nupper > 2 && lt > 0 ) {
                upper->deletePenultimate();
                nupper--;
                if (nupper > 2) {
                    lt = leftTurn(upper->getPoint(nupper - 3), upper->getPoint(nupper - 2), upper->getPoint(nupper - 1));
                }
            }
       }

            MyList lower;
            lower.addPoint(this->pointsList->getPoint(np-1));
            lower.addPoint(this->pointsList->getPoint(np-2));
            int nlower = 2;
            for (int i = np -3 ; i >= 0 ; i--) {
                 lower.addPoint(this->pointsList->getPoint(i));
                 nlower++;
                 lt = leftTurn(lower.getPoint(nlower - 3), lower.getPoint(nlower - 2), lower.getPoint(nlower - 1));

                 while ( nlower > 2 && lt > 0 ) {
                     lower.deletePenultimate();
                     nlower--;
                     if (nlower > 2) {
                         lt = leftTurn(lower.getPoint(nlower - 3), lower.getPoint(nlower - 2), lower.getPoint(nlower - 1));
                     }
                 }

             }
   upper->concatenate(lower);

   this->convList = upper;

}

void GLWidget::doConvexHull() {
    int pN = this->pointsList->getPointsNumber();
    if (pN > 3) {
        if (this->method == 0) {
            this->grahamsScan();
        }
        if (this->method == 1) {
            this->jarvisMarch();
        }
    } else {
            for (int k = 0 ; k < pN ; k++) {
                this->convList->addPoint(this->pointsList->getPoint(k));
    }
}
}


float GLWidget::getCos(QPointF a, QPointF b, QPointF c) {
    float n1 = sqrt( (a.x() - b.x()) * (a.x() - b.x()) + (a.y() - b.y()) * (a.y() - b.y()) );
    float n2 = sqrt( (c.x() - a.x()) * (c.x() - a.x()) + (c.y() - a.y()) * (c.y() - a.y()) );
    float ps = (b.x() - a.x()) * (c.x() - a.x()) + (b.y() - a.y()) * (c.y() - a.y());
    return ps / (n1 * n2);
}

int GLWidget::getMinAngle(QPointF a, QPointF b, int ka, int kb) {
    int n = this->pointsList->getPointsNumber();
    qDebug()<< "getMinAngle " << n;
    float currentMaxCos = -1;
    int currentIndexMin = -1;
    float currentCos;
    for (int k = 0 ; k < n ; k++) {
        if (k != ka && k != kb) {
            currentCos = getCos(a, b, this->pointsList->getPoint(k));
            qDebug()<< currentCos;
            if (currentCos > currentMaxCos) {
                currentMaxCos = currentCos;
                currentIndexMin = k;
                qDebug()<< k;
            }
        }
    }
    return currentIndexMin;
}

int GLWidget::getMaxAngle(QPointF a, QPointF b, int ka, int kb) {
    int n = this->pointsList->getPointsNumber();
    qDebug()<< "getMaxAngle " << n;
    float currentMinCos = 1;
    int currentIndexMax = -1;
    float currentCos;
    for (int k = 0 ; k < n ; k++) {
        if (k != ka && k != kb) {
            currentCos = getCos(b, this->pointsList->getPoint(k), a);
            qDebug()<< currentCos;
            if (currentCos < currentMinCos) {
                currentMinCos = currentCos;
                currentIndexMax = k;
                qDebug()<< k;
            }
        }
    }
    return currentIndexMax;
}


void GLWidget::jarvisMarch() {
    MyList * liste = new MyList();

        int i1 = this->pointsList->minPoint();
        int first = i1;
        int i2;
        QPointF p1 = this->pointsList->getPoint(i1);
        liste->addPoint(p1);
        i2 = this->getMinAngle(p1, QPointF(p1.x() +1, p1.y()), i1, -1);
        QPointF p2 = this->pointsList->getPoint(i2);
        liste->addPoint(p2);
        int i3;
        QPointF p3;
        do {
            i3 = this->getMaxAngle(p1, p2, i1, i2);

            qDebug() << "jarvis " << i1 << ", " << i2 << ", " << i3 << ", " << first;
            p3 = this->pointsList->getPoint(i3);
            liste->addPoint(p3);
            p1 = p2;
            p2 = p3;
            i1 = i2;
            i2 = i3;


        }
        while(first != i3);
    this->convList = liste;

}

