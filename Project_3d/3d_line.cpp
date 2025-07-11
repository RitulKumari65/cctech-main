#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>

using namespace std;

class Point3D {
public:
    double x, y, z;
    
    Point3D(double x = 0, double y = 0, double z = 0)
        : x(x), y(y), z(z) {}
};

class RotatingLine {
private:
    vector<Point3D> points;
    string datafile;

    // Rotation functions
    Point3D rotateX(const Point3D& p, double angle) {
        double cosA = cos(angle);
        double sinA = sin(angle);
        return {p.x, 
                p.y * cosA - p.z * sinA,
                p.y * sinA + p.z * cosA};
    }

    Point3D rotateY(const Point3D& p, double angle) {
        double cosA = cos(angle);
        double sinA = sin(angle);
        return {p.x * cosA + p.z * sinA,
                p.y,
                -p.x * sinA + p.z * cosA};
    }

    Point3D rotateZ(const Point3D& p, double angle) {
        double cosA = cos(angle);
        double sinA = sin(angle);
        return {p.x * cosA - p.y * sinA,
                p.x * sinA + p.y * cosA,
                p.z};
    }

public:
    RotatingLine(Point3D start, Point3D end, int numInternalPoints) {
        generatePoints(start, end, numInternalPoints);
        datafile = "line_data.dat";
    }

    void generatePoints(Point3D start, Point3D end, int numPoints) {
        points.clear();
        points.push_back(start);
        
        for(int i = 1; i <= numPoints; i++) {
            double t = static_cast<double>(i)/(numPoints + 1);
            points.push_back({
                start.x + t * (end.x - start.x),
                start.y + t * (end.y - start.y),
                start.z + t * (end.z - start.z)
            });
        }
        points.push_back(end);
    }

    void applyRotation(double x_deg, double y_deg, double z_deg) {
        double x_rad = x_deg * M_PI / 180.0;
        double y_rad = y_deg * M_PI / 180.0;
        double z_rad = z_deg * M_PI / 180.0;
        
        for(auto& p : points) {
            p = rotateX(p, x_rad);
            p = rotateY(p, y_rad);
            p = rotateZ(p, z_rad);
        }
    }

    void saveData() {
        ofstream out(datafile);
        for(const auto& p : points) {
            out << p.x << " " << p.y << " " << p.z << endl;
        }
        out.close();
    }

    void plotInteractive() {
        saveData();
        
        ofstream script("plot.gnu");
        script << "set title '3D Line with Interactive Rotation'\n"
               << "set xlabel 'X'\n"
               << "set ylabel 'Y'\n"
               << "set zlabel 'Z'\n"
               << "set grid\n"
               << "set mouse\n"  // Enable mouse interaction
               << "set view 60, 30\n"
               << "splot '" << datafile << "' with linespoints lc rgb 'red' pt 7\n"
               << "pause -1 'Press any key to exit'\n";
        script.close();

        system("gnuplot plot.gnu");
    }
};

int main() {
    double sx, sy, sz, ex, ey, ez;
    
    cout << "Enter START point (x y z): ";
    cin >> sx >> sy >> sz;
    
    cout << "Enter END point (x y z): ";
    cin >> ex >> ey >> ez;

    // Create line with 8 internal points
    RotatingLine line({sx, sy, sz}, {ex, ey, ez}, 8);
    
    // Optional: Apply initial rotation (0 degrees for no initial rotation)
    line.applyRotation(0, 0, 0);
    
    // Start interactive plot
    cout << "\nOpening plot window...\n";
    cout << "== Use mouse to rotate:\n";
    cout << "   - Left click + drag: Rotate\n";
    cout << "   - Right click + drag: Zoom\n";
    cout << "   - Close window to exit\n";
    
    line.plotInteractive();

    return 0;
}