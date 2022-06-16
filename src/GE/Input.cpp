#include <GLFW/glfw3.h>

#include "Input.hpp"
#include "Game.hpp"
bool Input::keyIsPressed(int key) {
    return glfwGetKey(
        window->getGlfwWindowPtr(),
        key
    );
}