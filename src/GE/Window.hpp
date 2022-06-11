#pragma once 
#include <string_view>
#include <stdint.h>

class GLFWwindow;

class Window {
    GLFWwindow* window;
    bool fullScreen;
    struct Size {
        int width;
        int height;
    };
    Size size;
    
public:
    Window(const std::string_view title="None", const uint16_t width=640, const uint16_t height=480);
    void setSize(int w, int h);
    void onResize(int w, int h);
    GLFWwindow* getGlfwWindowPtr() const;
    [[nodiscard]] Size getSize() const;
    void toggleFullScreen();
    ~Window();
};
