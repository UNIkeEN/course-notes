#pragma once

#include "revised-gui/Graph.h"
#include "revised-gui/Point.h"
#include <vector>
#include <stdexcept>

using namespace Graph_lib;

class Poly : public Shape {
public:
    Poly(const std::vector<Point>& points);

private:
    static bool doSegmentsIntersect(const Point& p1, const Point& p2, const Point& p3, const Point& p4);
    static double cross(const Point& a, const Point& b);
    static bool isPointOnLine(const Point& p, const Point& lineStart, const Point& lineEnd);
    static bool areAllPointsCollinear(const std::vector<Point>& points);
    static bool areAdjacentPointsEqual(const std::vector<Point>& points);
};

Poly::Poly(const std::vector<Point>& points) {
    if (points.size() < 3) {
        throw std::runtime_error("Poly requires at least 3 points.");
    }

    if (areAllPointsCollinear(points) || areAdjacentPointsEqual(points)) {
        throw std::runtime_error("All points are collinear or adjacent points are equal.");
    }

    for (size_t i = 0; i < points.size(); ++i) {
        size_t next_i = (i + 1) % points.size();

        for (size_t j = i + 1; j < points.size(); ++j) {
            size_t next_j = (j + 1) % points.size();

            if (j != next_i && i != next_j && doSegmentsIntersect(points[i], points[next_i], points[j], points[next_j])) {
                throw std::runtime_error("Poly segments intersect.");
            }
        }

        add(points[i]);
    }
    add(points[0]); 
}

bool Poly::doSegmentsIntersect(const Point& p1, const Point& p2, const Point& p3, const Point& p4) {
    double d1 = cross(Point(p3.x - p1.x, p3.y - p1.y), Point(p4.x - p1.x, p4.y - p1.y));
    double d2 = cross(Point(p3.x - p2.x, p3.y - p2.y), Point(p4.x - p2.x, p4.y - p2.y));
    double d3 = cross(Point(p1.x - p3.x, p1.y - p3.y), Point(p2.x - p3.x, p2.y - p3.y));
    double d4 = cross(Point(p1.x - p4.x, p1.y - p4.y), Point(p2.x - p4.x, p2.y - p4.y));

    if (((d1 > 0 && d2 < 0) || (d1 < 0 && d2 > 0)) && ((d3 > 0 && d4 < 0) || (d3 < 0 && d4 > 0))) {
        return true;
    }

    if (d1 == 0 && isPointOnLine(p3, p1, p2)) return true;
    if (d2 == 0 && isPointOnLine(p4, p1, p2)) return true;
    if (d3 == 0 && isPointOnLine(p1, p3, p4)) return true;
    if (d4 == 0 && isPointOnLine(p2, p3, p4)) return true;

    return false;
}

double Poly::cross(const Point& a, const Point& b) {
    return a.x * b.y - b.x * a.y;
}

bool Poly::isPointOnLine(const Point& p, const Point& lineStart, const Point& lineEnd) {
    double dxl = lineEnd.x - lineStart.x;
    double dyl = lineEnd.y - lineStart.y;
    double dxp = p.x - lineStart.x;
    double dyp = p.y - lineStart.y;

    double cross = dxp * dyl - dyp * dxl;
    if (cross != 0) return false;

    if (abs(dxl) >= abs(dyl))
        return dxl > 0 ? lineStart.x <= p.x && p.x <= lineEnd.x : lineEnd.x <= p.x && p.x <= lineStart.x;
    else
        return dyl > 0 ? lineStart.y <= p.y && p.y <= lineEnd.y : lineEnd.y <= p.y && p.y <= lineStart.y;
}

Point operator - (const Point& A, const Point& B)
{
    return Point(A.x-B.x,A.y-B.y);
}

bool Poly::areAllPointsCollinear(const std::vector<Point>& points) {
    for (size_t i = 0; i < points.size() - 2; ++i) {
        if (cross(points[i + 1] - points[i], points[i + 2] - points[i]) != 0) {
            return false;
        }
    }
    return true;
}

bool Poly::areAdjacentPointsEqual(const std::vector<Point>& points) {
    for (size_t i = 0; i < points.size(); ++i) {
        size_t next_i = (i + 1) % points.size();
        if (points[i] == points[next_i]) {
            return true;
        }
    }
    return false;
}