#ifndef SHAPE3D_H
#define SHAPE3D_H

class Shape3D {
public:
    virtual ~Shape3D() {}
    virtual void draw() = 0;  // every shape must implement draw()
};

#endif // SHAPE3D_H
