#pragma once 

#include <vector>
#include <algorithm>
#include <memory>
using namespace std;

class Vertex {
    double x, y, z;
public:
    Vertex(double x, double y, double z) : x(x), y(y), z(z) {}
    double getX() const { return x; }
    double getY() const { return y; }
    double getZ() const { return z; }
    void setX(double x) { this->x = x; }
    void setY(double y) { this->y = y; }
    void setZ(double z) { this->z = z; }
};

class Edge {
    Vertex* start;
    Vertex* end;
public:
    Edge(Vertex* start, Vertex* end) : start(start), end(end) {}
    Vertex* getStart() const { return start; }
    Vertex* getEnd() const { return end; }
    void setStart(Vertex* start) { this->start = start; }
    void setEnd(Vertex* end) { this->end = end; }
};

class Face {
    vector<Edge*> edges;
    public:
    Face(){}
    Face(vector<Edge*> edges) : edges(edges) {}
    void addEdge(Edge* edge) { edges.push_back(edge); }
    vector<Edge*> getEdges() const { return edges; }
    vector<Vertex*> getVertices() const {
        vector<Vertex*> vertices;
        for(auto& edge : edges) {
            auto start = edge->getStart();
            auto end = edge->getEnd();
            if (find(vertices.begin(), vertices.end(), start) == vertices.end()) {
                vertices.push_back(start);
            }
            if(find(vertices.begin(), vertices.end(), end) == vertices.end()) {
                vertices.push_back(end);
            }
        }
        return vertices;
    }
};

class Solid {
    vector<Face*> faces;
public:
    Solid() {}
    Solid(vector<Face*> faces) : faces(faces) {}
    void addFace(Face* face) { faces.push_back(face); }
    vector<Face*> getFaces() const { return faces; }
};