#pragma once

#include "revised-gui/Graph.h"
#include "revised-gui/Point.h"
#include <FL/Fl_Image.H>
#include <stdexcept>
#include <cstdlib> 
#include <cmath> 

const double PI = 3.141592653;

using namespace Graph_lib;

class Star : public Polygon {
public:
    Star(Point center, int radius, int vertices);

private:
    void draw_star();
    Color random_color();

    Point center;
    int radius;
    int num_vertices;
};

Star::Star(Point center, int radius, int vertices) : center(center), radius(radius), num_vertices(vertices) { 
    if (vertices < 3) {
        throw std::runtime_error("Star must have at least 3 vertices.");
    }
    draw_star();
}

void Star::draw_star() {

    double outer_angle_step = 2 * PI / num_vertices;
    double inner_angle_step = outer_angle_step / 2;
    double start_angle = PI / 2.0;

    for (int i = 0; i < num_vertices; ++i) {
        double outer_angle = start_angle - i * outer_angle_step;
        int outer_x = center.x + radius * cos(outer_angle);
        int outer_y = center.y - radius * sin(outer_angle);
        add(Point(outer_x, outer_y));

        double inner_angle = outer_angle - inner_angle_step;
        int inner_radius = radius / 3;  
        int inner_x = center.x + inner_radius * cos(inner_angle);
        int inner_y = center.y - inner_radius * sin(inner_angle);
        add(Point(inner_x, inner_y));
    }

    // add(Point(center.x + radius * cos(start_angle), center.y - radius * sin(start_angle)));

    set_color(random_color());
    set_fill_color(random_color());

}


Color Star::random_color() {
    uchar r = static_cast<uchar>(rand() % 256);
    uchar g = static_cast<uchar>(rand() % 256);
    uchar b = static_cast<uchar>(rand() % 256);
    return fl_rgb_color(r, g, b);
}
