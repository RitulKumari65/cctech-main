#pragma once
#include <vector>

class Rectangle
{
private:
    float X, Y; // starting vertice of rectangle
    float length, width;

public:
    Rectangle();
    Rectangle(float cx, float cy, float len, float wid);

    std::vector<std::vector<float>> vertices;

    void draw();

    // Getter methods to access private members
    float getLength() const { return length; }
    float getWidth() const { return width; }
    float getX() const { return X; }
    float getY() const { return Y; }
};
