#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Game.hpp"
#include "Render/Shader.hpp"

double delta_time {};

void G_loop() {
    uint32_t fps {};
    float next_second = 1;
    double time_last_frame {};
    while (!glfwWindowShouldClose(window->getGlfwWindowPtr())) {
        /* Render here */
        
        delta_time = glfwGetTime()-time_last_frame;
        time_last_frame = glfwGetTime();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        world.get()->updateWorld();

        /* Swap front and back buffers */
        glfwSwapBuffers(window->getGlfwWindowPtr());

        /* Poll for and process events */
        glfwPollEvents();

        if (double t = glfwGetTime(); t > next_second) {
            std::cout << "fps: " << fps << std::endl;
            next_second = t+1;
            fps = 0;
        }
        fps++;
    }
}

void G_init() {
    glfwSwapInterval(1);
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
