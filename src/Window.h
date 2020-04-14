#ifndef WINDOW_H
#define WINDOW_H

#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Window {
    public:
        Window();
        Window(GLint _windowWidth, GLint _windowHeight);
        ~Window();
        int Initialise();
        GLint GetBufferWidth();
        GLint GetBufferHeight();
        bool getShouldClose();
        void SwapBuffers();

    private:
        GLFWwindow* window{};
        GLint widht{}, height{};
        GLint bufferWidth{}, bufferHeight{};
};

#endif
