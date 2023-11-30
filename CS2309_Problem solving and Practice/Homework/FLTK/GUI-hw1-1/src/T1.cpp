#include "revised-gui/Simple_window.h"
#include "revised-gui/Graph.h"
#include "superellipse.hpp"

#include <iostream>

int main()
{
    using namespace Graph_lib; 
    double a, b, m, n;
    int N;

    std::cin >> a >> b >> m >> n;
    Simple_window win(Point(100, 100), 600, 600, "T1"); 

    Superellipse se(Point(300, 300), a, b, m, n);
    win.attach(se);
    se.set_color(Color::red);

    std::cin >> N;
    Lines star_lines;
    star_lines.set_color(Color::yellow);
    se.draw_star(star_lines, N);
    win.attach(star_lines);

    win.show();
    win.wait_for_button();

    return 0;
}
