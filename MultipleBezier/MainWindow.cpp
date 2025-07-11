#include "MainWindow.h"
#include "MyOpenGLWidget.h"
#include "Bezier.h"
#include "Booleanoperations/booleanoperations.h"
// #include "sphere1.h"

#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QInputDialog>
#include <vector>
#include <QPointF>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) {
    openGLWidget = new MyOpenGLWidget(this);
    setCentralWidget(openGLWidget);
    booleanWidget = new booleanoperations(this);
    booleanWidget->hide();
    createMenus();
}

MainWindow::~MainWindow() {}

void MainWindow::createMenus() {
    QMenu *fileMenu = menuBar()->addMenu("File");
    fileMenu->addAction("New Project");
    fileMenu->addAction("Open Saved Project");
    fileMenu->addAction("Save Project");
    fileMenu->addAction("File Conversions");

    QMenu *shapeMenu = menuBar()->addMenu("3D_Shapes");

    QAction *bezierAct = shapeMenu->addAction("Bezier");
    QAction *BooleanAct = shapeMenu->addAction("Boolean");

    connect(bezierAct, &QAction::triggered, this, &MainWindow::promptBezierPoints);
    connect(BooleanAct, &QAction::triggered, this, &MainWindow::showBooleanWidget);
}

void MainWindow::promptBezierPoints() {
    bool ok;
    int numPoints = QInputDialog::getInt(this, "Number of Control Points", "Enter number of control points:", 4, 2, 10, 1, &ok);
    if (!ok) return;

    std::vector<QPointF> controlPoints;
    for (int i = 0; i < numPoints; ++i) {
        float x = QInputDialog::getDouble(this, QString("Control Point %1 X").arg(i + 1), "Enter X coordinate:", 0, -1000, 1000, 2, &ok);
        if (!ok) return;
        float y = QInputDialog::getDouble(this, QString("Control Point %1 Y").arg(i + 1), "Enter Y coordinate:", 0, -1000, 1000, 2, &ok);
        if (!ok) return;
        controlPoints.emplace_back(x, y);
    }

    int numInterpolatingPoints = QInputDialog::getInt(this, "Number of Interpolating Points", "Enter number of interpolating points:", 100, 10, 1000, 10, &ok);
    if (!ok) return;

    openGLWidget->addBezierCurve(new Bezier(controlPoints, numInterpolatingPoints));
}

void MainWindow::showBooleanWidget() {
    if (booleanWidget) {
        booleanWidget->show();
        booleanWidget->raise();
        booleanWidget->activateWindow();
    }
}