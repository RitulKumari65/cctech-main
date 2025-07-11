// sketcherwidget.cpp
#include "sketcherwidget.h"
#include <QDebug>
#include <QOpenGLFunctions>
#include <QPainter>
#include <QVector3D>
#include <QColor>
#include <QMatrix4x4>

SketcherWidget::SketcherWidget(QWidget* parent)
    : QOpenGLWidget(parent)
{
    setMouseTracking(true);
}

SketcherWidget::~SketcherWidget() {}

void SketcherWidget::initializeGL() {
    initializeOpenGLFunctions();
    glEnable(GL_DEPTH_TEST);
    glClearColor(0, 0, 0, 0);
}

void SketcherWidget::resizeGL(int w, int h) {
    glViewport(0, 0, w, h);
    QMatrix4x4 projection;
    projection.setToIdentity();
    projection.perspective(45.0f, float(w)/h, 0.00001f, 1000.0f);

    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(projection.constData());
    glMatrixMode(GL_MODELVIEW);
}

void SketcherWidget::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glTranslatef(0.0f, 0.0f, -100.0f);

    // Render points
    glPointSize(5.0f);
    glBegin(GL_POINTS);
    glColor3f(1.0f, 0.0f, 0.0f); // Red color for points
    for (const auto& point : tempPoints) {
        glVertex3f(point.x(), point.y(), point.z());
    }
    glEnd();

    // Render faces
    for (size_t i = 0; i < faces.size(); ++i) {
        QColor color = (i == faces.size() - 1) ? Qt::green : Qt::gray;
        drawFace(faces[i], color);
    }
}

void SketcherWidget::drawFace(Face* face, QColor color) {
    glColor3f(color.redF(), color.greenF(), color.blueF());
    glBegin(GL_POLYGON);
    for (auto edge : face->getEdges()) {
        Vertex* v = edge->getStart();
        glVertex3f(v->getX(), v->getY(), v->getZ());
    }
    glEnd();
}

void SketcherWidget::startCreatingShape() {
    creatingShape = true;
    tempPoints.clear();
    qDebug() << "Started creating a new shape.";
}

void SketcherWidget::mousePressEvent(QMouseEvent* event) {
    // Map 2D click to 3D (simplified for now: z=0)
    float x = (float(event->pos().x()) / width() - 0.5f) * 100;
    float y = (0.5f - float(event->pos().y()) / height()) * 100;
    float z = 0.0f;

    tempPoints.emplace_back(x, y, z);
    update(); // Trigger repaint to show the point
}

void SketcherWidget::finalizeCurrentShape() {
    qDebug() << "finalizeCurrentShape called";
    if (tempPoints.size() < 3) {
        qDebug() << "Not enough points to create a polygon. Points count:" << tempPoints.size();
        return;
    }

    std::vector<Vertex*> vertices;
    std::vector<Edge*> edges;

    // Add vertices to the sketcher
    for (const auto& point : tempPoints) {
        qDebug() << "Adding vertex at:" << point;
        Vertex* vertex = sketcher.addVertex(point.x(), point.y(), point.z());
        if (vertex) {
            vertices.push_back(vertex);
        } else {
            qDebug() << "Failed to add vertex at:" << point;
        }
    }

    // Create edges between consecutive vertices
    for (size_t i = 0; i < vertices.size(); ++i) {
        Vertex* start = vertices[i];
        Vertex* end = vertices[(i + 1) % vertices.size()]; // Wrap around to the first vertex
        qDebug() << "Creating edge between:" << start << "and" << end;
        Edge* edge = sketcher.addEdge(start, end);
        if (edge) {
            edges.push_back(edge);
        } else {
            qDebug() << "Failed to create edge between:" << start << "and" << end;
        }
    }

    // Add a face to the sketcher
    if (sketcher.checkFaceEdgesValidity(edges)) {
        qDebug() << "Edges are valid for creating a polygon.";
        Face* face = sketcher.addFace(edges);
        if (face) {
            faces.push_back(face);
            qDebug() << "Polygon created successfully.";
        } else {
            qDebug() << "Failed to create polygon.";
        }
    } else {
        qDebug() << "Invalid edges for creating a polygon.";
    }

    tempPoints.clear();
    creatingShape = false;
    update();
}

void SketcherWidget::finalizeShape() {
    creatingShape = false;

    std::vector<Vertex*> vertices;
    std::vector<Edge*> edges;

    for (const auto& point : tempPoints) {
        Vertex* v = sketcher.addVertex(point.x(), point.y(), point.z());
        vertices.push_back(v);
    }

    for (size_t i = 0; i < vertices.size(); ++i) {
        Edge* e = sketcher.addEdge(vertices[i], vertices[(i + 1) % vertices.size()]);
        edges.push_back(e);
    }

    Face* face = sketcher.addFace(edges);
    if (face) faces.push_back(face);

    tempPoints.clear();
    update();
}

void SketcherWidget::applyUnion() {
    if (faces.size() < 2) return;
    qDebug() << "Performing Face Union (mocked)";

    Face* f1 = faces[faces.size() - 2];
    Face* f2 = faces.back();

    Face* merged = new Face();
    for (auto e : f1->getEdges()) merged->addEdge(e);
    for (auto e : f2->getEdges()) merged->addEdge(e);

    faces.pop_back();
    faces.pop_back();
    faces.push_back(merged);
    update();
}

void SketcherWidget::applyDifference() {
    if (faces.size() < 2) return;
    qDebug() << "Performing Face Difference (mocked)";
    faces.pop_back();
    update();
}

void SketcherWidget::applyIntersection() {
    if (faces.size() < 2) return;
    qDebug() << "Performing Face Intersection";

    // Retrieve the last two faces
    Face* f1 = faces[faces.size() - 2];
    Face* f2 = faces.back();

    // Create a new face for the intersection (mocked logic for now)
    Face* intersectionFace = new Face();

    // Add edges from f1 to the intersection face
    for (auto e : f1->getEdges()) {
        intersectionFace->addEdge(e);
    }

    // Update the faces vector
    faces.pop_back();
    faces.pop_back();
    faces.push_back(intersectionFace);
    update();
}