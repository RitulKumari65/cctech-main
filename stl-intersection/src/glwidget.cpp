#include "glwidget.h"
#include "intersection.h"
#include <QOpenGLFunctions>
#include <QOpenGLWidget>
#include <QColor>
#include <QDebug>
 
GLWidget::GLWidget(QWidget *parent)
    : QOpenGLWidget(parent)
{
}
 
GLWidget::~GLWidget()
{
}
 
void GLWidget::initializeGL()
{
    initializeOpenGLFunctions();
    glEnable(GL_DEPTH_TEST); // Enable depth testing for 3D
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
}
 
void GLWidget::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
    float aspect = float(w) / float(h ? h : 1);
    projection.setToIdentity();
    projection.perspective(45.0f, aspect, 0.1f, 100.0f);
    // No gluPerspective, no glMatrixMode/glLoadIdentity for projection
}
 
void GLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    view.setToIdentity();
    view.translate(0.0f, 0.0f, -3.0f * zoom);
    model.setToIdentity();
    model.rotate(rotationX, 1.0f, 0.0f, 0.0f);
    model.rotate(rotationY, 0.0f, 1.0f, 0.0f);
    QMatrix4x4 mvp = projection * view * model;
    glLoadMatrixf(mvp.constData());
 
    // Draw trianglesA in blue
    glColor3f(0.2f, 0.4f, 1.0f);
    for (const auto& tri : trianglesA) {
        glBegin(GL_LINE_LOOP);
        glVertex3f(tri.p1.x, tri.p1.y, tri.p1.z);
        glVertex3f(tri.p2.x, tri.p2.y, tri.p2.z);
        glVertex3f(tri.p3.x, tri.p3.y, tri.p3.z);
        glEnd();
    }
 
    // Draw trianglesB in red
    glColor3f(1.0f, 0.2f, 0.2f);
    for (const auto& tri : trianglesB) {
        glBegin(GL_LINE_LOOP);
        glVertex3f(tri.p1.x, tri.p1.y, tri.p1.z);
        glVertex3f(tri.p2.x, tri.p2.y, tri.p2.z);
        glVertex3f(tri.p3.x, tri.p3.y, tri.p3.z);
        glEnd();
    }
 
    // // Draw intersection points in green
    // glColor3f(0.0f, 0.8f, 0.0f);
    // glPointSize(8.0f);
    // std::vector<Point> intersectionPoints;
    // for (const auto& triA : trianglesA) {
    //     for (const auto& triB : trianglesB) {
    //         const Point* t1_pts[3] = { &triA.p1, &triA.p2, &triA.p3 };
    //         const Point* t2_pts[3] = { &triB.p1, &triB.p2, &triB.p3 };
    //         for (int i = 0; i < 3; ++i) {
    //             const Point& a1 = *t1_pts[i];
    //             const Point& a2 = *t1_pts[(i+1)%3];
    //             for (int j = 0; j < 3; ++j) {
    //                 const Point& b1 = *t2_pts[j];
    //                 const Point& b2 = *t2_pts[(j+1)%3];
    //                 // Use the same segmentsIntersect2D logic as in intersection.cpp
    //                 auto orientation = [](const Point& p, const Point& q, const Point& r) {
    //                     float val = (q.y - p.y) * (r.x - q.x) -
    //                                 (q.x - p.x) * (r.y - q.y);
    //                     if (val == 0) return 0;
    //                     return (val > 0) ? 1 : 2;
    //                 };
    //                 auto onSegment = [](const Point& p, const Point& q, const Point& r) {
    //                     return q.x <= std::max(p.x, r.x) && q.x >= std::min(p.x, r.x) &&
    //                            q.y <= std::max(p.y, r.y) && q.y >= std::min(p.y, r.y);
    //                 };
    //                 auto segmentsIntersect2D = [&](const Point& p1, const Point& q1, const Point& p2, const Point& q2, Point& intersection) {
    //                     int o1 = orientation(p1, q1, p2);
    //                     int o2 = orientation(p1, q1, q2);
    //                     int o3 = orientation(p2, q2, p1);
    //                     int o4 = orientation(p2, q2, q1);
    //                     if (o1 != o2 && o3 != o4) {
    //                         // Compute intersection point for two segments (in 2D)
    //                         float x1 = p1.x, y1 = p1.y;
    //                         float x2 = q1.x, y2 = q1.y;
    //                         float x3 = p2.x, y3 = p2.y;
    //                         float x4 = q2.x, y4 = q2.y;
    //                         float denom = (x1-x2)*(y3-y4)-(y1-y2)*(x3-x4);
    //                         if (denom == 0) return false;
    //                         float px = ((x1*y2-y1*x2)*(x3-x4)-(x1-x2)*(x3*y4-y3*x4))/denom;
    //                         float py = ((x1*y2-y1*x2)*(y3-y4)-(y1-y2)*(x3*y4-y3*x4))/denom;
    //                         intersection = Point(px, py, 0);
    //                         return true;
    //                     }
    //                     // Special cases: colinear and overlapping
    //                     // Not handled for intersection point, only for boolean
    //                     return false;
    //                 };
    //                 Point intersection;
    //                 if (segmentsIntersect2D(a1, a2, b1, b2, intersection)) {
    //                     glBegin(GL_POINTS);
    //                     glVertex2f(intersection.x, intersection.y);
    //                     glEnd();
    //                     intersectionPoints.push_back(intersection);
    //                 }
    //             }
    //         }
    //     }
    // }
    // glPointSize(1.0f);
 
    // Connect all intersection points with each other
    // if (intersectionPoints.size() >= 2) {
    //     glColor3f(0.0f, 0.8f, 0.0f);
    //     glLineWidth(3.0f);
    //     for (size_t i = 0; i < intersectionPoints.size(); ++i) {
    //         for (size_t j = i + 1; j < intersectionPoints.size(); ++j) {
    //             glBegin(GL_LINES);
    //             glVertex2f(intersectionPoints[i].x, intersectionPoints[i].y);
    //             glVertex2f(intersectionPoints[j].x, intersectionPoints[j].y);
    //             glEnd();
    //         }
    //     }
    //     glLineWidth(1.0f);
    // }
 
    // Draw triangle edge segments that are inside the other triangle (magenta)
    // auto pointInTriangle = [](const Point& p, const Triangle& t) {
    //     // Barycentric technique
    //     float x = p.x, y = p.y;
    //     float x1 = t.p1.x, y1 = t.p1.y;
    //     float x2 = t.p2.x, y2 = t.p2.y;
    //     float x3 = t.p3.x, y3 = t.p3.y;
    // // For each edge of trianglesA, check if inside trianglesB
    // for (const auto& triA : trianglesA) {
    //     const Point* pts[3] = { &triA.p1, &triA.p2, &triA.p3 };
    //     for (int i = 0; i < 3; ++i) {
    //         const Point& p0 = *pts[i];
    //         const Point& p1 = *pts[(i+1)%3];
    //         for (const auto& triB : trianglesB) {
    //             glBegin(GL_LINE_STRIP);
    //             for (float t = 0; t <= 1.0f; t += step) {
    //                 float x = p0.x + t * (p1.x - p0.x);
    //                 float y = p0.y + t * (p1.y - p0.y);
    //                 Point pt(x, y, 0);
    //                 if (pointInTriangle(pt, triB)) {
    //                     glVertex2f(x, y);
    //                 }
    //             }
    //             glEnd();
    //         }
    //     }
    // }
    // // For each edge of trianglesB, check if inside trianglesA
    // for (const auto& triB : trianglesB) {
    //     const Point* pts[3] = { &triB.p1, &triB.p2, &triB.p3 };
    //     for (int i = 0; i < 3; ++i) {
    //         const Point& p0 = *pts[i];
    //         const Point& p1 = *pts[(i+1)%3];
    //         for (const auto& triA : trianglesA) {
    //             glBegin(GL_LINE_STRIP);
    //             for (float t = 0; t <= 1.0f; t += step) {
    //                 float x = p0.x + t * (p1.x - p0.x);
    //                 float y = p0.y + t * (p1.y - p0.y);
    //                 Point pt(x, y, 0);
    //                 if (pointInTriangle(pt, triA)) {
    //                     glVertex2f(x, y);
    //                 }
    //             }
    //             glEnd();
    //         }
    //     }
    // }
 
    // For each pair of triangles, handle coplanar and non-coplanar cases
    std::vector<std::pair<Point, Point>> intersectionSegments;
    for (const auto& triA : trianglesA) {
        for (const auto& triB : trianglesB) {
            if (trianglesCoplanar(triA, triB)) {
                // Coplanar: collect intersection points along edges
                auto pointInTriangle = [](const Point& p, const Triangle& t) {
                    float x = p.x, y = p.y;
                    float x1 = t.p1.x, y1 = t.p1.y;
                    float x2 = t.p2.x, y2 = t.p2.y;
                    float x3 = t.p3.x, y3 = t.p3.y;
                    float denom = (y2 - y3)*(x1 - x3) + (x3 - x2)*(y1 - y3);
                    float a = ((y2 - y3)*(x - x3) + (x3 - x2)*(y - y3)) / denom;
                    float b = ((y3 - y1)*(x - x3) + (x1 - x3)*(y - y3)) / denom;
                    float c = 1.0f - a - b;
                    return a >= 0 && b >= 0 && c >= 0 && a <= 1 && b <= 1 && c <= 1;
                };
                float step = 0.01f;
                // For each edge of triA, check if inside triB
                const Point* ptsA[3] = { &triA.p1, &triA.p2, &triA.p3 };
                for (int i = 0; i < 3; ++i) {
                    const Point& p0 = *ptsA[i];
                    const Point& p1 = *ptsA[(i+1)%3];
                    std::vector<Point> segment;
                    for (float t = 0; t <= 1.0f; t += step) {
                        float x = p0.x + t * (p1.x - p0.x);
                        float y = p0.y + t * (p1.y - p0.y);
                        Point pt(x, y, 0);
                        if (pointInTriangle(pt, triB)) {
                            segment.push_back(pt);
                        } else if (!segment.empty()) {
                            // End of a segment inside
                            if (segment.size() > 1)
                                intersectionSegments.emplace_back(segment.front(), segment.back());
                            segment.clear();
                        }
                    }
                    if (segment.size() > 1)
                        intersectionSegments.emplace_back(segment.front(), segment.back());
                }
                // For each edge of triB, check if inside triA
                const Point* ptsB[3] = { &triB.p1, &triB.p2, &triB.p3 };
                for (int i = 0; i < 3; ++i) {
                    const Point& p0 = *ptsB[i];
                    const Point& p1 = *ptsB[(i+1)%3];
                    std::vector<Point> segment;
                    for (float t = 0; t <= 1.0f; t += step) {
                        float x = p0.x + t * (p1.x - p0.x);
                        float y = p0.y + t * (p1.y - p0.y);
                        Point pt(x, y, 0);
                        if (pointInTriangle(pt, triA)) {
                            segment.push_back(pt);
                        } else if (!segment.empty()) {
                            if (segment.size() > 1)
                                intersectionSegments.emplace_back(segment.front(), segment.back());
                            segment.clear();
                        }
                    }
                    if (segment.size() > 1)
                        intersectionSegments.emplace_back(segment.front(), segment.back());
                }
            } else {
                // Non-coplanar: collect intersection segment endpoints as a line
                Point segA, segB;
                if (triangleTriangleIntersectionSegment(triA, triB, segA, segB)) {
                    intersectionSegments.emplace_back(segA, segB);
                }
            }
        }
    }
    // Draw all intersection segments as lines in white
    glColor3f(1.0f, 1.0f, 1.0f);
    glLineWidth(3.0f);
    for (const auto& seg : intersectionSegments) {
        glBegin(GL_LINES);
        glVertex3f(seg.first.x, seg.first.y, seg.first.z);
        glVertex3f(seg.second.x, seg.second.y, seg.second.z);
        glEnd();
    }
    glLineWidth(1.0f);
}
 
void GLWidget::mousePressEvent(QMouseEvent *event)
{
    lastMousePos = event->pos();
}
 
void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    int dx = event->x() - lastMousePos.x();
    int dy = event->y() - lastMousePos.y();
    if (event->buttons() & Qt::LeftButton) {
        rotationX += dy;
        rotationY += dx;
        update();
    }
    lastMousePos = event->pos();
}
 
void GLWidget::wheelEvent(QWheelEvent *event)
{
    if (event->angleDelta().y() > 0)
        zoom *= 0.9f;
    else
        zoom *= 1.1f;
    update();
}
 
 
 
 