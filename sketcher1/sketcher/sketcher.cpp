#include <fstream>
#include <sstream>
#include <string>
#include <memory>
#include <algorithm>

#include "sketcher.h"
#include <QDebug>

Sketcher::~Sketcher() {
    // No need to manually delete as unique_ptr handles memory cleanup
}

Vertex* Sketcher::addVertex(double x, double y, double z) {
    for (const auto& v : vertices) {
        if (v->getX() == x && v->getY() == y && v->getZ() == z) {
            return v.get();
        }
    }

    auto vertex = std::make_unique<Vertex>(x, y, z);
    Vertex* vertexPtr = vertex.get();
    vertices.push_back(std::move(vertex));
    return vertexPtr;
}

void Sketcher::removeVertex(double x, double y, double z) {
    auto it = std::remove_if(vertices.begin(), vertices.end(), [&](const std::unique_ptr<Vertex>& v) {
        return v->getX() == x && v->getY() == y && v->getZ() == z;
    });
    vertices.erase(it, vertices.end());
}

void Sketcher::removeVertex(Vertex* vertex) {
    auto it = std::remove_if(vertices.begin(), vertices.end(), [&](const std::unique_ptr<Vertex>& v) {
        return v.get() == vertex;
    });
    vertices.erase(it, vertices.end());
}

void Sketcher::removeVertex(int vertexIdx) {
    if (vertexIdx < 0 || vertexIdx >= vertices.size()) return;
    vertices.erase(vertices.begin() + vertexIdx);
}

/* Methods for adding and removing a Edge in sketch */
Edge* Sketcher::addEdge(Vertex* start, Vertex* end) {
    for (const auto& e : edges) {
        if ((e->getStart() == start && e->getEnd() == end) ||
            (e->getStart() == end && e->getEnd() == start)) {
            return e.get();
        }
    }

    auto edge = std::make_unique<Edge>(start, end);
    Edge* edgePtr = edge.get();
    edges.push_back(std::move(edge));
    return edgePtr;
}

Edge* Sketcher::addEdge(int startIdx, int endIdx) {
    if (startIdx < 0 || startIdx >= vertices.size() || endIdx < 0 || endIdx >= vertices.size()) return nullptr;
    Vertex* start = vertices[startIdx].get();
    Vertex* end = vertices[endIdx].get();
    return addEdge(start, end);
}

void Sketcher::removeEdge(Vertex* start, Vertex* end) {
    auto it = std::remove_if(edges.begin(), edges.end(), [&](const std::unique_ptr<Edge>& e) {
        return e->getStart() == start && e->getEnd() == end;
    });
    edges.erase(it, edges.end());
}

void Sketcher::removeEdge(Edge* edge) {
    auto it = std::remove_if(edges.begin(), edges.end(), [&](const std::unique_ptr<Edge>& e) {
        return e.get() == edge;
    });
    edges.erase(it, edges.end());
}

void Sketcher::removeEdge(int edgeIdx) {
    if (edgeIdx < 0 || edgeIdx >= edges.size()) return;
    edges.erase(edges.begin() + edgeIdx);
}

// validation for face edges
bool Sketcher::checkFaceEdgesValidity(vector<Edge*>& faceEdges) {
    // check if edges are connected to each other
    for (int i = 0; i < faceEdges.size() - 1; i++) {
        auto e1 = faceEdges[i], e2 = faceEdges[i + 1];
        auto v1 = e1->getStart(), v2 = e1->getEnd();
        auto v3 = e2->getStart(), v4 = e2->getEnd();

        if (!(v1 == v3 || v1 == v4 || v2 == v3 || v2 == v4)) {
            return false;
        }
    }

    auto firstEdge = faceEdges[0];
    auto lastEdge = faceEdges[faceEdges.size() - 1];
    auto firstStart = firstEdge->getStart(), firstEnd = firstEdge->getEnd();
    auto lastStart = lastEdge->getStart(), lastEnd = lastEdge->getEnd();

    if (!(firstStart == lastStart || firstStart == lastEnd || firstEnd == lastStart || firstEnd == lastEnd)) {
        return false;
    }

    return true;
}

/* Methods for adding and removing faces in sketches */
Face* Sketcher::addFace() {
    auto face = std::make_unique<Face>();
    Face* facePtr = face.get();
    faces.push_back(std::move(face));
    return facePtr;
}

Face* Sketcher::addFace(vector<Edge*> faceEdges) {
    auto face = std::make_unique<Face>(faceEdges);
    Face* facePtr = face.get();
    faces.push_back(std::move(face));
    return facePtr;
}

Face* Sketcher::addFace(vector<int>& edgeIdxs) {
    vector<Edge*> faceEdges;
    for (auto idx : edgeIdxs) {
        if (idx < 0 || idx >= edges.size()) return nullptr;
        faceEdges.push_back(edges[idx].get());
    }

    bool areEdgesValid = checkFaceEdgesValidity(faceEdges);
    if (!areEdgesValid) {
        qDebug() << "Not valid edges to form a face!";
        return nullptr;
    }

    return addFace(faceEdges);
}

void Sketcher::removeFace(int faceIdx) {
    if (faceIdx < 0 || faceIdx >= faces.size()) return;
    faces.erase(faces.begin() + faceIdx);
}

void Sketcher::removeFace(Face* face) {
    auto it = std::remove_if(faces.begin(), faces.end(), [&](const std::unique_ptr<Face>& f) {
        return f.get() == face;
    });
    faces.erase(it, faces.end());
}

/* Methods for adding and removing solids in sketcher */
Solid* Sketcher::addSolid(Solid* solid = nullptr) {
    if (solid == nullptr) {
        auto newSolid = std::make_unique<Solid>();
        Solid* solidPtr = newSolid.get();
        solids.push_back(std::move(newSolid));
        return solidPtr;
    } else {
        auto solidPtr = std::make_unique<Solid>(*solid);
        Solid* solidRawPtr = solidPtr.get();
        solids.push_back(std::move(solidPtr));
        return solidRawPtr;
    }
}

Solid* Sketcher::addSolid(vector<Face*> faces) {
    auto solid = std::make_unique<Solid>(faces);
    Solid* solidPtr = solid.get();
    solids.push_back(std::move(solid));
    return solidPtr;
}

void Sketcher::removeSolid(int solidIdx) {
    if (solidIdx < 0 || solidIdx >= solids.size()) return;
    solids.erase(solids.begin() + solidIdx);
}

void Sketcher::removeSolid(Solid* solid) {
    auto it = std::remove_if(solids.begin(), solids.end(), [&](const std::unique_ptr<Solid>& s) {
        return s.get() == solid;
    });
    solids.erase(it, solids.end());
}

int Sketcher::findVertex(Vertex* v) {
    for (int i = 0; i < vertices.size(); i++) {
        if (vertices[i].get() == v) return i;
    }
    return -1;
}

int Sketcher::findEdge(Edge* e) {
    for (int i = 0; i < edges.size(); i++) {
        if (edges[i].get() == e) return i;
    }
    return -1;
}

int Sketcher::findFace(Face* f) {
    for (int i = 0; i < faces.size(); i++) {
        if (faces[i].get() == f) return i;
    }
    return -1;
}

int Sketcher::findSolid(Solid* s) {
    for (int i = 0; i < solids.size(); i++) {
        if (solids[i].get() == s) return i;
    }
    return -1;
}

Solid* Sketcher::extrudeFace(Face* face, double height) {
    if (face == nullptr) return nullptr;

    vector<Edge*> faceEdges = face->getEdges();
    vector<Vertex*> faceVertices = face->getVertices();

    vector<Vertex*> extrudedVertices;
    vector<Edge*> extrudedEdges;
    vector<Face*> extrudedFaces;

    // step 1: create new vertices at the extruded height
    for (auto v : faceVertices) {
        double x = v->getX();
        double y = v->getY();
        double z = v->getZ() + height;
        Vertex* newVertex = this->addVertex(x, y, z);
        extrudedVertices.push_back(newVertex);
    }

    // step 2: create side faces
    int n = faceVertices.size();
    for (int i = 0; i < n; i++) {
        Vertex* v1 = faceVertices[i];
        Vertex* v2 = faceVertices[(i + 1) % n];
        Vertex* v3 = extrudedVertices[i];
        Vertex* v4 = extrudedVertices[(i + 1) % n];

        // step 2A: create edges for the side face
        Edge* e1 = this->addEdge(v1, v2);
        Edge* e2 = this->addEdge(v3, v4);
        Edge* e3 = this->addEdge(v1, v3);
        Edge* e4 = this->addEdge(v2, v4);

        // step 2B:  create the side face
        vector<Edge*> sideEdges = {e1, e4, e2, e3};
        Face* sideFace = this->addFace(sideEdges);
        extrudedFaces.push_back(sideFace);
    }

    // step 3: create the top face
    vector<Edge*> topEdges;
    for (int i = 0; i < n; i++) {
        Vertex* v1 = extrudedVertices[i];
        Vertex* v2 = extrudedVertices[(i + 1) % n];
        Edge* e = this->addEdge(v1, v2);
        topEdges.push_back(e);
    }
    Face* topFace = this->addFace(topEdges);

    // Step 4: Add top and base face to the extruded faces
    extrudedFaces.push_back(topFace);
    extrudedFaces.push_back(face);

    // Step 5: Create the solid from extruded faces
    Solid* extrudedSolid = this->addSolid(extrudedFaces);
    return extrudedSolid;
}

Solid* Sketcher::extrudeFace(int faceIdx, double height) {
    if (faceIdx < 0 || faceIdx >= faces.size()) return nullptr;
    Face* f = faces[faceIdx].get();
    return extrudeFace(f, height);
}

Solid* Sketcher::revolveFace(Face* face, double angle, int steps = 20) {
    if (face == nullptr) return nullptr;

    // angle around y-axis
    double radianAngle = angle * M_PI / 180.0;
    double delta = radianAngle / steps;

    vector<Vertex*> baseVertices = face->getVertices();
    vector<vector<Vertex*>> rings;

    // step 1: create rotated rings
    for (int i = 0; i <= steps; i++) {
        double theta = i * delta;
        double cosTheta = cos(theta);
        double sinTheta = sin(theta);

        vector<Vertex*> ring;
        for (auto v : baseVertices) {
            double x = v->getX();
            double y = v->getY();
            double z = v->getZ();

            double newX = x * cosTheta + z * sinTheta;
            double newZ = -x * sinTheta + z * cosTheta;

            Vertex* newVertex = this->addVertex(newX, y, newZ);
            ring.push_back(newVertex);
        }
        rings.push_back(ring);
    }

    Solid* s = this->addSolid();

    // step 2: create side faces between each ring
    int n = baseVertices.size();
    for (int i = 0; i < steps; ++i) {
        vector<Vertex*>& ring1 = rings[i];
        vector<Vertex*>& ring2 = rings[i + 1];

        for (int j = 0; j < n; ++j) {
            Vertex* v1 = ring1[j];
            Vertex* v2 = ring1[(j + 1) % n];
            Vertex* v3 = ring2[(j + 1) % n];
            Vertex* v4 = ring2[j];

            // Create 4 edges and face
            Edge* e1 = this->addEdge(v1, v2);
            Edge* e2 = this->addEdge(v2, v3);
            Edge* e3 = this->addEdge(v3, v4);
            Edge* e4 = this->addEdge(v4, v1);
            // Face* side = addFace({e1, e2, e3, e4});
            Face* side = this->addFace({e1, e4, e3, e2});
            s->addFace(side);
        }
    }

    vector<Edge*> bottomEdges;
    for (int i = 0; i < n; ++i) {
        Vertex* v1 = rings[0][i];
        Vertex* v2 = rings[0][(i + 1) % n];
        Edge* bottomEdge = this->addEdge(v1, v2);
        bottomEdges.push_back(bottomEdge);
    }
    Face* bottomFace = this->addFace(bottomEdges);
    s->addFace(bottomFace);

    // Step 4: Cap the top (last ring)
    vector<Edge*> topEdges;
    for (int i = 0; i < n; ++i) {
        Vertex* v1 = rings[steps][i];
        Vertex* v2 = rings[steps][(i + 1) % n];
        Edge* topEdge = this->addEdge(v1, v2);
        topEdges.push_back(topEdge);
    }
    Face* topFace = this->addFace(topEdges);
    s->addFace(topFace);
    return s;
}

Solid* Sketcher::revolveFace(int faceIdx, double angle, int steps = 20) {
    if (faceIdx < 0 || faceIdx >= faces.size()) return nullptr;
    Face* f = faces[faceIdx].get();
    return revolveFace(f, angle, steps);
}

void Sketcher::clear() {
    vertices.clear();
    edges.clear();
    faces.clear();
    solids.clear();
}

bool Sketcher::loadSketchFromFile(const string filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        qDebug() << "Could not open file: " << QString::fromStdString(filename);
        return false;
    }

    string ext = filename.substr(filename.find_last_of(".") + 1);
    if (ext == "stl") {
        return loadSketchFromSTL(filename);
    } else if (ext == "obj") {
        return loadSketchFromOBJ(filename);
    }
    // else ->
    qDebug() << "Unsupported file format: " << QString::fromStdString(ext);
    return false;
}

bool Sketcher::loadSketchFromSTL(const string filename) {
    ifstream file(filename);

    if (!file.is_open()) {
        qDebug() << "Could not open file: " << QString::fromStdString(filename);
        return false;
    }

    string line;
    vector<Vertex*> currentTriangle;
    vector<Edge*> currentEdges;

    while (getline(file, line)) {
        stringstream ss(line);
        string word;
        ss >> word;

        if (word == "vertex") {
            double x, y, z;
            ss >> x >> y >> z;
            Vertex* v = addVertex(x, y, z);
            currentTriangle.push_back(v);
        }

        if (word == "endloop") {
            if (currentTriangle.size() == 3) {
                Vertex* v0 = currentTriangle[0];
                Vertex* v1 = currentTriangle[1];
                Vertex* v2 = currentTriangle[2];
                Edge* e1 = addEdge(v0, v1);
                Edge* e2 = addEdge(v1, v2);
                Edge* e3 = addEdge(v2, v0);
                Face* f = addFace({e1, e2, e3});
            }
            currentTriangle.clear();
        }
    }

    file.close();
    qDebug() << "STL file loaded successfully";
    return true;
}

bool Sketcher::loadSketchFromOBJ(const string filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        qDebug() << "Could not open OBJ file: " << QString::fromStdString(filename);
        return false;
    }

    vector<Vertex*> tempVertices;
    string line;
    while (getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;

        stringstream ss(line);
        string prefix;
        ss >> prefix;

        if (prefix == "v") {
            double x, y, z;
            ss >> x >> y >> z;
            Vertex* v = addVertex(x, y, z);
            tempVertices.push_back(v);
        } else if (prefix == "f") {
            vector<Vertex*> faceVertices;
            string token;
            while (ss >> token) {
                stringstream tokenStream(token);
                string indexStr;
                getline(tokenStream, indexStr, '/');

                int vertexIndex = stoi(indexStr) - 1;
                if (vertexIndex >= 0 && vertexIndex < tempVertices.size()) {
                    faceVertices.push_back(tempVertices[vertexIndex]);
                }
            }

            vector<Edge*> faceEdges;
            int n = faceVertices.size();
            for (int i = 0; i < n; i++) {
                Vertex* v0 = faceVertices[i];
                Vertex* v1 = faceVertices[(i + 1) % n];
                Edge* e = addEdge(v0, v1);
                faceEdges.push_back(e);
            }

            Face* f = addFace(faceEdges);
        }
    }

    file.close();
    qDebug() << "OBJ file successfully loaded into sketcher";
    return true;
}