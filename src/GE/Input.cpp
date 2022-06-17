#include <GLFW/glfw3.h>

#include "Input.hpp"
#include "Game.hpp"

std::unique_ptr<Input> input = std::make_unique<Input>();

bool Input::keyIsPressed(int key) {
    return glfwGetKey(
        window->getGlfwWindowPtr(),
        key
    );
}

