// mainwindow.h
#pragma once

#include <QMainWindow>
#include "sketcherwidget.h"

class QPushButton;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    SketcherWidget* openGLWidget;
    QPushButton* createShapeButton;
    QPushButton* unionButton;
    QPushButton* diffButton;
    QPushButton* intersectButton;

    void setupUI();
};
