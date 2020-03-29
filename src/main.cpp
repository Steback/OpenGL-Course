#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

// Window dimensions
const GLint WIDTH = 800, HEIGHT = 600;

int main() {
    // Initialise GLFW
    if ( !glfwInit() ) {
        std::cout << "GLFW initialisation failed!" << std::endl;
        glfwTerminate();
        return 1;
    }

    // Setup GLFW window properties
    // OpenGL version
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    // Core profile = No backwards compatibility
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Allow forward compatibility
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "Test Window", nullptr, nullptr);

    if ( !mainWindow ) {
        std::cout << "GLFW Window creation failed" << std::endl;
        glfwTerminate();
        return 1;
    }

    // Get buffer size information
    int bufferWidth, bufferHeight;
    glfwGetFramebufferSize(mainWindow, &bufferHeight, &bufferWidth);

    // Set context for GLEW to use
    glfwMakeContextCurrent(mainWindow);

    // Allow modern extension features
    glewExperimental = GL_TRUE;

    if ( glewInit() != GLEW_OK ) {
        std::cout << "GLEW initialisation failed!" << std::endl;
        glfwDestroyWindow(mainWindow);
        glfwTerminate();
        return 1;
    }

    // Setup Viewport size
    glViewport(0, 0, bufferWidth, bufferHeight);

    // Loop until window closed
    while ( !glfwWindowShouldClose(mainWindow) ) {
        // Get + Handle user input
        glfwPollEvents();

        // Clear Window
        glClearColor(1, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(mainWindow);
    }

    return 0;
}