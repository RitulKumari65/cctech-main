#pragma once

#include <QOpenGLFunctions>
#include <QOpenGLWidget>
#include <QVector3D>
#include <QVector>
#include <QMouseEvent>
#include <QComboBox>
#include <QPushButton>
#include <QCheckBox>
#include <vector>
#include <QPointF>
#include <QPainterPath>

struct Polygon {
    QVector<QVector3D> points;          // Points of the polygon
    QVector3D normal;                   // Normal vector of the polygon
    float BooleanHeight = 0.0f;       // Boolean height for the polygon
    bool isClosed = false;              // Whether the polygon is closed
    QPoint position;                    // Position for free movement
    // For arc/circle support (if used)
    bool isArc = false;
    bool isCircle = false;
    QVector3D arcCenter;
    float arcRadius = 0.0f;
    float arcStartAngle = 0.0f;
    float arcEndAngle = 0.0f;
    QVector3D circleCenter;
    float circleRadius = 0.0f;
};

class booleanoperations : public QOpenGLWidget, protected QOpenGLFunctions {
    Q_OBJECT

public:
    explicit booleanoperations(QWidget *parent = nullptr);
    ~booleanoperations();

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    QVector<Polygon> polygons;
    int selectedPolygonIndex = -1;
    QPoint lastMousePos;
    float rotationX = 0.0f;
    float rotationY = 0.0f;
    bool rightMousePressed = false;
    bool polygonClosed = false;
    float BooleanHeight = 0.0f;

    QComboBox *modeComboBox = nullptr;
    QComboBox *booleanComboBox = nullptr;
    QCheckBox *subtractionModeCheckBox = nullptr;
    QPushButton *applyBooleanButton = nullptr;

    // For arc/circle drawing
    bool drawingArc = false;
    bool drawingCircle = false;
    QVector3D arcStartPoint, arcEndPoint, circleCenter;
    float circleRadius = 0.0f;

    // Helper functions
    QVector3D screenToWorld(const QPoint &pos);
    bool isCloseToFirstPoint(const QVector3D &p, const Polygon &polygon);
    QVector3D computePolygonNormal(const QVector<QVector3D> &points);

    // Boolean operation helpers
    bool pointInPolygon(const QPointF& point, const std::vector<QPointF>& polygon);
    bool segmentsIntersect(const QPointF& a1, const QPointF& a2, const QPointF& b1, const QPointF& b2, QPointF& out);
    void addAndSortIntersections(std::vector<QPointF>& result, const QPointF& point);
    std::vector<QPointF> splitEdges(const std::vector<QPointF>& polygon, const std::vector<QPointF>& intersections);

    // Boolean operations
    std::vector<QPointF> intersect(const std::vector<QPointF>& poly1, const std::vector<QPointF>& poly2);
    std::vector<QPointF> unionPolygons(const std::vector<QPointF>& poly1, const std::vector<QPointF>& poly2);
    std::vector<QPointF> subtractPolygons(const std::vector<QPointF>& poly1, const std::vector<QPointF>& poly2);

    // QPainterPath conversion
    QPainterPath convertToPath(const Polygon &shape);

    // Arc/circle support
    void addCircle(const QVector3D &center, float radius);
    void addArc(const QVector3D &startPoint, const QVector3D &endPoint);

private slots:
    void onApplyBooleanOperation();
};