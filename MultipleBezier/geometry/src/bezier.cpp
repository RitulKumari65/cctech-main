#include "bezier.h"
#include <GL/gl.h>
#include <cmath>
#include <QDebug> // For debugging
#include <algorithm>

Bezier::Bezier(const std::vector<QPointF> &controlPoints, int numInterpolatingPoints)
    : controlPoints(controlPoints), numInterpolatingPoints(numInterpolatingPoints) {
    calculateCurve();
}

void Bezier::draw() {
    // Draw control points
    glPointSize(8.0f);
    glBegin(GL_POINTS);
    glColor3f(1.0f, 0.0f, 0.0f); // Red for control points
    for (const auto &point : controlPoints) {
        glVertex2f(point.x(), point.y());
    }
    glEnd();

    // Draw curve
    

    glLineWidth(3.0f);
    glBegin(GL_LINE_STRIP);
    glColor3f(1.0f, 0.5f, 0.5f); // pink
    for (const auto &point : curvePoints) {
        glVertex2f(point.x()+0.02, point.y()-0.02);
    }
    glEnd();

    glLineWidth(3.0f);
    glBegin(GL_LINE_STRIP);
    glColor3f(1.0f, 0.5f, 0.5f); // pink
    for (const auto &point : curvePoints) {
        glVertex2f(point.x()-0.02, point.y()+0.02);
    }
    glEnd();

    glLineWidth(2.0f);
    glBegin(GL_LINE_STRIP);
    glColor3f(1.0f, 1.0f, 1.0f); // white
    for (const auto &point : curvePoints) {
        glVertex2f(point.x(), point.y());
    }
    glEnd();

    
}

void Bezier::updateControlPoint(int index, const QPointF &newPoint) {
    if (index >= 0 && index < static_cast<int>(controlPoints.size())) {
        controlPoints[index] = newPoint;
        calculateCurve();
    }
}

int Bezier::findControlPoint(const QPointF &mousePos, float radius) {
    for (int i = 0; i < static_cast<int>(controlPoints.size()); ++i) {
        if (std::hypot(controlPoints[i].x() - mousePos.x(), controlPoints[i].y() - mousePos.y()) <= radius) {
            return i;     
        }
    }
    return -1;
}

void Bezier::calculateCurve() {
    curvePoints.clear();
    for (int i = 0; i <= numInterpolatingPoints; ++i) {
        float t = static_cast<float>(i) / numInterpolatingPoints;
        curvePoints.push_back(deCasteljau(controlPoints, t));
    }
    qDebug() << "Curve points calculated:" << curvePoints.size();
}

QPointF Bezier::deCasteljau(const std::vector<QPointF> &points, float t) const {
    if (points.size() == 1) {
        return points[0];
    }
    std::vector<QPointF> nextPoints;
    for (size_t i = 0; i < points.size() - 1; ++i) {
        float x = (1 - t) * points[i].x() + t * points[i + 1].x();
        float y = (1 - t) * points[i].y() + t * points[i + 1].y();
        nextPoints.emplace_back(x, y);
    }
    return deCasteljau(nextPoints, t);
}

std::vector<QPointF> Bezier::findIntersections(const Bezier& other) const {
    std::vector<QPointF> intersections;

    // Increase sampling resolution for better accuracy
    int samples = 500;
    std::vector<QPointF> thisPoints = this->evaluatePoints(samples);
    std::vector<QPointF> otherPoints = other.evaluatePoints(samples);

    // Check for intersections
    for (const auto& p1 : thisPoints) {
        for (const auto& p2 : otherPoints) {
            if (std::hypot(p1.x() - p2.x(), p1.y() - p2.y()) < 0.05) { // Adjusted threshold for intersection
                intersections.push_back(p1);
            }
        }
    }

    return intersections;
}

std::vector<QPointF> Bezier::evaluatePoints(int samples) const {
    std::vector<QPointF> points;
    for (int i = 0; i <= samples; ++i) {
        float t = static_cast<float>(i) / samples;
        QPointF point = evaluate(t); // Assuming `evaluate` computes a point on the curve for parameter t
        points.push_back(point);
    }
    return points;
}

QPointF Bezier::evaluate(float t) const {
    return deCasteljau(controlPoints, t);
}