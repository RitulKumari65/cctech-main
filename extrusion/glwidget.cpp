#include "glwidget.h"
#include <QOpenGLFunctions_3_3_Core>
#include <QMatrix4x4>
#include <algorithm> // For std::max and std::min

GLWidget::GLWidget(QWidget *parent)
    : QOpenGLWidget(parent),
      rect1(0.3f, 0.3f, 0.2f, 0.2f), // Positioned at (0.3, 0.3)
      rect2(0.4f, 0.4f, 0.2f, 0.2f)  // Positioned at (0.4, 0.4)
{
}

void GLWidget::initializeGL()
{
    initializeOpenGLFunctions();

    glClearColor(0, 0, 0, 0);
    QMatrix4x4 projection;
    projection.ortho(0.0f, 1.0f, 0.0f, 1.0f, -1.0f, 1.0f); // Orthographic projection
    glLoadMatrixf(projection.constData());
}

void GLWidget::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
}

void GLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT);
    rect1.draw();
    rect2.draw();

    performBooleanOperation();
}

void GLWidget::setOperation(const QString &op)
{
    operation = op;
    update();
}

void drawPolygon(const std::vector<std::vector<float>> &vertices, float r, float g, float b)
{
    glColor3f(r, g, b);
    glBegin(GL_POLYGON);
    for (const auto &v : vertices)
        glVertex2f(v[0], v[1]);
    glEnd();
}

// Helper to get rectangle bounds
void getRectBounds(const Rectangle &rect, float &xmin, float &xmax, float &ymin, float &ymax)
{
    xmin = rect.getX() - rect.getLength() / 2.0f;
    xmax = rect.getX() + rect.getLength() / 2.0f;
    ymin = rect.getY() - rect.getWidth() / 2.0f;
    ymax = rect.getY() + rect.getWidth() / 2.0f;
}

// Perform the boolean operations and plot the result
void GLWidget::performBooleanOperation()
{
    float x1_min, x1_max, y1_min, y1_max;
    float x2_min, x2_max, y2_min, y2_max;
    getRectBounds(rect1, x1_min, x1_max, y1_min, y1_max);
    getRectBounds(rect2, x2_min, x2_max, y2_min, y2_max);

    // Vertices for intersection
    float ix_min = std::max(x1_min, x2_min);
    float ix_max = std::min(x1_max, x2_max);
    float iy_min = std::max(y1_min, y2_min);
    float iy_max = std::min(y1_max, y2_max);

    if (operation == "intersection") {
        if (ix_min < ix_max && iy_min < iy_max) {
            std::vector<std::vector<float>> interVertices = {
                {ix_min, iy_min},
                {ix_max, iy_min},
                {ix_max, iy_max},
                {ix_min, iy_max}
            };
            drawPolygon(interVertices, 1.0f, 0.0f, 0.5f); // pink
        }
    } else if (operation == "union") {
        // Union: draw both rectangles filled
        drawPolygon(rect1.vertices, 1.0f, 0.0f, 0.5f); // Blue
        drawPolygon(rect2.vertices, 1.0f, 0.0f, 0.5f); // Blue
    } else if (operation == "difference") {
        // Difference: draw rect1, then "erase" intersection
        drawPolygon(rect1.vertices, 1.0f, 0.0f, 0.5f); // Orange
        if (ix_min < ix_max && iy_min < iy_max) {
            std::vector<std::vector<float>> interVertices = {
                {ix_min, iy_min},
                {ix_max, iy_min},
                {ix_max, iy_max},
                {ix_min, iy_max}
            };
            drawPolygon(interVertices, 0.0f, 0.0f, 0.0f); // White "erase"
        }
    }
}
