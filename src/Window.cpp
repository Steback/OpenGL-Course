#include "Window.h"

Window::Window() : widht(800), height(600) {  }

Window::Window(GLint _windowWidth, GLint _windowHeight) : widht(_windowWidth), height(_windowHeight) {  }

Window::~Window() {
    glfwDestroyWindow(window);
    glfwTerminate();
}

int Window::Initialise() {
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

    window = glfwCreateWindow(widht, height, "Test Window", nullptr, nullptr);

    if ( !window ) {
        std::cout << "GLFW Window creation failed" << std::endl;
        glfwTerminate();
        return 1;
    }

    // Get buffer size information
    glfwGetFramebufferSize(window, &bufferWidth, &bufferHeight);

    // Set context for GLEW to use
    glfwMakeContextCurrent(window);

    // Allow modern extension features
    glewExperimental = GL_TRUE;

    if ( glewInit() != GLEW_OK ) {
        std::cout << "GLEW initialisation failed!" << std::endl;
        glfwDestroyWindow(window);
        glfwTerminate();
        return 1;
    }

    glEnable(GL_DEPTH_TEST);

    // Setup Viewport size
    glViewport(0, 0, bufferWidth, bufferHeight);
}

GLint Window::GetBufferWidth() { return bufferWidth; }

GLint Window::GetBufferHeight() { return bufferHeight; }

bool Window::getShouldClose() { return !glfwWindowShouldClose(window); }

void Window::SwapBuffers() { glfwSwapBuffers(window); }
