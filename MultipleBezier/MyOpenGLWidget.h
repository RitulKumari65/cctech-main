#ifndef MYOPENGLWIDGET_H
#define MYOPENGLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QMatrix4x4>
#include <vector>

using namespace std;

class Shape3D;
class Bezier;

class MyOpenGLWidget : public QOpenGLWidget, protected QOpenGLFunctions {
    Q_OBJECT

public:
    explicit MyOpenGLWidget(QWidget *parent = nullptr);
    ~MyOpenGLWidget();

    void setShape(Shape3D *shape);
    void addShape(Shape3D *shape) {
        shapes.push_back(shape);
        update();
    }
    void addBezierCurve(Bezier* bezier) {
        bezierCurves.push_back(bezier);
        update();
    }

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;

private:
    Shape3D *currentShape;
    vector<Shape3D*> shapes; // Store multiple shapes
    vector<Bezier*> bezierCurves; // Store multiple Bezier curves
    int selectedBezierIndex = -1; // Index of the selected Bezier curve
    int selectedControlPoint = -1; // Index of the selected control point
    float zoomFactor = 1.0f;

private slots:
    void drawAxis();
};

#endif // MYOPENGLWIDGET_H
