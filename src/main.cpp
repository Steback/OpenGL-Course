#include <iostream>
#include <cstring>
#include <cmath>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "../lib/glm/glm.hpp"
#include "../lib/glm/gtc/matrix_transform.hpp"
#include "../lib/glm/gtc/type_ptr.hpp"

#include "Mesh.h"
#include "Shader.h"

// Window dimensions
const GLint WIDTH = 800, HEIGHT = 600;
const float toRadians = 3.14159265f / 180.0f;

std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

bool direction = true;
float triOffset  = 0.0f;
float triMaxoffset = 0.7f;
float triIncrement = 0.005f;

float curlAngle = 0.0f;

bool sizeDirection = true;
float curSize = 0.4f;
float maxSize = 0.8f;
float minSize = 0.1f;

// Vertex Shader
std::string vShader = "Shaders/shader.vert";

// Fragment Shader
std::string fShader = "Shaders/shader.frag";

void createObjects() {
    unsigned int indices[] = {
        0, 3, 1,
        1, 3, 2,
        2, 3, 0,
        0, 1, 2
    };

    GLfloat vertices[] {
        -1.0f, -1.0f, 0.0f,
        0.0f, -1.0f, 1.0f,
        1.0f, -1.0f, 0.0f,
        0.0f, 1.0f, 0.0f
    };

    Mesh* mesh = new Mesh();
    mesh->CreateMesh(vertices, indices, 12, 12);
    meshList.push_back(mesh);
}

void CreateShaders() {
    auto* shader1 = new Shader();
    shader1->CreateFormFiles(vShader, fShader);
    shaderList.push_back(*shader1);
}

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
    glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

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

    glEnable(GL_DEPTH_TEST);

    // Setup Viewport size
    glViewport(0, 0, bufferWidth, bufferHeight);

    createObjects();
    CreateShaders();

    GLuint uniformModel = 0, uniformProjection = 0;
    glm::mat4 projection = glm::perspective(45.0f, static_cast<GLfloat>(bufferWidth) / static_cast<GLfloat>(bufferHeight), 0.1f, 100.0f);

    // Loop until window closed
    while ( !glfwWindowShouldClose(mainWindow) ) {
        // Get + Handle user input
        glfwPollEvents();

        if ( direction ) {
            triOffset += triIncrement;
        } else {
            triOffset -= triIncrement;
        }

        if ( std::abs(triOffset) >= triMaxoffset ) {
            direction = !direction;
        }

        curlAngle = curlAngle <= 360 ? curlAngle + 0.5f : curlAngle - 360;

        if ( sizeDirection ) {
            curSize += 0.001f;
        } else {
            curSize -= 0.001f;
        }

        if ( curSize >= maxSize || curSize <= minSize ) {
            sizeDirection = !sizeDirection;
        }

        // Clear Window
        glClearColor(0, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shaderList[0].UseShader();
        uniformModel = shaderList[0].GetModelLocation();
        uniformProjection = shaderList[0].GetProjectionLocation();

        glm::mat4 model(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.5f));
        model = glm::rotate(model, curlAngle * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));

        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));

        meshList[0]->RenderMesh();

        glUseProgram(0);

        glfwSwapBuffers(mainWindow);
    }

    return 0;
}