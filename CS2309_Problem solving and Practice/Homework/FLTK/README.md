## CS2309 GUI hw

UNIkeEN

### 编译流程

以第一次作业第1题（超椭圆）为例

#### 1.引入依赖

提交的代码不包含 FLTK 的静态链接库等文件，您需要首先：

* 编译 FLTK 源码，并以如下结构放置文件在 `fltk-1.3.8-dist/`目录下；您可以在 [此处](https://github.com/w4ngzhen/fltk-demo) 下载他人编译并编排的文件。

* 将教师提供的 `revise-gui/` 目录放置在 `gui-hw1-1/src/` 目录下。

完成依赖库导入后，文件目录应该类似如下：

```
├─fltk-1.3.8-dist
│  ├─include
│  │  └─FL
│  │          Fl.H
│  │          ...
│  │          x.H
│  │
│  └─lib
│      ├─Linux-release
│      │      libfltk.a
│      │      libfltk_forms.a
│      │      libfltk_gl.a
│      │      libfltk_images.a
│      │
│      └─Windows-release
│              fltk.lib
│              fltk_forms.lib
│              fltk_gl.lib
│              fltk_images.lib
│              fltk_jpeg.lib
│              fltk_png.lib
│              fltk_z.lib
│
├─gui-hw1-1
│  │  CMakeLists.txt
│  │
│  ├─screenshot
│  │      T1_1.png
│  │      T1_2.png
│  │      T1_3.png
│  │
│  └─src
│      │  superellipse.hpp
│      │  T1.cpp
│      │
│      └─revised-gui
│              Graph.cpp
│              ...
```

#### 2.完成编译

我撰写了 `CMakeList.txt`，可以执行下方命令以编译。

```
cd gui-hw1-1
cmake -Bbuild
```

* 对于 Windows，接下来需要打开 `build/` 下生成的 `.sln` 文件，使用 Visual Studio 生成解决方案（无需对 Visual Studio 进行其他设置）

* 对于 Linux，接下来执行：

    ```
    cd build
    make
    ```

本代码已在 Windows 11, Visual Studio 2022, CMake 3.28-rc5 下成功构建，`CMakeList.txt` 虽然支持 linux 下的构建，但未经测试。如您使用的是 MinGW、ninja，也无法保证成功构建。

如有任何问题，请联系我~