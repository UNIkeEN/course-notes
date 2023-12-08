#include "revised-gui/Simple_window.h" 
#include "revised-gui/Point.h"
#include "revised-gui/Graph.h"
#include "star.hpp"
#include <iostream>
#include <vector>
#include <ctime>

int main() {

    srand(time(NULL));

    using namespace Graph_lib;
    int x;
    std::cout << "Input number of vertices" << std::endl;
    std::cin >> x;
    if (x < 3) {
        std::cout << "Star must have at least 3 vertices.";
        system("pause");
        return 0;
    }
    Simple_window win(Point(100, 100), 600, 600, "T1"); 

    Star star(Point(300, 300), 160, x);
    win.attach(star);

    win.show();
    win.wait_for_button();

    return 0;
}