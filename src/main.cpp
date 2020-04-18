#include <iostream>
#include <cmath>
#include <vector>

#include "../lib/glm/glm.hpp"
#include "../lib/glm/gtc/matrix_transform.hpp"
#include "../lib/glm/gtc/type_ptr.hpp"

#include "Mesh.h"
#include "Shader.h"
#include "Window.h"
#include "Camera.h"

std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

auto* shader = new Shader();
Mesh* mesh = new Mesh();
Camera camera(glm::vec3(0.0f, 0.0f, -5.0f), glm::vec3(0.0f, 1.0f, 0.0f), 90.0f, 0.0f, 0.1f, 1.0f);

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

    mesh->CreateMesh(vertices, indices, 12, 12);
    meshList.push_back(mesh);
}

void CreateShaders() {
    shader->CreateFormFiles(vShader, fShader);
    shaderList.push_back(*shader);
}

int main() {
    Window window(800, 600);
    window.Initialise();

    createObjects();
    CreateShaders();

    GLuint uniformModel = 0, uniformProjection = 0, unifornmView = 0;
    glm::mat4 projection = glm::perspective(45.0f, static_cast<GLfloat>(window.GetBufferWidth()) / static_cast<GLfloat>(window.GetBufferHeight()), 0.1f, 100.0f);

    // Loop until window closed
    while ( window.getShouldClose() ) {
        // Get + Handle user input
        glfwPollEvents();

        camera.KeyControl(window.getKeys());

        // Clear Window
        glClearColor(0, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shaderList[0].UseShader();
        uniformModel = shaderList[0].GetModelLocation();
        uniformProjection = shaderList[0].GetProjectionLocation();
        unifornmView = shaderList[0].GetViewLocation();

        glm::mat4 model(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.5f));
//        model = glm::rotate(model, 0.0f, glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(unifornmView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
        meshList[0]->RenderMesh();

        glUseProgram(0);

        window.SwapBuffers();
    }

    delete mesh;
    delete shader;

    return 0;
}