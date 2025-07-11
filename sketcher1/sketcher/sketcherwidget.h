// sketcherwidget.h
#pragma once

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QMouseEvent>
#include <vector>
#include "sketcher.h"

class SketcherWidget : public QOpenGLWidget, protected QOpenGLFunctions {
    Q_OBJECT

public:
    explicit SketcherWidget(QWidget* parent = nullptr);
    ~SketcherWidget();

    void startCreatingShape();
    void finalizeCurrentShape(); // New method to finalize the current shape

public slots:
    void applyUnion();
    void applyDifference();
    void applyIntersection();

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

    void mousePressEvent(QMouseEvent* event) override;

private:
    Sketcher sketcher;
    std::vector<Solid*> shapes;
    std::vector<QVector3D> tempPoints; // Temporarily stores clicked points
    std::vector<Face*> faces; // Stores created faces
    bool creatingShape = false;

    void finalizeShape();
    void drawSolid(Solid* solid, QColor color = Qt::gray);
    void drawFace(Face* face, QColor color);
};
