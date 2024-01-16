#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Output.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Native_File_Chooser.H>
#include <FL/Fl_JPEG_Image.H>
#include <FL/fl_message.H>
#include <FL/Fl_Image_Surface.H>
#include <FL/fl_draw.H>
#include <vector>
#include <cmath>
#include <algorithm>
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip> 

// #include "card.h"
#include "customCheck.h"

class ImageViewer : public Fl_Widget {
    Fl_Image *image;
    Fl_Button *btnClearPoints, *btnCalculate;
    std::vector<std::pair<double, double>> &points;  // 引用Prj1Window中的点
    double scale_ratio;

public:
    bool is_draw_poly;

    ImageViewer(int X, int Y, int W, int H, std::vector<std::pair<double, double>> &pts, Fl_Button* btn1, Fl_Button* btn2, const char *L = 0)
        : Fl_Widget(X, Y, W, H, L), image(NULL), points(pts), scale_ratio(1.0f), btnClearPoints(btn1), btnCalculate(btn2), is_draw_poly(false) {}

    void draw() override {
        // 绘制图片
        if (image) {
            image->draw(x(), y(), w(), h());
        }

        // 绘制所有点
        fl_color(fl_rgb_color(153, 51, 255));
        for (auto &p : points) {
            int draw_x = static_cast<int>(p.first * scale_ratio) + x();
            int draw_y = static_cast<int>(p.second * scale_ratio) + y();
            fl_pie(draw_x, draw_y, 5, 5, 0, 360);
        }

        // 绘制多边形
        if (is_draw_poly) {
            fl_color(fl_rgb_color(0, 103, 192));

            for (size_t i = 0; i < points.size(); i++) {
                int next_i = (i + 1) % points.size();
                int draw_x1 = static_cast<int>(points[i].first * scale_ratio) + x();
                int draw_y1 = static_cast<int>(points[i].second * scale_ratio) + y();
                int draw_x2 = static_cast<int>(points[next_i].first * scale_ratio) + x();
                int draw_y2 = static_cast<int>(points[next_i].second * scale_ratio) + y();
                fl_line(draw_x1, draw_y1, draw_x2, draw_y2);
            }

            Fl_Image_Surface *temp_surface = new Fl_Image_Surface(w(), h());
            temp_surface->set_current();

            fl_begin_complex_polygon();
            for (auto &p : points) {
                fl_vertex(p.first * scale_ratio, p.second * scale_ratio);
            }
            fl_end_complex_polygon();

            // 获取临时图像并添加 alpha 通道
            Fl_RGB_Image *temp_image = temp_surface->image();
            uchar *alpha_data = new uchar[temp_image->w() * temp_image->h() * 4];
            for (int i = 0; i < temp_image->w() * temp_image->h(); i++) {
                alpha_data[i * 4] = temp_image->data()[0][i * 3];
                alpha_data[i * 4 + 1] = temp_image->data()[0][i * 3 + 1];
                alpha_data[i * 4 + 2] = temp_image->data()[0][i * 3 + 2];
                alpha_data[i * 4 + 3] = 64; 
            }
            Fl_RGB_Image *transparent_image = new Fl_RGB_Image(alpha_data, temp_image->w(), temp_image->h(), 4);

            // 绘制到当前表面
            Fl_Display_Device::display_device()->set_current();
            transparent_image->draw(x(), y());
            delete temp_surface;
            delete temp_image;
            delete transparent_image;
            delete[] alpha_data;
        }

    }

    void setImage(Fl_Image *img) {
        image = img;
        redraw();
    }

    void setScaleRatio(double ratio) {
        scale_ratio = ratio;
    }

    int handle(int event) override {
        switch (event) {
            case FL_PUSH:
                if (image && Fl::event_x() >= x() && Fl::event_x() <= x() + w() &&
                    Fl::event_y() >= y() && Fl::event_y() <= y() + h()) {
                    int scaled_x = static_cast<int>((Fl::event_x() - x()) / scale_ratio);
                    int scaled_y = static_cast<int>((Fl::event_y() - y()) / scale_ratio);

                    // 添加点，并确保它们在图片内
                    if (scaled_x >= 0 && scaled_x < image->w() / scale_ratio &&
                        scaled_y >= 0 && scaled_y < image->h() / scale_ratio) {
                        points.push_back(std::make_pair(scaled_x, scaled_y));
                        redraw();
                        btnCalculate->activate();
                        btnClearPoints->activate();
                    }
                    return 1;
                }
                break;
        }
        return Fl_Widget::handle(event);
    }
};

class Prj1Window : public Fl_Window
{
private:
    ImageViewer *imgViewer;
    Fl_Button *btnLoad, *btnAdd;
    Fl_Button *btnClearPoints;
    Fl_Native_File_Chooser *fileChooser;
    Fl_Input *inputX, *inputY;
    Fl_Input *inputScaleX, *inputScaleY;
    Fl_Button *btnCalculate;
    Fl_Box* outputPoints, *outputArea, *labelM, *sepLine;
    CustomCheckButton *checkComplexPoly;
    int ori_img_w, ori_img_h;
    
    double scale_ratio;
    std::vector<std::pair<double, double>> points;

public:
    Prj1Window() : Fl_Window(900, 600, "Project1: 多边形面积计算的应用") {
        begin();

        Fl_Box* _borderBox = new Fl_Box(23, 23, w() - 46, h() - 46);
        _borderBox->box(FL_FLAT_BOX); 
        _borderBox->color(fl_rgb_color(226, 229, 234)); // 浅灰色

        Fl_Box* borderBox = new Fl_Box(25, 25, w() - 50, h() - 50);
        borderBox->box(FL_FLAT_BOX); 
        borderBox->color(fl_rgb_color(251, 251, 253)); // 浅灰色

        sepLine = new Fl_Box(224, 25, 2, 550);
        sepLine->box(FL_FLAT_BOX); // 绘制一条向下的线
        sepLine->color(fl_rgb_color(226, 229, 234));

        btnLoad = new Fl_Button(50, 50, 150, 30, "加载图片");
        btnLoad->callback(btnLoad_CB, (void*)this);
        btnLoad->color(fl_rgb_color(254, 254, 254));
        btnLoad->color2(fl_rgb_color(211, 211, 212));

        fileChooser = new Fl_Native_File_Chooser();
        fileChooser->filter("*.jpg");
        fileChooser->title("选择图片");

        inputX = new Fl_Input(70, 110, 50, 30, "X:");
        inputY = new Fl_Input(150, 110, 50, 30, "Y:");
        btnAdd = new Fl_Button(50, 150, 150, 30, "添加");
        btnAdd->callback(btnAdd_CB, (void*)this);
        btnAdd->deactivate();
        btnAdd->color(fl_rgb_color(254, 254, 254));
        btnAdd->color2(fl_rgb_color(211, 211, 212));
        btnClearPoints = new Fl_Button(50, 190, 150, 30, "清空");
        btnClearPoints->callback(btnClearPoints_CB, (void*)this);
        btnClearPoints->deactivate();
        btnClearPoints->color(fl_rgb_color(254, 254, 254));
        btnClearPoints->color2(fl_rgb_color(211, 211, 212));

        outputPoints = new Fl_Box(250, 560, 600, 20);
        outputPoints->box(FL_NO_BOX);
        outputPoints->labelsize(12);

        inputScaleX = new Fl_Input(50, 250, 60, 30);
        inputScaleY = new Fl_Input(120, 250, 60, 30, ":");
        labelM = new Fl_Box(180, 250, 20, 30, "m");
        labelM->box(FL_NO_BOX); 
        labelM->labelsize(14); 

        btnCalculate = new Fl_Button(50, 290, 150, 30, "计算面积");
        btnCalculate->callback(btnCalculate_CB, (void*)this);
        btnCalculate->deactivate();
        btnCalculate->color(fl_rgb_color(254, 254, 254));
        btnCalculate->color2(fl_rgb_color(211, 211, 212));

        checkComplexPoly = new CustomCheckButton(50, 340, 150, 30, "检查复杂多边形");
        checkComplexPoly->value(0); // 默认选中
        checkComplexPoly -> color2(fl_rgb_color(0, 103, 192));

        outputArea = new Fl_Box(50, 380, 250, 50);
        outputArea->align(FL_ALIGN_LEFT | FL_ALIGN_INSIDE);
        outputArea->box(FL_NO_BOX);
        outputArea->labelsize(12);

        imgViewer = new ImageViewer(250, 50, 600, 500, points, btnClearPoints, btnCalculate);
        imgViewer->color(fl_rgb_color(240, 240, 240));
        imgViewer->redraw();
    }

    static void btnLoad_CB(Fl_Widget*, void* v) {
        Prj1Window* window = (Prj1Window*)v;
        window->LoadImage();
    }

    static void btnAdd_CB(Fl_Widget*, void* v) {
        Prj1Window* window = (Prj1Window*)v;
        window->AddPoint();
    }

    static void btnCalculate_CB(Fl_Widget*, void* v) {
        Prj1Window* window = (Prj1Window*)v;
        window->CalculateArea();
    }

    static void btnClearPoints_CB(Fl_Widget*, void* v) {
        Prj1Window* window = (Prj1Window*)v;
        window->ClearPoints();
    }

    void LoadImage() {
        fileChooser->show();

        if (fileChooser->filename() != NULL) {
            Fl_JPEG_Image* img = new Fl_JPEG_Image(fileChooser->filename());

            if (img->data() == NULL) {
                fl_message("无法加载图片，请确保文件是有效的JPEG格式");
                delete img;
                return;
            }

            if (img->w() > 1200 || img->h() > 1000) {
                fl_message("图片过大");
                delete img;
                return;
            }

            ori_img_w = img->w();
            ori_img_h = img->h();

            int max_width = 600, max_height = 500;
            scale_ratio = 1.0;  // 默认不缩放

            if (img->w() > max_width || img->h() > max_height) {
                scale_ratio = std::fmin(double(max_width) / img->w(), double(max_height) / img->h());
                int new_width = static_cast<int>(img->w() * scale_ratio);
                int new_height = static_cast<int>(img->h() * scale_ratio);

                Fl_Image* scaled_img = img->copy(new_width, new_height);
                imgViewer->setImage(scaled_img);
                imgViewer->setScaleRatio(scale_ratio);
                btnAdd->activate();
                delete img;
            } else {
                // 如果图片原始尺寸小于600x500，则不缩放
                imgViewer->setImage(img);
                imgViewer->setScaleRatio(scale_ratio);
                btnAdd->activate();
            }
            ClearPoints();
        }
    }

    void AddPoint() {

        double x, y;
        try {
            x = std::stod(inputX->value());
            y = std::stod(inputY->value());
        } catch (const std::invalid_argument& ia) {
            fl_message("非法坐标");
            return;
        }

        // 检查坐标是否重复
        for (const auto &p : points) {
            if (std::abs(p.first - x) < 1e-6 && std::abs(p.second - y) < 1e-6) {
                fl_message("该点重复");
                return;
            }
        }

        if (x<0 || x>ori_img_w || y<0 || y>ori_img_h) {
            fl_message("非法坐标");
            return;
        }

        btnCalculate->activate();
        btnClearPoints->activate();

        // 直接存储原始坐标
        points.push_back(std::make_pair(x, y));
        imgViewer->redraw();

        inputX->value("");
        inputY->value("");

        // 更新输出框
        // std::ostringstream coordinates_stream;
        // coordinates_stream << std::fixed << std::setprecision(1);
        // for (auto &p : points) {
        //     coordinates_stream << "(" << (p.first) << ", " << (p.second) << ") ";
        // }
        // std::string coordinates_str = coordinates_stream.str();
        // outputPoints->copy_label(coordinates_str.c_str());
    }

    void Prj1Window::ClearPoints() {
        points.clear();
        imgViewer->is_draw_poly = false;
        imgViewer->redraw(); 
        btnClearPoints->deactivate();
        outputArea->copy_label("");
    }

    double calculatePolygonArea(const std::vector<std::pair<double, double>>& points) {
        double area = 0.0;
        int j = points.size() - 1;
        for (size_t i = 0; i < points.size(); i++) {
            area += (points[j].first + points[i].first) * (points[j].second - points[i].second);
            j = i;
        }
        return std::abs(area / 2.0);
    }


    void CalculateArea() {
        if (!isValidPolygon(points)) {
            fl_message("非法的多边形");
            return;
        }
        imgViewer->is_draw_poly = true;
        imgViewer->redraw();
        double scaleX, scaleY;
        double pixelArea = calculatePolygonArea(points);
        try {
            scaleX = std::stod(inputScaleX->value());
            scaleY = std::stod(inputScaleY->value());
        } catch (const std::invalid_argument& ia) {
            fl_message("非法比例");
            return;
        }
        double realArea = pixelArea * (scaleY / scaleX) * (scaleY / scaleX);

        std::ostringstream ss;

        // auto formatNumber = [](std::ostringstream& ss, double number) {
        //     if (number == 0.0) {
        //         ss << std::fixed << std::setprecision(0) << number;
        //     } else {
        //         double absNumber = std::abs(number);
        //         if (absNumber < 1e-6 || absNumber >= 1e6) {
        //             // 科学计数法
        //             ss << std::scientific << std::setprecision(5) << number;
        //         } else {
        //             int digits = static_cast<int>(std::log10(absNumber)) + 1;
        //             ss << std::fixed << std::setprecision(0>5 - digits?0:5-digits) << number;
        //         }
        //     }
        // };

        //setprecision使用银行家舍入，在样例上和答案差一位，改用手动+sprintf严格四舍五入
        auto formatNumber = [](std::ostringstream& ss, double number) {

            double scale = std::pow(10.0, 5 - std::floor(std::log10(std::abs(number))));
            number = std::round(number * scale) / scale;

            char buffer[50];
            if (number == 0.0) {
                std::sprintf(buffer, "%.6g", number);
            } else {
                double absNumber = std::abs(number);
                if (absNumber < 1e-6 || absNumber >= 1e6) {
                    // 科学计数法
                    std::sprintf(buffer, "%.6g", number);
                } else {
                    int digits = static_cast<int>(std::log10(absNumber)) + 1;
                    std::sprintf(buffer, "%.*f", (digits >= 6) ? 0 : 6 - digits, number);
                }
            }
            ss << buffer;
        };

        ss << "像素面积: ";
        formatNumber(ss, pixelArea);
        ss << "\n\n实际面积: ";
        formatNumber(ss, realArea);
        ss << " m^2";
        outputArea->copy_label(ss.str().c_str());
    }

    // int crossProduct(double ax, double ay, double bx, double by) {
    //     return ax * by - ay * bx;
    // }

    int orientation(const std::pair<double, double>& p, const std::pair<double, double>& q, const std::pair<double, double>& r) {
        double val = (q.second - p.second) * (r.first - q.first) - (q.first - p.first) * (r.second - q.second);
        if (val == 0) return 0;  // colinear
        return (val > 0) ? 1 : 2; // clock or counterclock wise
    }

    // 检查点 q 是否在线段 pr 上
    bool onSegment(const std::pair<double, double>& p, const std::pair<double, double>& q, const std::pair<double, double>& r) {
        if (q.first <= std::fmax(p.first, r.first) && q.first >= std::fmin(p.first, r.first) &&
            q.second <= std::fmax(p.second, r.second) && q.second >= std::fmin(p.second, r.second)) {
            return true;
        }
        return false;
    }

    // 检查两线段是否相交
    bool segmentsIntersect(const std::pair<double, double>& p1, const std::pair<double, double>& q1,
                        const std::pair<double, double>& p2, const std::pair<double, double>& q2) {
        int o1 = orientation(p1, q1, p2);
        int o2 = orientation(p1, q1, q2);
        int o3 = orientation(p2, q2, p1);
        int o4 = orientation(p2, q2, q1);

        if (o1 != o2 && o3 != o4) return true;

        if (o1 == 0 && onSegment(p1, p2, q1)) return true;
        if (o2 == 0 && onSegment(p1, q2, q1)) return true;
        if (o3 == 0 && onSegment(p2, p1, q2)) return true;
        if (o4 == 0 && onSegment(p2, q1, q2)) return true;

        return false;
    }

    bool isValidPolygon(const std::vector<std::pair<double, double>>& points) {
        size_t n = points.size();
        if (n < 3) return false;  // 至少需要3个点

        bool flag = false;

        // 检查是否所有点都在同一直线上
        for (size_t i = 0; i < n; i++) {
            size_t j = (i + 1) % n, k = (i + 2) % n;
            double x1 = points[i].first, y1 = points[i].second;
            double x2 = points[j].first, y2 = points[j].second;
            double x3 = points[k].first, y3 = points[k].second;
            if ((y2 - y1) * (x3 - x2) != (y3 - y2) * (x2 - x1)) {
                flag = true;
                break;  // 发现一个非共线点
            }
        }
        if (!flag) return false;

        if (checkComplexPoly->value()!=0){
            for (size_t i = 0; i < n; ++i) {
                for (size_t j = 0; j < n; ++j) {
                    if (i != j && (i + 1) % n != j && (j + 1) % n != i) {
                        if (segmentsIntersect(points[i], points[(i + 1) % n], points[j], points[(j + 1) % n])) {
                            return false;
                        }
                    }
                }
            }
        }
        

        return true;
    }


};
int main()
{
    Fl::scheme("gtk+");
    Fl::background(240, 243, 249); 
    Fl::background2(254, 254, 254);
    Fl::visual(FL_DOUBLE|FL_INDEX); 
    Fl::set_font(FL_HELVETICA, "Segoe UI");

    Prj1Window window;
    window.show();
    return Fl::run();
}
