#include "mainwindow.h"
#include "glwidget.h"
#include "stlparser.h"
#include <QVBoxLayout>
#include <QWidget>
#include <QFileDialog>
#include <QMessageBox>
#include <QString>
#include <vector>
#include <string>
#include "intersection.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(centralWidget);

    importButton = new QPushButton("Import STL", this);
    intersectionButton = new QPushButton("Find Intersection", this);
    glWidget = new GLWidget(this);

    layout->addWidget(importButton);
    layout->addWidget(intersectionButton);
    layout->addWidget(glWidget, 1); // Stretch factor for OpenGL area

    setCentralWidget(centralWidget);

    connect(importButton, &QPushButton::clicked, this, &MainWindow::onImportSTL);
    connect(intersectionButton, &QPushButton::clicked, this, &MainWindow::onFindIntersection);
}

MainWindow::~MainWindow() {}

void MainWindow::onImportSTL()
{
    static bool loadToA = true;
    QString fileName = QFileDialog::getOpenFileName(this, "Open STL File", "", "STL Files (*.stl)");
    if (!fileName.isEmpty()) {
        std::vector<Triangle> tris;
        if (loadSTLFile(fileName.toStdString(), tris)) {
            if (loadToA) {
                trianglesA = tris;
                QMessageBox::information(this, "Import STL", "Loaded as A: " + fileName);
            } else {
                trianglesB = tris;
                QMessageBox::information(this, "Import STL", "Loaded as B: " + fileName);
            }
            loadToA = !loadToA;
            glWidget->update();
        } else {
            QMessageBox::warning(this, "Import STL", "Failed to load STL file.");
        }
    }
}

void MainWindow::onFindIntersection()
{
    // Just update the GLWidget to show intersection (if any) between trianglesA and trianglesB
    glWidget->update();
    QMessageBox::information(this, "Find Intersection", "Intersection (if any) is now shown in the view.");
}