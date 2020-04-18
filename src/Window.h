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
        void Initialise();
        GLfloat GetBufferWidth() const;
        GLfloat GetBufferHeight() const;
        bool getShouldClose();
        bool* getKeys();
        GLfloat getXChange();
        GLfloat getYChange();
        void SwapBuffers();

    private:
        GLFWwindow* window{};
        GLint widht{}, height{};
        GLint bufferWidth{}, bufferHeight{};
        bool keys[1024]{};
        GLfloat lastX{};
        GLfloat lastY{};
        GLfloat xChange{};
        GLfloat yChange{};
        bool mouseFirstMoved{};

        void createCallBacks();
        static void handleKeys(GLFWwindow* _window, int _key, int _code, int _action, int _mode);
        static void handleMouse(GLFWwindow* _window, double _xPos, double _yPos);
};

#endif
