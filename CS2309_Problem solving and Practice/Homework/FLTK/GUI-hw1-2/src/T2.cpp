#include "revised-gui/Simple_window.h" 
#include "revised-gui/Point.h"
#include "revised-gui/Graph.h"
#include "poly.hpp"
#include <iostream>
#include <vector>

using namespace Graph_lib;

int main() {
    std::vector<Point> init;
    int x, y;
    std::cout << "input (x, y) as lines to add points, input ctrl+z to end. (in Windows)" << std::endl;
    while (std::cin >> x >> y) {
        init.push_back(Point(x, y));
    }

    try {
        Poly pol(init);
        pol.set_color(Color::magenta);

        Simple_window win(Point(300, 200), 600, 400, "T2");
        win.attach(pol);
        win.wait_for_button();
    } catch (const std::runtime_error& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    system("pause");
    return 0;
}