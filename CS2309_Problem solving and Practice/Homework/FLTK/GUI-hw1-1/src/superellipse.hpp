#pragma once

#include "revised-gui/Graph.h"
#include "revised-gui/Point.h"
#include <stdexcept>
#include <cmath>
#include <vector>

const double PI = 3.141592653;

using namespace Graph_lib;

class Superellipse : public Shape {
private:
    double a, b, m, n;
    Point center;
    int cnt; 
    int x_scale, y_scale;

    Point calc_point(double theta) {
        double cos_t = cos(theta);
        double sin_t = sin(theta);
        return Point(center.x + (cos_t >= 0 ? 1 : -1) * int(pow(fabs(cos_t), 2/m) * a * x_scale),
                     center.y + (sin_t >= 0 ? 1 : -1) * int(b * pow(fabs(sin_t), 2 / n) * y_scale));
    }

    void draw() {
        for (int i = 0; i < cnt; ++i) {
            double theta = 2 * PI * i / cnt;

            double x = pow(fabs(cos(theta)), 2.0/m) * a * (cos(theta) >= 0 ? 1 : -1) * x_scale;
            double y = pow(fabs(sin(theta)), 2.0/n) * b * (sin(theta) >= 0 ? 1 : -1) * y_scale;

            add(Point(center.x + x, center.y + y));
        }
    }

public:

    ~Superellipse() = default;
    
    Superellipse(Point _center, double _a, double _b, double _m, double _n, int _cnt = 1000, int _x_scale = 50, int _y_scale = 50)
        : center(_center), a(_a), b(_b), m(_m), n(_n), cnt(_cnt), x_scale(_x_scale), y_scale(_y_scale) {
        if (_a <= 0 || _b <= 0 || _m <= 0 || _n <= 0 || _cnt <= 0) throw std::runtime_error("Invalid parameters for superellipse");
        draw();
    }

    void draw_star(Lines &lineset, int N) {
        std::vector<Point> points;

        if (N <= 0) throw std::runtime_error("Number of points must be positive.");

        double dtheta = 2 * PI / N;

        for (int i = 0; i < N; ++i) points.push_back(calc_point(i * dtheta));

        for (int i = 0; i < N; ++i) {
            for (int j = i + 1; j < N; ++j) {
                lineset.add(points[i], points[j]);
            }
        }
    }
};