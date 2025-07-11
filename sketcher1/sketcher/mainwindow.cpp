// mainwindow.cpp
#include "mainwindow.h"
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setupUI();
}

MainWindow::~MainWindow() {}

void MainWindow::setupUI() {
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout;

    // OpenGL widget
    openGLWidget = new SketcherWidget(this);
    openGLWidget->setMinimumSize(600, 400);
    mainLayout->addWidget(openGLWidget);

    // Buttons for operations
    QHBoxLayout *buttonLayout = new QHBoxLayout;

    createShapeButton = new QPushButton("Create New Shape");
    unionButton = new QPushButton("Union (1 + 2)");
    diffButton = new QPushButton("Difference (1 - 2)");
    intersectButton = new QPushButton("Intersection (1 ∩ 2)");

    buttonLayout->addWidget(createShapeButton);
    buttonLayout->addWidget(unionButton);
    buttonLayout->addWidget(diffButton);
    buttonLayout->addWidget(intersectButton);

    mainLayout->addLayout(buttonLayout);
    centralWidget->setLayout(mainLayout);
    setCentralWidget(centralWidget);

    // Connect buttons to slots
    connect(createShapeButton, &QPushButton::clicked, openGLWidget, &SketcherWidget::startCreatingShape);
    connect(createShapeButton, &QPushButton::clicked, openGLWidget, &SketcherWidget::finalizeCurrentShape);
    connect(unionButton, &QPushButton::clicked, openGLWidget, &SketcherWidget::applyUnion);
    connect(diffButton, &QPushButton::clicked, openGLWidget, &SketcherWidget::applyDifference);
    connect(intersectButton, &QPushButton::clicked, openGLWidget, &SketcherWidget::applyIntersection);
}
