#include "intersection.h"
#include <algorithm>
#include <cmath>

// Remove hardcoded triangles
// std::vector<Triangle> trianglesA = { ... };
// std::vector<Triangle> trianglesB = { ... };
std::vector<Triangle> trianglesA;
std::vector<Triangle> trianglesB;

// Helper function: orientation for 2D points
static int orientation(const Point& p, const Point& q, const Point& r) {
    float val = (q.y - p.y) * (r.x - q.x) -
                (q.x - p.x) * (r.y - q.y);
    if (val == 0) return 0;  // colinear
    return (val > 0) ? 1 : 2; // clock or counterclock wise
}

// Helper function: check if q lies on segment pr
static bool onSegment(const Point& p, const Point& q, const Point& r) {
    return q.x <= std::max(p.x, r.x) && q.x >= std::min(p.x, r.x) &&
           q.y <= std::max(p.y, r.y) && q.y >= std::min(p.y, r.y);
}

// 2D segment intersection test (ignoring z)
static bool segmentsIntersect2D(const Point& p1, const Point& q1, const Point& p2, const Point& q2) {
    int o1 = orientation(p1, q1, p2);
    int o2 = orientation(p1, q1, q2);
    int o3 = orientation(p2, q2, p1);
    int o4 = orientation(p2, q2, q1);

    // General case
    if (o1 != o2 && o3 != o4)
        return true;

    // Special Cases
    if (o1 == 0 && onSegment(p1, p2, q1)) return true;
    if (o2 == 0 && onSegment(p1, q2, q1)) return true;
    if (o3 == 0 && onSegment(p2, p1, q2)) return true;
    if (o4 == 0 && onSegment(p2, q1, q2)) return true;

    return false;
}

// Check if any edge of t1 intersects any edge of t2 (2D)
bool trianglesIntersect(const Triangle& t1, const Triangle& t2) {
    const Point* t1_pts[3] = { &t1.p1, &t1.p2, &t1.p3 };
    const Point* t2_pts[3] = { &t2.p1, &t2.p2, &t2.p3 };
    // For each edge in t1
    for (int i = 0; i < 3; ++i) {
        const Point& a1 = *t1_pts[i];
        const Point& a2 = *t1_pts[(i+1)%3];
        // For each edge in t2
        for (int j = 0; j < 3; ++j) {
            const Point& b1 = *t2_pts[j];
            const Point& b2 = *t2_pts[(j+1)%3];
            if (segmentsIntersect2D(a1, a2, b1, b2)) {
                return true;
            }
        }
    }
    return false;
}

// Helper: cross product
static Point cross(const Point& a, const Point& b) {
    return Point(
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    );
}
// Helper: dot product
static float dot(const Point& a, const Point& b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}
// Helper: subtract
static Point sub(const Point& a, const Point& b) {
    return Point(a.x - b.x, a.y - b.y, a.z - b.z);
}
// Helper: check coplanarity
bool trianglesCoplanar(const Triangle& t1, const Triangle& t2) {
    // Plane normal for t1
    Point n1 = cross(sub(t1.p2, t1.p1), sub(t1.p3, t1.p1));
    float d1 = -dot(n1, t1.p1);
    // Check all points of t2 in t1's plane
    float dist1 = dot(n1, t2.p1) + d1;
    float dist2 = dot(n1, t2.p2) + d1;
    float dist3 = dot(n1, t2.p3) + d1;
    float eps = 1e-5f;
    return (fabs(dist1) < eps && fabs(dist2) < eps && fabs(dist3) < eps);
}

// Compute intersection segment of two triangles in 3D (returns true if intersect, and sets segA, segB)
bool triangleTriangleIntersectionSegment(const Triangle& t1, const Triangle& t2, Point& segA, Point& segB) {
    // Möller–Trumbore style: test all edges of t1 against t2 and vice versa
    // Collect intersection points
    std::vector<Point> isects;
    auto segmentIntersectTriangle = [](const Point& p0, const Point& p1, const Triangle& tri, Point& isect) -> bool {
        // Plane of triangle
        Point u = sub(tri.p2, tri.p1);
        Point v = sub(tri.p3, tri.p1);
        Point n = cross(u, v);
        if (n.x == 0 && n.y == 0 && n.z == 0) return false; // degenerate
        Point dir = sub(p1, p0);
        float denom = dot(n, dir);
        if (fabs(denom) < 1e-6f) return false; // parallel
        float t = (dot(n, sub(tri.p1, p0))) / denom;
        if (t < 0.0f || t > 1.0f) return false; // not within segment
        Point P = Point(p0.x + t*dir.x, p0.y + t*dir.y, p0.z + t*dir.z);
        // Inside-triangle test (barycentric)
        Point w = sub(P, tri.p1);
        float uu = dot(u, u), uv = dot(u, v), vv = dot(v, v);
        float wu = dot(w, u), wv = dot(w, v);
        float D = uv * uv - uu * vv;
        float s = (uv * wv - vv * wu) / D;
        float t2 = (uv * wu - uu * wv) / D;
        if (s >= -1e-5f && t2 >= -1e-5f && (s + t2) <= 1.0f + 1e-5f) {
            isect = P;
            return true;
        }
        return false;
    };
    // t1 edges vs t2
    const Point* t1_pts[3] = { &t1.p1, &t1.p2, &t1.p3 };
    for (int i = 0; i < 3; ++i) {
        Point isect;
        if (segmentIntersectTriangle(*t1_pts[i], *t1_pts[(i+1)%3], t2, isect))
            isects.push_back(isect);
    }
    // t2 edges vs t1
    const Point* t2_pts[3] = { &t2.p1, &t2.p2, &t2.p3 };
    for (int i = 0; i < 3; ++i) {
        Point isect;
        if (segmentIntersectTriangle(*t2_pts[i], *t2_pts[(i+1)%3], t1, isect))
            isects.push_back(isect);
    }
    // Remove duplicates (within epsilon)
    float eps = 1e-5f;
    auto same = [eps](const Point& a, const Point& b) {
        return fabs(a.x-b.x)<eps && fabs(a.y-b.y)<eps && fabs(a.z-b.z)<eps;
    };
    std::vector<Point> unique;
    for (const auto& p : isects) {
        bool found = false;
        for (const auto& q : unique) if (same(p, q)) { found = true; break; }
        if (!found) unique.push_back(p);
    }
    if (unique.size() == 2) {
        segA = unique[0];
        segB = unique[1];
        return true;
    }
    return false;
}