#include "../geometry/include/robot.h"
#include <fstream>
#include <cmath>

Robot::Robot(double x, double y, double z)
    : torso(x, y, z, 2.0, 1.0, 3.0),    
      head(x + 1.0, y + 0.5, z + 3.5, 1.0), 
      leftArm(x - 1.2, y, z + 2.5, 0.4, 2.0), 
      rightArm(x + 2.2, y, z + 2.5, 0.4, 2.0), 
      leftLeg(x + 0.3, y, z - 1.5, 0.4, 2.5), 
      rightLeg(x + 1.3, y, z - 1.5, 0.4, 2.5),
      translateX(0), translateY(0), translateZ(0),
      rotateX(0), rotateY(0), rotateZ(0),
      scaleX(1), scaleY(1), scaleZ(1)
{}

// Apply translation
void Robot::setTranslation(double tx, double ty, double tz) {
    translateX = tx;
    translateY = ty;
    translateZ = tz;
}

// Apply rotation (in degrees)
void Robot::setRotation(double rotX, double rotY, double rotZ) {
    rotateX = rotX * M_PI / 180.0;
    rotateY = rotY * M_PI / 180.0;
    rotateZ = rotZ * M_PI / 180.0;
}

// Apply scaling
void Robot::setScaling(double sx, double sy, double sz) {
    scaleX = sx;
    scaleY = sy;
    scaleZ = sz;
}

// Function to apply transformations on a point
Point3D Robot::applyTransform(const Point3D& p) {
    double x = p.x, y = p.y, z = p.z;

    // Scaling
    x *= scaleX;
    y *= scaleY;
    z *= scaleZ;

    // Rotation around X-axis
    double y1 = y * cos(rotateX) - z * sin(rotateX);
    double z1 = y * sin(rotateX) + z * cos(rotateX);

    // Rotation around Y-axis
    double x2 = x * cos(rotateY) + z1 * sin(rotateY);
    double z2 = -x * sin(rotateY) + z1 * cos(rotateY);

    // Rotation around Z-axis
    double x3 = x2 * cos(rotateZ) - y1 * sin(rotateZ);
    double y3 = x2 * sin(rotateZ) + y1 * cos(rotateZ);

    // Translation
    x3 += translateX;
    y3 += translateY;
    z2 += translateZ;

    return {x3, y3, z2};
}

void Robot::saveData() {
    std::ofstream out("robot_data.dat");

    auto writeShape = [&](auto& shape) {
        auto vertices = shape.getDrawable();
        for (auto& v : vertices) {
            Point3D transformed = applyTransform(v);
            out << transformed.x << " " << transformed.y << " " << transformed.z << "\n";
        }
        out << "NaN NaN NaN\n";
    };

    writeShape(torso);
    writeShape(head);
    writeShape(leftArm);
    writeShape(rightArm);
    writeShape(leftLeg);
    writeShape(rightLeg);

    out.close();
}

void Robot::plotInteractive() {
    saveData();

    std::ofstream script("plot_robot.gnu");
    script << "set title 'Transformed 3D Robot'\n"
           << "set xlabel 'X'\n"
           << "set ylabel 'Y'\n"
           << "set zlabel 'Z'\n"
           << "set grid\n"
           << "set mouse\n"
           << "set view 60, 30\n"
           << "splot 'robot_data.dat' with lines lc rgb 'red'\n"
           << "pause -1 'Press any key to exit'\n";
    script.close();

    system("gnuplot plot_robot.gnu");
}
