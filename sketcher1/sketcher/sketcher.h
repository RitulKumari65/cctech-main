#pragma once
#include "sketcher_primitives.h"
#include <memory>

class Sketcher {
    std::vector<std::unique_ptr<Vertex>> vertices;
    std::vector<std::unique_ptr<Edge>> edges;
    std::vector<std::unique_ptr<Face>> faces;
    std::vector<std::unique_ptr<Solid>> solids;

    // TODO: for future use (selection purpose)
    int currentVertexIdx = 0;
    int currentEdgeIdx = 0;
    int currentFaceIdx = 0;
    int currentSolidIdx = 0;

public:
    Sketcher() {}
    ~Sketcher(); 

    Vertex* addVertex(double x, double y, double z);
    void removeVertex(double x, double y, double z);
    void removeVertex(Vertex* vertex);
    void removeVertex(int vertexIdx);

    Edge* addEdge(Vertex* start, Vertex* end);
    Edge* addEdge(int startIdx, int endIdx);
    void removeEdge(Vertex* start, Vertex* end);
    void removeEdge(Edge* edge);
    void removeEdge(int edgeIdx);

    Face* addFace();
    Face* addFace(std::vector<Edge*> faceEdges);
    Face* addFace(std::vector<int>& edgeIdxs);
    void removeFace(int faceIdx);
    void removeFace(Face* face);

    Solid* addSolid(Solid* solid);
    Solid* addSolid(std::vector<Face*> faces);
    void removeSolid(int solidIdx);
    void removeSolid(Solid* solid);

    std::vector<std::unique_ptr<Vertex>>& getVertices() { return vertices; }
    std::vector<std::unique_ptr<Edge>>& getEdges() { return edges; }
    std::vector<std::unique_ptr<Face>>& getFaces() { return faces; }
    std::vector<std::unique_ptr<Solid>>& getSolids() { return solids; }

    int findVertex(Vertex* v);
    int findEdge(Edge* e);
    int findFace(Face* f);
    int findSolid(Solid* s);

    // validation methods
    bool checkFaceEdgesValidity(std::vector<Edge*>& faceEdges);

    // extrusion methods
    Solid* extrudeFace(Face* face, double height);
    Solid* extrudeFace(int faceIdx, double height);

    Solid* revolveFace(Face* face, double angle, int steps);
    Solid* revolveFace(int faceIdx, double angle, int steps);

    void clear();

    bool loadSketchFromFile(const std::string filename);
    bool loadSketchFromSTL(const std::string filename);
    bool loadSketchFromOBJ(const std::string filename);
};