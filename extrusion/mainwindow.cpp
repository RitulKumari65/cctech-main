#include "mainwindow.h"
#include "glwidget.h"
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>
#include "Extrusionwidget/PolygonExtrusionwidget.h" // Include the header

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), extrusionWidget(nullptr)
{
    QWidget *central = new QWidget(this);

    // Create a horizontal layout for the main window
    QHBoxLayout *mainLayout = new QHBoxLayout(central);

    // Create a vertical layout for the buttons
    QVBoxLayout *buttonLayout = new QVBoxLayout();

    glWidget = new GLWidget(this);

    intersectBtn = new QPushButton("Intersection", this);
    unionBtn = new QPushButton("Union", this);
    diffBtn = new QPushButton("Difference", this);
    extrusionBtn = new QPushButton("Extrusion", this); // New button

    // Set fixed width for the buttons
    intersectBtn->setFixedWidth(100);
    unionBtn->setFixedWidth(100);
    diffBtn->setFixedWidth(100);
    extrusionBtn->setFixedWidth(100);

    // Add buttons to the vertical layout
    buttonLayout->addWidget(intersectBtn);
    buttonLayout->addWidget(unionBtn);
    buttonLayout->addWidget(diffBtn);
    buttonLayout->addWidget(extrusionBtn); // Add new button
    buttonLayout->addStretch(); // Add stretch to push buttons to the top

    // Add the GLWidget and button layout to the main layout
    mainLayout->addWidget(glWidget);
    mainLayout->addLayout(buttonLayout);

    setCentralWidget(central);

    connect(intersectBtn, &QPushButton::clicked, this, &MainWindow::setIntersection);
    connect(unionBtn, &QPushButton::clicked, this, &MainWindow::setUnion);
    connect(diffBtn, &QPushButton::clicked, this, &MainWindow::setDifference);
    connect(extrusionBtn, &QPushButton::clicked, this, &MainWindow::openExtrusionWindow); // Connect new button
}

MainWindow::~MainWindow()
{
    if (extrusionWidget) delete extrusionWidget;
}

void MainWindow::setIntersection()
{
    glWidget->setOperation("intersection");
}

void MainWindow::setUnion()
{
    glWidget->setOperation("union");
}

void MainWindow::setDifference()
{
    glWidget->setOperation("difference");
}

void MainWindow::openExtrusionWindow()
{
    if (!extrusionWidget) {
        extrusionWidget = new PolygonExtrusionWidget();
        extrusionWidget->setAttribute(Qt::WA_DeleteOnClose);
    }
    extrusionWidget->show();
    extrusionWidget->raise();
    extrusionWidget->activateWindow();
}
