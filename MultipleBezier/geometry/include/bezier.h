#ifndef BEZIER_H
#define BEZIER_H

#include "Shape3D.h"
#include <vector>
#include <QPointF>

class Bezier : public Shape3D {
public:
    Bezier(const std::vector<QPointF> &controlPoints, int numInterpolatingPoints);
    void draw() override;
    void updateControlPoint(int index, const QPointF &newPoint);
    int findControlPoint(const QPointF &mousePos, float radius = 10.0f);
    std::vector<QPointF> findIntersections(const Bezier& other) const;
    std::vector<QPointF> evaluatePoints(int samples) const;

private:
    std::vector<QPointF> controlPoints;
    std::vector<QPointF> curvePoints;
    int numInterpolatingPoints;

    void calculateCurve();
    QPointF deCasteljau(const std::vector<QPointF> &points, float t) const;
    QPointF evaluate(float t) const; // Computes a point on the curve for parameter t
};

#endif // BEZIER1_H