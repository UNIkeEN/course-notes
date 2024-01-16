#pragma once
#include <FL/Fl.H>
#include <FL/Fl_Check_Button.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Box.H>

class CustomCheckButton : public Fl_Check_Button {
public:
    CustomCheckButton(int X, int Y, int W, int H, const char *L = 0)
        : Fl_Check_Button(X, Y, W, H, L) {}

protected:
    void draw() {
        if (value()) {
            // 选中状态
            fl_color(fl_rgb_color(0, 103, 192)); 
            fl_rectf(x(), y()+ (h()-12)/2, 12, 12);

            fl_color(FL_WHITE); // 白色对勾
            fl_line_style(FL_SOLID, 2);
            int x1 = x() + 2, y1 = y() + (h()-12)/2 + 7;
            int x2 = x() + 5, y2 = y() + (h()-12)/2 + 10;
            int x3 = x() + 10, y3 = y() + (h()-12)/2 + 2;
            fl_line(x1, y1, x2, y2);
            fl_line(x2, y2, x3, y3);
        } else {
            // 未选中状态
            fl_color(FL_BACKGROUND_COLOR); 
            fl_rectf(x(), y() + (h() - 12) / 2, 12, 12);
        }

        // 绘制标签
        int labelX = x() + 18; 
        int labelY = y();
        int labelW = w() - h() - 6; 
        int labelH = h();
        draw_label(labelX, labelY, labelW, labelH);
    }
};
