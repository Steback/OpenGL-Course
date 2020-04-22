#include <iostream>
#include <vector>

#include "../lib/glm/glm.hpp"
#include "../lib/glm/gtc/matrix_transform.hpp"
#include "../lib/glm/gtc/type_ptr.hpp"

#include "Mesh.h"
#include "Shader.h"
#include "Window.h"
#include "Camera.h"
#include "Light.h"

#define STB_IMAGE_IMPLEMENTATION
#include "Texture.h"

std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

auto* shader = new Shader();
Mesh* mesh = new Mesh();
Camera camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 5.0f, 0.5f);

Texture brickTexture("assets/images/brick.png");

Light ligh(0.0f, 0.0f, 1.0f, 0.5f);

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;

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
    //    X      Y     Z     U     V
        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, -1.0f, 1.0f, 0.5f, 0.0f,
        1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.5f, 1.0f
    };

    mesh->CreateMesh(vertices, indices, 20, 12);
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

    brickTexture.LoadTexture();

    GLuint uniformModel = 0, uniformProjection = 0, unifornmView = 0, uniformAmbientIntensity = 0, uniformAmbientColour = 0;
    glm::mat4 projection = glm::perspective(45.0f, static_cast<GLfloat>(window.GetBufferWidth()) / static_cast<GLfloat>(window.GetBufferHeight()), 0.1f, 100.0f);

    // Loop until window closed
    while ( window.getShouldClose() ) {

        // This function returns the value of the GLFW timer.
        GLfloat now = glfwGetTime();
        deltaTime = now - lastTime;
        lastTime = now;

        // This function processes only those events that are already in the event queue and then returns immediately.
        // Processing events will cause the window and input callbacks associated with those events to be called.
        glfwPollEvents();

        camera.KeyControl(window.getKeys(), deltaTime);
        camera.MouseControl(window.getXChange(), window.getYChange());

        // glClearColor — specify clear values for the color buffers
        glClearColor(0, 0, 0, 1);

        // GL_COLOR_BUFFER_BIT - Indicates the buffers currently enabled for color writing.
        // GL_DEPTH_BUFFER_BIT - Indicates the depth buffer.
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shaderList[0].UseShader();
        uniformModel = shaderList[0].GetModelLocation();
        uniformProjection = shaderList[0].GetProjectionLocation();
        unifornmView = shaderList[0].GetViewLocation();
        uniformAmbientIntensity = shaderList[0].GetAmbientIntensityLocation();
        uniformAmbientColour = shaderList[0].GetAmbientColourLocation();

        ligh.useLisht(uniformAmbientIntensity, uniformAmbientColour);

        glm::mat4 model(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, .0f, -2.5f));
        model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));

        // glUniform — Specify the value of a uniform variable for the current program object
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(unifornmView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));

        brickTexture.UserTexture();
        meshList[0]->RenderMesh();

        // glUseProgram — Installs a program object as part of current rendering state
        glUseProgram(0);

        window.SwapBuffers();
    }

    delete mesh;
    delete shader;

    return 0;
}