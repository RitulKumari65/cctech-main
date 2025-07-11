#include "PolygonExtrusionWidget.h"
#include <QOpenGLFunctions>
#include <QDebug>
#include <cmath>
#include <QComboBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QCheckBox>
#include <algorithm>
#include <vector>

using std::vector;

PolygonExtrusionWidget::PolygonExtrusionWidget(QWidget *parent)
    : QOpenGLWidget(parent) {
    setMouseTracking(true);
    setWindowFlags(Qt::Window | Qt::WindowMinimizeButtonHint |
                   Qt::WindowMaximizeButtonHint | Qt::WindowCloseButtonHint);

    booleanComboBox = new QComboBox(this);
    booleanComboBox->addItem("Union");
    booleanComboBox->addItem("Subtraction");
    booleanComboBox->addItem("Intersection");
    booleanComboBox->setFixedSize(100, 25);

    subtractionModeCheckBox = new QCheckBox("A - B", this); // Toggle
    subtractionModeCheckBox->setChecked(true);
    subtractionModeCheckBox->setFixedSize(100, 20);

    applyBooleanButton = new QPushButton("Apply", this);
    applyBooleanButton->setFixedSize(100, 25);
    connect(applyBooleanButton, &QPushButton::clicked, this, &PolygonExtrusionWidget::onApplyBooleanOperation);

    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(booleanComboBox);
    layout->addWidget(subtractionModeCheckBox);
    layout->addWidget(applyBooleanButton);
    layout->setAlignment(Qt::AlignTop | Qt::AlignRight);
    layout->setContentsMargins(10, 10, 10, 10); 
    setLayout(layout);
}

PolygonExtrusionWidget::~PolygonExtrusionWidget() {}

void PolygonExtrusionWidget::initializeGL() {
    initializeOpenGLFunctions();
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glEnable(GL_DEPTH_TEST);
}

void PolygonExtrusionWidget::resizeGL(int w, int h) {
    glViewport(0, 0, w, h);
}

void PolygonExtrusionWidget::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glRotatef(rotationX, 1.0f, 0.0f, 0.0f);
    glRotatef(rotationY, 0.0f, 1.0f, 0.0f);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE_LOOP);

    for (const auto &polygon : polygons) {
        glColor3f(1.5f, 0.5f, 0.5f);
        glBegin(GL_LINE_STRIP);
        for (const auto &p : polygon.points) {
            glVertex3f(p.x(), p.y(), p.z());
        }
        if (polygon.isClosed) {
            glVertex3f(polygon.points.first().x(), polygon.points.first().y(), polygon.points.first().z());
        }
        glEnd();

        if (!polygon.points.isEmpty()) {
            QVector3D first = polygon.points.first();
            glPointSize(8.0f);
            glColor3f(0.0f, 0.0f, 1.0f);
            glBegin(GL_POINTS);
            glVertex3f(first.x(), first.y(), first.z());
            glEnd();
        }

        if (polygon.isClosed) {
            glColor3f(1.5f, 0.5f, 0.5f);
            glBegin(GL_LINES);
            for (int i = 0; i < polygon.points.size(); ++i) {
                int next = (i + 1) % polygon.points.size();
                QVector3D base1 = polygon.points[i];
                QVector3D base2 = polygon.points[next];
                QVector3D top1 = base1 + polygon.normal * extrusionHeight;
                QVector3D top2 = base2 + polygon.normal * extrusionHeight;

                glVertex3f(base1.x(), base1.y(), base1.z());
                glVertex3f(top1.x(), top1.y(), top1.z());

                glVertex3f(top1.x(), top1.y(), top1.z());
                glVertex3f(top2.x(), top2.y(), top2.z());

                glVertex3f(top2.x(), top2.y(), top2.z());
                glVertex3f(base2.x(), base2.y(), base2.z());
            }
            glEnd();
        }
    }
}

void PolygonExtrusionWidget::mousePressEvent(QMouseEvent *event) {
    QVector3D worldPoint = screenToWorld(event->pos());

    if (event->button() == Qt::LeftButton && event->type() == QEvent::MouseButtonDblClick) {
        if (polygonClosed || selectedPolygonIndex == -1 || polygons.isEmpty()) {
            Polygon newPolygon;
            newPolygon.points.append(worldPoint);
            polygons.append(newPolygon);
            selectedPolygonIndex = polygons.size() - 1;
            polygonClosed = false;
            qDebug() << "Started a new polygon. Index:" << selectedPolygonIndex;
        } else {
            if (!polygons[selectedPolygonIndex].points.isEmpty() &&
                isCloseToFirstPoint(worldPoint, polygons[selectedPolygonIndex])) {
                polygons[selectedPolygonIndex].isClosed = true;
                polygons[selectedPolygonIndex].normal = computePolygonNormal(polygons[selectedPolygonIndex].points);
                polygonClosed = true;
                qDebug() << "Polygon closed. Index:" << selectedPolygonIndex;
            } else {
                polygons[selectedPolygonIndex].points.append(worldPoint);
                qDebug() << "Added point to polygon. Index:" << selectedPolygonIndex;
            }
        }
        update();
    } else if (event->button() == Qt::RightButton) {
        rightMousePressed = true;
        lastMousePos = event->pos();
        qDebug() << "Right mouse pressed:" << rightMousePressed;
    }
}

void PolygonExtrusionWidget::mouseMoveEvent(QMouseEvent *event) {
    if (rightMousePressed) {
        float dx = event->pos().x() - lastMousePos.x();
        float dy = event->pos().y() - lastMousePos.y();

        rotationX += dy * 0.5f;
        rotationY += dx * 0.5f;

        lastMousePos = event->pos();
        update();
    } else if (polygonClosed && (event->buttons() & Qt::LeftButton)) {
        float dy = event->pos().y() - height() / 2.0f;
        extrusionHeight = dy * 0.01f;
        qDebug() << "Height:" << extrusionHeight;
        update();
    }
}

void PolygonExtrusionWidget::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::RightButton) {
        rightMousePressed = false;
    }
}

QVector3D PolygonExtrusionWidget::screenToWorld(const QPoint &pos) {
    float x = (2.0f * pos.x()) / width() - 1.0f;
    float y = 1.0f - (2.0f * pos.y()) / height();
    return QVector3D(x, y, 0.0f);
}

bool PolygonExtrusionWidget::isCloseToFirstPoint(const QVector3D &p, const Polygon &polygon) {
    if (polygon.points.isEmpty()) return false;
    return (polygon.points.first() - p).length() < 0.05f;
}

QVector3D PolygonExtrusionWidget::computePolygonNormal(const QVector<QVector3D> &points) {
    QVector3D normal(0, 0, 0);
    for (int i = 0; i < points.size(); ++i) {
        const QVector3D &curr = points[i];
        const QVector3D &next = points[(i + 1) % points.size()];
        normal += QVector3D::crossProduct(curr, next);
    }
    return normal.normalized();
}

bool PolygonExtrusionWidget::pointInPolygon(const QPointF& point, const std::vector<QPointF>& polygon) {
    int count = 0;
    for (size_t i = 0; i < polygon.size(); ++i) {
        QPointF a = polygon[i];
        QPointF b = polygon[(i + 1) % polygon.size()];
        if ((a.y() > point.y()) != (b.y() > point.y())) {
            double intersectX = (b.x() - a.x()) * (point.y() - a.y()) / (b.y() - a.y()) + a.x();
            if (point.x() < intersectX) {
                count++;
            }
        }
    }
    return (count % 2) == 1;
}

bool PolygonExtrusionWidget::segmentsIntersect(const QPointF& a1, const QPointF& a2, const QPointF& b1, const QPointF& b2, QPointF& out) {
    double dx1 = a2.x() - a1.x();
    double dy1 = a2.y() - a1.y();
    double dx2 = b2.x() - b1.x();
    double dy2 = b2.y() - b1.y();

    double denom = dx1 * dy2 - dy1 * dx2;
    if (std::abs(denom) < 1e-8) return false; // parallel

    double ua = ((b1.x() - a1.x()) * dy2 - (b1.y() - a1.y()) * dx2) / denom;
    double ub = ((b1.x() - a1.x()) * dy1 - (b1.y() - a1.y()) * dx1) / denom;

    if (ua < 0 || ua > 1 || ub < 0 || ub > 1) return false;

    out = QPointF(a1.x() + ua * dx1, a1.y() + ua * dy1);
    return true;
}

void PolygonExtrusionWidget::addAndSortIntersections(std::vector<QPointF>& result, const QPointF& point) {
    if (std::find(result.begin(), result.end(), point) == result.end()) {
        result.push_back(point);
    }
}

std::vector<QPointF> PolygonExtrusionWidget::splitEdges(const std::vector<QPointF>& polygon, const std::vector<QPointF>& intersections) {
    std::vector<QPointF> result;
    for (size_t i = 0; i < polygon.size(); ++i) {
        QPointF a = polygon[i];
        QPointF b = polygon[(i + 1) % polygon.size()];
        result.push_back(a);

        for (const auto& inter : intersections) {
            QPointF dummy;
            if (segmentsIntersect(a, b, inter, inter, dummy)) {
                result.push_back(inter);
            }
        }
    }

    std::sort(result.begin(), result.end(), [](const QPointF& p1, const QPointF& p2) {
        return p1.x() < p2.x() || (p1.x() == p2.x() && p1.y() < p2.y());
    });

    return result;
}

std::vector<QPointF> PolygonExtrusionWidget::intersect(const std::vector<QPointF>& poly1, const std::vector<QPointF>& poly2) {
 vector<QPointF> intersections;

    // Find all intersection points
    for (size_t i = 0; i < poly1.size(); ++i) {
        QPointF a1 = poly1[i];
        QPointF a2 = poly1[(i + 1) % poly1.size()];
        for (size_t j = 0; j < poly2.size(); ++j) {
            QPointF b1 = poly2[j];
            QPointF b2 = poly2[(j + 1) % poly2.size()];
            QPointF inter;
            if (segmentsIntersect(a1, a2, b1, b2, inter)) {
                addAndSortIntersections(intersections, inter);
            }
        }
    }

    // Add points from poly1 inside poly2
    for (const auto& point : poly1) {
        if (pointInPolygon(point, poly2)) {
            addAndSortIntersections(intersections, point);
        }
    }

    // Add points from poly2 inside poly1
    for (const auto& point : poly2) {
        if (pointInPolygon(point, poly1)) {
            addAndSortIntersections(intersections, point);
        }
    }

    // Sort points in clockwise order
    QPointF centroid(0, 0);
    for (const auto& point : intersections) {
        centroid += point;
    }
    centroid /= intersections.size();

    sort(intersections.begin(), intersections.end(), [&centroid](const QPointF& a, const QPointF& b) {
        double angleA = atan2(a.y() - centroid.y(), a.x() - centroid.x());
        double angleB = atan2(b.y() - centroid.y(), b.x() - centroid.x());
        return angleA < angleB;
    });

    return intersections;

}

std::vector<QPointF> PolygonExtrusionWidget::unionPolygons(const std::vector<QPointF>& poly1, const std::vector<QPointF>& poly2) {
vector<QPointF> intersections;

    // Find all intersection points
    for (size_t i = 0; i < poly1.size(); ++i) {
        QPointF a1 = poly1[i];
        QPointF a2 = poly1[(i + 1) % poly1.size()];
        for (size_t j = 0; j < poly2.size(); ++j) {
            QPointF b1 = poly2[j];
            QPointF b2 = poly2[(j + 1) % poly2.size()];
            QPointF inter;
            if (segmentsIntersect(a1, a2, b1, b2, inter)) {
                addAndSortIntersections(intersections, inter);
            }
        }
    }

    // Split edges at intersection points
    vector<QPointF> splitPoly1 = splitEdges(poly1, intersections);
    vector<QPointF> splitPoly2 = splitEdges(poly2, intersections);

    // Construct union polygon
    vector<QPointF> result;

    // Add points from poly1 that are outside poly2
    for (const auto& point : splitPoly1) {
        if (!pointInPolygon(point, poly2)) {
            result.push_back(point);
        }
    }

    // Add points from poly2 that are outside poly1
    for (const auto& point : splitPoly2) {
        if (!pointInPolygon(point, poly1)) {
            result.push_back(point);
        }
    }

    // Add intersection points
    for (const auto& inter : intersections) {
        result.push_back(inter);
    }

    // Sort points in clockwise order
    QPointF centroid(0, 0);
    for (const auto& point : result) {
        centroid += point;
    }
    centroid /= result.size();

    sort(result.begin(), result.end(), [&centroid](const QPointF& a, const QPointF& b) {
        double angleA = atan2(a.y() - centroid.y(), a.x() - centroid.x());
        double angleB = atan2(b.y() - centroid.y(), b.x() - centroid.x());
        return angleA < angleB;
    });

    // Remove duplicate points
    result.erase(unique(result.begin(), result.end(), [](const QPointF& a, const QPointF& b) {
        return qFuzzyCompare(a.x(), b.x()) && qFuzzyCompare(a.y(), b.y());
    }), result.end());

    return result;
}

std::vector<QPointF> PolygonExtrusionWidget::subtractPolygons(const std::vector<QPointF>& poly1, const std::vector<QPointF>& poly2) {
    std::vector<QPointF> intersections;

    // Find all intersection points
    for (size_t i = 0; i < poly1.size(); ++i) {
        QPointF a1 = poly1[i];
        QPointF a2 = poly1[(i + 1) % poly1.size()];
        for (size_t j = 0; j < poly2.size(); ++j) {
            QPointF b1 = poly2[j];
            QPointF b2 = poly2[(j + 1) % poly2.size()];
            QPointF inter;
            if (segmentsIntersect(a1, a2, b1, b2, inter)) {
                addAndSortIntersections(intersections, inter);
            }
        }
    }

    // Split edges at intersection points
    std::vector<QPointF> splitPoly1 = splitEdges(poly1, intersections);

    // Construct the subtraction result
    std::vector<QPointF> result;

    // Add points from poly1 that are outside poly2
    for (const auto& point : splitPoly1) {
        if (!pointInPolygon(point, poly2)) {
            result.push_back(point);
        }
    }

    // Add intersection points
    for (const auto& inter : intersections) {
        result.push_back(inter);
    }

    // Sort points in clockwise order
    QPointF centroid(0, 0);
    for (const auto& point : result) {
        centroid += point;
    }
    centroid /= result.size();

    std::sort(result.begin(), result.end(), [&centroid](const QPointF& a, const QPointF& b) {
        double angleA = atan2(a.y() - centroid.y(), a.x() - centroid.x());
        double angleB = atan2(b.y() - centroid.y(), b.x() - centroid.x());
        return angleA < angleB;
    });

    // Remove duplicate points
    result.erase(std::unique(result.begin(), result.end(), [](const QPointF& a, const QPointF& b) {
        return qFuzzyCompare(a.x(), b.x()) && qFuzzyCompare(a.y(), b.y());
    }), result.end());

    return result;
}

void PolygonExtrusionWidget::onApplyBooleanOperation() {
    if (polygons.size() < 2) {
        qDebug() << "At least two shapes are required.";
        return;
    }

    QString operation = booleanComboBox->currentText();
    Polygon &A = polygons[0];
    Polygon &B = polygons[1];

    // Convert shapes to QPainterPath
    QPainterPath pathA = convertToPath(A);
    QPainterPath pathB = convertToPath(B);
    QPainterPath resultPath;

    if (operation == "Union") {
        resultPath = pathA.united(pathB);
    } else if (operation == "Intersection") {
        resultPath = pathA.intersected(pathB);
    } else if (operation == "Subtraction") {
        resultPath = pathA.subtracted(pathB);
    }

    // Convert resultPath back to Polygon (for union/intersection)
    QVector<QVector3D> resultPoints;
    for (int i = 0; i < resultPath.elementCount(); ++i) {
        QPainterPath::Element elem = resultPath.elementAt(i);
        if (elem.isMoveTo() || elem.isLineTo()) {
            resultPoints.append(QVector3D(elem.x, elem.y, 0.0f));
        }
    }

    if (!resultPoints.isEmpty()) {
        A.points = resultPoints;
        A.isClosed = true;
        A.normal = computePolygonNormal(A.points);
    } else {
        qDebug() << "Operation failed or returned empty.";
    }

    // Remove the second shape (B) from the polygons list
    if (polygons.size() > 1) {
        polygons.removeAt(1);
    }

    // Trigger a repaint
    update();
}

QPainterPath PolygonExtrusionWidget::convertToPath(const Polygon &shape) {
    QPainterPath path;

    // if (shape.isArc) {
    //     // Convert arc to QPainterPath
    //     QRectF rect(shape.arcCenter.x() - shape.arcRadius, shape.arcCenter.y() - shape.arcRadius,
    //                 shape.arcRadius * 2, shape.arcRadius * 2);
    //     path.arcTo(rect, shape.arcStartAngle, shape.arcEndAngle - shape.arcStartAngle);
    // } else if (shape.isCircle) {
    //     // Convert circle to QPainterPath
    //     QRectF rect(shape.circleCenter.x() - shape.circleRadius, shape.circleCenter.y() - shape.circleRadius,
    //                 shape.circleRadius * 2, shape.circleRadius * 2);
    //     path.addEllipse(rect);
    //}
     if (!shape.points.isEmpty()) {
        // Convert polygon to QPainterPath
        path.moveTo(shape.points.first().toPointF());
        for (const auto &point : shape.points) {
            path.lineTo(point.toPointF());
        }
        if (shape.isClosed) {
            path.closeSubpath();
        }
    }

    return path;
}
