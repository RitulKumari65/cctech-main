// robotwidget.h
 
 
#ifndef ROBOTWIDGET_H
#define ROBOTWIDGET_H
 
#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QMatrix4x4>
#include <QTimer>
 
class RobotWidget : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core {
    Q_OBJECT
public:
    RobotWidget(QWidget *parent = nullptr);
    ~RobotWidget();
 
protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;
 
private:
    void drawCube(const QMatrix4x4 &modelMatrix);
    void drawRobot();
 
    QOpenGLShaderProgram shader;
    QOpenGLBuffer vbo;
    QOpenGLVertexArrayObject vao;
    QMatrix4x4 projection;
    QTimer timer;
    float angle = 0.0f;
 
private slots:
    void updateAnimation();
};
 
#endif // ROBOTWIDGET_H
 