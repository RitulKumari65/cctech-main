#include <QApplication>
#include <QMainWindow>
#include "RobotWidget.h"
 
int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    QMainWindow window;
    RobotWidget *robot = new RobotWidget();
    window.setCentralWidget(robot);
    window.resize(800, 600);
    window.show();
    return app.exec();
}