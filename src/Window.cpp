#include "Window.h"

Window::Window() : widht(800), height(600), xChange(0.0f), yChange(0.0f) {  }

Window::Window(GLint _windowWidth, GLint _windowHeight) : widht(_windowWidth), height(_windowHeight), xChange(0.0f), yChange(0.0f) {  }

Window::~Window() {
    // This function destroys the specified window and its context.
    glfwDestroyWindow(window);

    // Terminates the GLFW library.
    glfwTerminate();
}

void Window::Initialise() {
    // 	Initializes the GLFW library
    if ( !glfwInit() ) {
        std::cout << "GLFW initialisation failed!" << std::endl;
        glfwTerminate();
    }

    // This function sets hints for the next call to glfwCreateWindow.
    // GLFW_CONTEXT_VERSION_MAJOR - Context client API major version hint and attribute
    // GLFW_CONTEXT_VERSION_MINOR - Context client API minor version hint and attribute.
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);

    // GLFW_OPENGL_PROFILE - OpenGL profile hint and attribute.
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Allow forward compatibility
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // This function creates a window and its associated OpenGL or OpenGL ES context.
    window = glfwCreateWindow(widht, height, "Test Window", nullptr, nullptr);

    if ( !window ) {
        std::cout << "GLFW Window creation failed" << std::endl;
        glfwTerminate();
    }

    // This function retrieves the size, in pixels, of the framebuffer of the specified window.
    glfwGetFramebufferSize(window, &bufferWidth, &bufferHeight);

    // Hadle Key + Mouse input
    createCallBacks();

    // This function sets an input mode option for the specified window.
    // GLFW_CURSOR - the value must be one of the following cursor modes:
    // GLFW_CURSOR_DISABLED - hides and grabs the cursor, providing virtual and unlimited cursor movement. This is useful for implementing for example 3D camera controls.
//    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // This function makes the OpenGL or OpenGL ES context of the specified window current on the calling thread.
    glfwMakeContextCurrent(window);

    // Allow modern extension features
    glewExperimental = GL_TRUE;

    // initialize the extension entry points.
    if ( glewInit() != GLEW_OK ) {
        std::cout << "GLEW initialisation failed!" << std::endl;
        glfwDestroyWindow(window);
        glfwTerminate();
    }

    // glEnable — enable or disable server-side GL capabilities
    glEnable(GL_DEPTH_TEST);

    // Setup Viewport size
    glViewport(0, 0, bufferWidth, bufferHeight);

    // glViewport specifies the affine transformation of x and y from normalized device coordinates to window coordinates. Let x nd y nd be normalized device coordinates.
    glfwSetWindowUserPointer(window, this);
}

GLfloat Window::GetBufferWidth() const { return bufferWidth; }

GLfloat Window::GetBufferHeight() const { return bufferHeight; }

bool Window::getShouldClose() { return !glfwWindowShouldClose(window); }

bool* Window::getKeys() { return keys; }

GLfloat Window::getXChange() {
    GLfloat c = xChange;
    xChange = 0.0f;
    return c;
}

GLfloat Window::getYChange() {
    GLfloat c = yChange;
    yChange = 0.0f;
    return c;
}

void Window::SwapBuffers() { glfwSwapBuffers(window); }

void Window::createCallBacks() {
    // This function sets the key callback of the specified window, which is called when a key is pressed, repeated or released.
    glfwSetKeyCallback(window, handleKeys);

    // This function sets the cursor position callback of the specified window, which is called when the cursor is moved.
    glfwSetCursorPosCallback(window, handleMouse);
}

void Window::handleKeys(GLFWwindow *_window, int _key, int _code, int _action, int _mode) {
    // This function returns the current value of the user-defined pointer of the specified window.
    auto* w = static_cast<Window*>(glfwGetWindowUserPointer(_window));

    if ( _key == GLFW_KEY_ESCAPE && _action == GLFW_PRESS ) {
        // This function sets the value of the close flag of the specified window.
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
}