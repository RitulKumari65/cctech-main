#include <iostream>
#include "../geometry/include/geometry.h"
#include "../geometry/include/cuboid.h"
#include "../geometry/include/sphere.h"
#include "../geometry/include/cylinder.h"

int main() {
    int choice;
    std::cout << "Choose a shape to plot:\n";
    std::cout << "1. Line\n";
    std::cout << "2. Cuboid\n";
    std::cout << "3. Cube\n";
    std::cout << "4. Sphere\n";
    std::cout << "5. Cylinder\n";
    std::cout << "Enter choice: ";
    std::cin >> choice;

    if (choice == 1) {
        double sx, sy, sz, ex, ey, ez;
        std::cout << "Enter START point (x y z): ";
        std::cin >> sx >> sy >> sz;
        std::cout << "Enter END point (x y z): ";
        std::cin >> ex >> ey >> ez;
        RotatingLine line({sx, sy, sz}, {ex, ey, ez}, 8);
        line.plotInteractive();
    }
    else if (choice == 2) {
        double x, y, z, l, b, h;
        std::cout << "Enter origin (x y z): ";
        std::cin >> x >> y >> z;
        std::cout << "Enter dimensions (length breadth height): ";
        std::cin >> l >> b >> h;
        Cuboid cuboid(x, y, z, l, b, h);
        cuboid.plotInteractive();
    }
    else if (choice == 3) {
        double x, y, z, side;
        std::cout << "Enter origin (x y z): ";
        std::cin >> x >> y >> z;
        std::cout << "Enter side length: ";
        std::cin >> side;
        Cube cube(x, y, z, side);
        cube.plotInteractive();
    }
    else if (choice == 4) {
        double x, y, z, radius;
        std::cout << "Enter center (x y z): ";
        std::cin >> x >> y >> z;
        std::cout << "Enter radius: ";
        std::cin >> radius;
        Sphere sphere(x, y, z, radius);
        sphere.plotInteractive();
    }
    else if (choice == 5) {  // 🟢 Added Cylinder Case
        double x, y, z, radius, height;
        int segments;
        std::cout << "Enter bottom center (x y z): ";
        std::cin >> x >> y >> z;
        std::cout << "Enter radius: ";
        std::cin >> radius;
        std::cout << "Enter height: ";
        std::cin >> height;
        std::cout << "Enter number of segments (for smoothness, e.g., 30): ";
        std::cin >> segments;

        Cylinder cylinder(x, y, z, radius, height, segments);
        cylinder.plotInteractive();
    }
    else {
        std::cout << "Invalid choice!\n";
    }

    return 0;
}
