#include "revised-gui/GUI.h" 
#include "revised-gui/Point.h"
#include "revised-gui/Graph.h"
#include <FL/Fl_GIF_Image.H>
#include <iostream>
#include <vector>
#include <ctime>

using namespace Graph_lib;

const int WINDOW_HEIGHT = 600;
const int WINDOW_WIDTH = 900;
const int WINDOW_PADDING = 120;

class Image_button : public Button {
public:
    Image_button(Point xy, int w, int h, const string& label, Graph_lib::Callback cb)
        : Graph_lib::Button(xy, w, h, label, cb) {
        img = new Fl_GIF_Image("../assets/hutao.gif");
    }

    void attach(Graph_lib::Window& win) {
        Graph_lib::Button::attach(win);
        pw->align(FL_ALIGN_IMAGE_BACKDROP);
        pw->image(img->copy(width, height));
    }

private:
    Fl_GIF_Image* img;
};

class Image_window : Graph_lib::Window {
public:
    Image_window(Point xy, int w, int h, const string& title)
        : Graph_lib::Window(xy, w, h, title),
          button(Point(WINDOW_PADDING + rand() % (WINDOW_WIDTH - 2 * WINDOW_PADDING), WINDOW_PADDING + rand() % (WINDOW_HEIGHT - 2 * WINDOW_PADDING)),
            100, 100, "", cb_next) {
        attach(button);
    }

    int _show() {
        show(); //protected
        return Fl::run();
    }

private:
    Image_button button;

    static void cb_next(Graph_lib::Address, Graph_lib::Address pw) {
        Graph_lib::reference_to<Image_window>(pw).random_move();
    }

    void random_move() {
        int x = button.loc.x, y = button.loc.y;

        int nx = WINDOW_PADDING + rand() % (WINDOW_WIDTH - 2 * WINDOW_PADDING);
        int ny = WINDOW_PADDING + rand() % (WINDOW_HEIGHT - 2 * WINDOW_PADDING);

        button.move(nx - x, ny - y);
    }       
};

int main() {
    srand(time(NULL));

    Image_window win(Point(100, 100), WINDOW_WIDTH, WINDOW_HEIGHT, "Hw3");
    
    return win._show();
}