#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class MyOpenGLWidget;
class booleanoperations;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void createMenus();
    void promptBezierPoints();
    void openBooleanWindow(); // keep for compatibility
    void showBooleanWidget();
    MyOpenGLWidget *openGLWidget;
    booleanoperations *booleanWidget = nullptr;
};

#endif // MAINWINDOW_H
