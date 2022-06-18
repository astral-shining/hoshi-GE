#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Game.hpp"
#include "Render/Shader.hpp"

void G_loop() {
    static uint32_t fps {};
    float nextSecond = 1;
    while (!glfwWindowShouldClose(window->getGlfwWindowPtr())) {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        world.get()->updateWorld();

        /* Swap front and back buffers */
        glfwSwapBuffers(window->getGlfwWindowPtr());

        /* Poll for and process events */
        glfwPollEvents();

        if (double t = glfwGetTime(); t > nextSecond) {
            std::cout << fps << std::endl;
            nextSecond = t+1;
            fps = 0;
        }
        fps++;
    }
}

void G_init() {
    glfwSwapInterval(0);
    if (!gladLoadGLES2Loader((GLADloadproc) glfwGetProcAddress)) {
        terminate("ERROR: Initializing glad");
    } 

    if (!GLAD_GL_ES_VERSION_3_0) {
        terminate("ERROR: Opengl 3.0 not supported");
    }

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);  

    //Shader::compileAll();
}

int G_initialization = [] () {
    G_init();
    return 0;
}();