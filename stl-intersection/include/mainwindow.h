#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>

class GLWidget; // Forward declaration

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onImportSTL();
    void onFindIntersection();

private:
    QPushButton *importButton;
    QPushButton *intersectionButton;
    GLWidget *glWidget;
};

#endif