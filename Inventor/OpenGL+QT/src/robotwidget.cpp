#include "robotwidget.h"
#include <cmath>
 
static const float cubeVertices[] = {
    // Each line represents two vertices (wireframe lines)
    // Front face
    -0.5f, -0.5f,  0.5f,   0.5f, -0.5f,  0.5f,
     0.5f, -0.5f,  0.5f,   0.5f,  0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,  -0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,  -0.5f, -0.5f,  0.5f,
 
    // Back face
    -0.5f, -0.5f, -0.5f,   0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,   0.5f,  0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,  -0.5f,  0.5f, -0.5f,
    -0.5f,  0.5f, -0.5f,  -0.5f, -0.5f, -0.5f,
 
    // Connectors
    -0.5f, -0.5f,  0.5f,  -0.5f, -0.5f, -0.5f,
     0.5f, -0.5f,  0.5f,   0.5f, -0.5f, -0.5f,
     0.5f,  0.5f,  0.5f,   0.5f,  0.5f, -0.5f,
    -0.5f,  0.5f,  0.5f,  -0.5f,  0.5f, -0.5f,
};
 
RobotWidget::RobotWidget(QWidget *parent) : QOpenGLWidget(parent), vbo(QOpenGLBuffer::VertexBuffer) {
    connect(&timer, &QTimer::timeout, this, &RobotWidget::updateAnimation);
    timer.start(30);
}
 
RobotWidget::~RobotWidget() {
    makeCurrent();
    vao.destroy();
    vbo.destroy();
    shader.removeAllShaders();
    doneCurrent();
}
 
void RobotWidget::initializeGL() {
    initializeOpenGLFunctions();
    glEnable(GL_DEPTH_TEST);
 
    shader.addShaderFromSourceCode(QOpenGLShader::Vertex,
        R"(#version 330 core
           layout(location = 0) in vec3 position;
           uniform mat4 mvp;
           void main() { gl_Position = mvp * vec4(position, 1.0); })");
    shader.addShaderFromSourceCode(QOpenGLShader::Fragment,
        R"(#version 330 core
           out vec4 fragColor;
           void main() { fragColor = vec4(1.0, 1.0, 1.0, 1.0); })");
    shader.link();
 
    vao.create();
    vao.bind();
 
    vbo.create();
    vbo.bind();
    vbo.allocate(cubeVertices, sizeof(cubeVertices));
    shader.enableAttributeArray(0);
    shader.setAttributeBuffer(0, GL_FLOAT, 0, 3);
 
    vao.release();
    vbo.release();
}
 
void RobotWidget::resizeGL(int w, int h) {
    projection.setToIdentity();
    projection.perspective(45.0f, float(w) / h, 0.1f, 100.0f);
}
 
void RobotWidget::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    QMatrix4x4 view;
    view.lookAt({0, 5, 20}, {0, 0, 0}, {0, 1, 0});
 
    shader.bind();
    vao.bind();
 
    drawRobot();
 
    vao.release();
    shader.release();
}
 
void RobotWidget::drawCube(const QMatrix4x4 &model) {
    QMatrix4x4 view;
    view.lookAt({0, 5, 20}, {0, 0, 0}, {0, 1, 0});
    QMatrix4x4 mvp = projection * view * model;
    shader.setUniformValue("mvp", mvp);
    glDrawArrays(GL_LINES, 0, 24);
}
 
void RobotWidget::drawRobot() {
    QMatrix4x4 model;
 
    // Body
    model.setToIdentity();
    model.scale(2.0f, 3.0f, 1.0f);
    drawCube(model);
 
    // Head
    model.setToIdentity();
    model.translate(0.0f, 2.5f, 0.0f);
    drawCube(model);
 
    // Left Arm
    model.setToIdentity();
    model.translate(-1.5f, 1.0f, 0.0f);
    model.rotate(std::sin(angle) * 45, 1.0f, 0.0f, 0.0f);
    model.translate(0.0f, -1.0f, 0.0f);
    model.scale(0.5f, 2.0f, 0.5f);
    drawCube(model);
 
    // Right Arm
    model.setToIdentity();
    model.translate(1.5f, 1.0f, 0.0f);
    model.rotate(-std::sin(angle) * 45, 1.0f, 0.0f, 0.0f);
    model.translate(0.0f, -1.0f, 0.0f);
    model.scale(0.5f, 2.0f, 0.5f);
    drawCube(model);
 
    // Left Leg
    model.setToIdentity();
    model.translate(-0.5f, -2.0f, 0.0f);
    model.rotate(-std::sin(angle) * 45, 1.0f, 0.0f, 0.0f);
    model.translate(0.0f, -1.0f, 0.0f);
    model.scale(0.5f, 2.0f, 0.5f);
    drawCube(model);
 
    // Right Leg
    model.setToIdentity();
    model.translate(0.5f, -2.0f, 0.0f);
    model.rotate(std::sin(angle) * 45, 1.0f, 0.0f, 0.0f);
    model.translate(0.0f, -1.0f, 0.0f);
    model.scale(0.5f, 2.0f, 0.5f);
    drawCube(model);
}
 
void RobotWidget::updateAnimation() {
    angle += 0.1f;
    if (angle > 2 * M_PI) angle -= 2 * M_PI;
    update();
}