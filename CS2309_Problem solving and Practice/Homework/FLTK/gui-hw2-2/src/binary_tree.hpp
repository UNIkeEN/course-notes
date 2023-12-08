#pragma once

#include "revised-gui/Graph.h"
#include "revised-gui/Point.h"
#include <FL/Fl_Image.H>
#include <stdexcept>
#include <cstdlib> 
#include <vector> 
#include <iostream>

#define NODE_INTERVAL 15

const double PI = 3.141592653;

using namespace Graph_lib;

auto _random_color() {
    uchar r = static_cast<uchar>(rand() % 128);
    uchar g = static_cast<uchar>(rand() % 128);
    uchar b = static_cast<uchar>(rand() % 128);
    return fl_rgb_color(r, g, b);
}

class Binary_tree : public Shape {
protected:
    int levels;
    vector <Point> nodes;

    virtual void draw_nodes(Point c) const;
    void draw_lines() const;

public:
    Point root;
    Binary_tree(Point root, int levels = 0);
    ~Binary_tree() = default;
};

const double WIDTH = 300; 
const double HEIGHT = 500; 
const int NODE_RADIUS = 8; 

Binary_tree :: Binary_tree(Point root, int levels) : root(root), levels(levels) {
    if (levels < 0) throw std::runtime_error("Levels cannot be negative");
    nodes.resize(1 << levels);

    int v_gap = HEIGHT / (levels + 1); 
    int t = 2;

    for (int level = 0; level < levels; ++level) {
        int level_nodes = 1 << level; 
        for (int i = 0; i < level_nodes; ++i) {
            int x = root.x;
            if (level >= 1) {
                x = root.x - (WIDTH / 2.0) + (2.0 * i+1) * WIDTH / t;
            }
            int y = root.y + v_gap * (level + 1);
            nodes[(1 << level) + i] = Point(x, y);
            add(nodes[(1 << level) + i]);
            // std::cout<<(1 << level) + i<<' '<< x <<' '<< y<<std::endl;
        }
        t *= 2;
    }
}

void Binary_tree :: draw_lines() const {
    fl_color(_random_color());
    for (int i = 1; i < nodes.size()/2; i ++){
        fl_line(nodes[i].x, nodes[i].y, nodes[2*i].x, nodes[2*i].y);
        fl_line(nodes[i].x, nodes[i].y, nodes[2*i+1].x, nodes[2*i+1].y);
    }
    fl_color(_random_color());
    for(int i = 1; i < nodes.size(); i ++) draw_nodes(nodes[i]);
}

void Binary_tree :: draw_nodes(Point c) const {
    fl_arc(c.x - 8, c.y - 8, 16, 16, 0, 360);
}
