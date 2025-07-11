#include "MyOpenGLWidget.h"
#include "Shape3D.h"
#include "bezier.h"
#include <QOpenGLShaderProgram>
#include <QDebug>
#include <cmath> // For tan and M_PI

MyOpenGLWidget::MyOpenGLWidget(QWidget *parent)
    : QOpenGLWidget(parent), currentShape(nullptr), selectedControlPoint(-1), selectedBezierIndex(-1) {}

MyOpenGLWidget::~MyOpenGLWidget() {
    delete currentShape;
    for (Shape3D *shape : shapes) {
        delete shape;
    }
    for (Bezier *bezier : bezierCurves) {
        delete bezier;
    }
}

void MyOpenGLWidget::setShape(Shape3D *shape) {
    if (currentShape)
        delete currentShape;
    currentShape = shape;
    update();
}

void MyOpenGLWidget::initializeGL() {
    initializeOpenGLFunctions();
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
}

void MyOpenGLWidget::resizeGL(int w, int h) {

    glViewport(0, 0, w, h);

    QMatrix4x4 projection;
    projection.setToIdentity();
    projection.perspective(45.0f, float(w)/h, 0.00001f, 1000.0f);

    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(projection.constData());
    glMatrixMode(GL_MODELVIEW);
}

void MyOpenGLWidget::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    QMatrix4x4 viewMatrix;
    viewMatrix.setToIdentity();
    viewMatrix.translate(0, 0, -5.0f); // Move the camera back
    viewMatrix.scale(zoomFactor); // Apply zoom factor
    glLoadMatrixf(viewMatrix.data());

    for (Bezier* bezier : bezierCurves) {
        bezier->draw();
    }

    // Find and draw intersection points
    glColor3f(1.0f, 1.0f, 0.0f); // White color for intersection points
    glPointSize(10.0f);
    for (size_t i = 0; i < bezierCurves.size(); ++i) {
        for (size_t j = i + 1; j < bezierCurves.size(); ++j) {
            std::vector<QPointF> intersections = bezierCurves[i]->findIntersections(*bezierCurves[j]);
            glBegin(GL_POINTS);
            for (const auto& point : intersections) {
                glVertex2f(point.x(), point.y());
            }
            glEnd();
        }
    }

    drawAxis();
}

void MyOpenGLWidget::mousePressEvent(QMouseEvent *event) {
    float ndcX = 4 * ((2.0f * event->pos().x()) / width() - 1.0f);
    float ndcY = 2 * (1.0f - (2.0f * event->pos().y()) / height());

    float worldX = ndcX / zoomFactor;
    float worldY = ndcY / zoomFactor;

    QPointF mousePos = QPointF(worldX, worldY);

    selectedBezierIndex = -1;
    selectedControlPoint = -1;

    for (int i = 0; i < bezierCurves.size(); ++i) {
        Bezier* bezier = bezierCurves[i];
        int controlPointIndex = bezier->findControlPoint(mousePos, 0.4f / zoomFactor);
        if (controlPointIndex != -1) {
            selectedBezierIndex = i;
            selectedControlPoint = controlPointIndex;
            qDebug() << "Selected Bezier:" << selectedBezierIndex << "Control Point:" << selectedControlPoint;
            break;
        }
    }
}

void MyOpenGLWidget::mouseMoveEvent(QMouseEvent *event) {
    if (selectedBezierIndex != -1 && selectedControlPoint != -1) {
        float ndcX = (2.0f * event->pos().x()) / width() - 1.0f;
        float ndcY = 1.0f - (2.0f * event->pos().y()) / height();

        float worldX = 4 * (ndcX / zoomFactor);
        float worldY = 2 * (ndcY / zoomFactor);
        QPointF mousePos = QPointF(worldX, worldY);

        Bezier* bezier = bezierCurves[selectedBezierIndex];
        bezier->updateControlPoint(selectedControlPoint, mousePos);
        update();
    }
}

void MyOpenGLWidget::wheelEvent(QWheelEvent *event) {
    if (event->angleDelta().y() > 0) {
        zoomFactor *= 1.1f; // Zoom in
    } else {
        zoomFactor /= 1.1f; // Zoom out
    }
    update();
}

void MyOpenGLWidget::drawAxis() {
    // draw x and y axis from -10 to 10
    glBegin(GL_LINES);
    glColor3f(1.0f, 0.0f, 0.0f); // Red for X axis
    glVertex3f(-10.0f, 0.0f, 0.0f);
    glVertex3f(10.0f, 0.0f, 0.0f);
    glColor3f(0.0f, 1.0f, 0.0f); // Green for Y axis
    glVertex3f(0.0f, -10.0f, 0.0f);
    glVertex3f(0.0f, 10.0f, 0.0f);
    glEnd();
}


