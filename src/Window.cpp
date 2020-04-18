#include "Window.h"

Window::Window() : widht(800), height(600) {  }

Window::Window(GLint _windowWidth, GLint _windowHeight) : widht(_windowWidth), height(_windowHeight) {  }

Window::~Window() {
    glfwDestroyWindow(window);
    glfwTerminate();
}

void Window::Initialise() {
    // Initialise GLFW
    if ( !glfwInit() ) {
        std::cout << "GLFW initialisation failed!" << std::endl;
        glfwTerminate();
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
    }

    // Get buffer size information
    glfwGetFramebufferSize(window, &bufferWidth, &bufferHeight);

    // Hadle Key + Mouse input
    createCallBacks();

    // Set context for GLEW to use
    glfwMakeContextCurrent(window);

    // Allow modern extension features
    glewExperimental = GL_TRUE;

    if ( glewInit() != GLEW_OK ) {
        std::cout << "GLEW initialisation failed!" << std::endl;
        glfwDestroyWindow(window);
        glfwTerminate();
    }

    glEnable(GL_DEPTH_TEST);

    // Setup Viewport size
    glViewport(0, 0, bufferWidth, bufferHeight);

    glfwSetWindowUserPointer(window, this);
}

GLfloat Window::GetBufferWidth() const { return bufferWidth; }

GLfloat Window::GetBufferHeight() const { return bufferHeight; }

bool Window::getShouldClose() { return !glfwWindowShouldClose(window); }

void Window::SwapBuffers() { glfwSwapBuffers(window); }

void Window::createCallBacks() {
    glfwSetKeyCallback(window, handleKeys);
    glfwSetCursorPosCallback(window, handleMouse);
}

void Window::handleKeys(GLFWwindow *_window, int _key, int _code, int _action, int _mode) {
    auto* w = static_cast<Window*>(glfwGetWindowUserPointer(_window));

    if ( _key == GLFW_KEY_ESCAPE && _action == GLFW_PRESS ) {
        glfwSetWindowShouldClose(_window, GL_TRUE);
    }

    if ( _key >= 0 && _key < 1024 ) {
        if ( _action == GLFW_PRESS ) {
            w->keys[_key] = true;
        } else if ( _action == GLFW_RELEASE ) {
            w->keys[_key] = false;
        }
    }
}

void Window::handleMouse(GLFWwindow *_window, double _xPos, double _yPos) {
    auto* w = static_cast<Window*>(glfwGetWindowUserPointer(_window));

    if ( w->mouseFirstMoved ) {
        w->lastX = _xPos;
        w->lastY = _yPos;
        w->mouseFirstMoved = false;
    }

    w->xChange = _xPos - w->lastX;
    w->yChange = w->lastY - _yPos;

    w->lastX = _xPos;
    w->lastY = _yPos;

    printf("x: %.6f, y:%.6f\n", w->xChange, w->yChange);
}