#include <GLFW/glfw3.h>
#include <array>

#include "Input.hpp"
#include "Window.hpp"
#include "Utility.hpp"

std::unique_ptr<Input> input = std::make_unique<Input>();
static std::array<uint8_t, 512> keys;

static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    auto& k = keys[key];
    if (action != GLFW_RELEASE) {
        if (k == 0) {
            k = 1;
        }
    } else {
        k = 0;
    }
}

Input::Input() {
    glfwSetKeyCallback(window->getGlfwWindowPtr(), key_callback);
}

bool Input::getKey(int key) {
    return keys[key] > 0;
}

bool Input::getKeyDown(int key) {
    auto& k = keys[key];
    bool pressed = k == 1;
    if (pressed) {
        k++;
    }

    return pressed;
}

