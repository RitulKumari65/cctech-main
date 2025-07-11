#pragma once

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include "rectangle.h"

class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    GLWidget(QWidget *parent = nullptr);
    void setOperation(const QString &op);

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

private:
    Rectangle rect1;
    Rectangle rect2;
    QString operation;
    void performBooleanOperation();
};
