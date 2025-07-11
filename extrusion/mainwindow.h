#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class GLWidget;
class QPushButton;
class PolygonExtrusionWidget; // Forward declaration

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    GLWidget *glWidget;
    QPushButton *intersectBtn;
    QPushButton *unionBtn;
    QPushButton *diffBtn;
    QPushButton *extrusionBtn; // New button
    PolygonExtrusionWidget *extrusionWidget; // New member

private slots:
    void setIntersection();
    void setUnion();
    void setDifference();
    void openExtrusionWindow(); // New slot
};

#endif // MAINWINDOW_H
