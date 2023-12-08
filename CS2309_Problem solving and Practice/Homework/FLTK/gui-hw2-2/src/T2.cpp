#include "revised-gui/Simple_window.h" 
#include "revised-gui/Point.h"
#include "revised-gui/Graph.h"
#include "binary_tree.hpp"
#include <iostream>
#include <vector>
#include <ctime>


int main() {

    srand(time(NULL));

    using namespace Graph_lib;
    int x;
    std::cout << "Input number of levels" << std::endl;
    std::cin >> x;
    if (x < 0) {
        std::cout << "Level must be at least 0.";
        system("pause");
        return 0;
    }
    Simple_window win(Point(100, 100), 600, 600, "T2"); 

    Binary_tree tree(Point(300, 100), x);
    win.attach(tree);

    win.show();
    win.wait_for_button();

    return 0;
}