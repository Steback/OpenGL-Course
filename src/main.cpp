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
#include "Material.h"

#define STB_IMAGE_IMPLEMENTATION
#include "Texture.h"

std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

auto* shader = new Shader();
Mesh* mesh = new Mesh();
Camera camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 5.0f, 0.5f);

Texture brickTexture("assets/images/brick.png");

Light ligh(1.0f, 1.0f, 1.0f, 0.2f, 2.0f, -1.0f, -2.0f, 1.0f);

Material shinyMaterial(1.0f, 32);
Material dullMaterial(0.3f, 4);

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;

void calcAverageNormals(const unsigned int* _indices, unsigned int _indicesCount, GLfloat* _vertices,
        unsigned int _verticesCount, unsigned int _vLenght, unsigned int _normalOffset) {
    for ( unsigned int i = 0; i < _indicesCount; i += 3 ) {
        unsigned int in0 = _indices[i] * _vLenght;
        unsigned int in1 = _indices[i + 1] * _vLenght;
        unsigned int in2 = _indices[i + 2] * _vLenght;

        glm::vec3 v1(_vertices[in1] - _vertices[in0], _vertices[in1 + 1] - _vertices[in0 + 1], _vertices[in1+ 2] - _vertices[in0+ 2]);
        glm::vec3 v2(_vertices[in2] - _vertices[in0], _vertices[in2 + 1] - _vertices[in0 + 1], _vertices[in2+ 2] - _vertices[in0+ 2]);
        glm::vec3 normal = glm::normalize(glm::cross(v1, v2));

        in0 += _normalOffset;
        in1 += _normalOffset;
        in2 += _normalOffset;

        _vertices[in0] += normal.x;
        _vertices[in0 + 1] += normal.y;
        _vertices[in0 + 2] += normal.z;

        _vertices[in1] += normal.x;
        _vertices[in1 + 1] += normal.y;
        _vertices[in1 + 2] += normal.z;

        _vertices[in2] += normal.x;
        _vertices[in2 + 1] += normal.y;
        _vertices[in2 + 2] += normal.z;
    }

    for ( unsigned int i = 0; i < _verticesCount / _vLenght; i++ ) {
        unsigned int nOffset = i * _vLenght + _normalOffset;
        glm::vec3 vec = glm::normalize(glm::vec3(_vertices[nOffset], _vertices[nOffset + 1], _vertices[nOffset + 2]));

        _vertices[nOffset] = vec.x;
        _vertices[nOffset + 1] = vec.y;
        _vertices[nOffset + 2] = vec.z;
    }
}

void createObjects() {
    unsigned int indices[] = {
        0, 3, 1,
        1, 3, 2,
        2, 3, 0,
        0, 1, 2
    };

    GLfloat vertices[] {
    //    X      Y     Z     U     V     NX    NY    NZ
        -1.0f, -1.0f, -0.6f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        0.0f, -1.0f, 1.0f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f,
        1.0f, -1.0f, -0.6f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f
    };

    calcAverageNormals(indices, 12, vertices, 32, 8, 5);

    mesh->CreateMesh(vertices, indices, 32, 12);
    meshList.push_back(mesh);
}

void CreateShaders() {
    shader->CreateFormFiles( "Shaders/shader.vert", "Shaders/shader.frag");
    shaderList.push_back(*shader);
}

int main() {
    Window window(1366, 768);
    window.Initialise();

    createObjects();
    CreateShaders();

    brickTexture.LoadTexture();

    GLuint uniformModel = 0, uniformProjection = 0, unifornmView = 0, uniformEyePosition = 0;
    GLuint uniformAmbientIntensity = 0, uniformAmbientColour = 0;
    GLuint uniformDirection = 0, uniformDiffuseIntensity = 0;
    GLuint uniformSpecularIntesity = 0, uniformShininess = 0;
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
        uniformDirection = shaderList[0].GetDirectionLocation();
        uniformDiffuseIntensity = shaderList[0].GetDiffuseIntensityLocation();
        uniformEyePosition = shaderList[0].GetUniformLocation("eyePosition");
        uniformSpecularIntesity = shaderList[0].GetUniformLocation("mateial.specularIntesnity");
        uniformShininess = shaderList[0].GetUniformLocation("mateial.shininess");

        ligh.useLisht(uniformAmbientIntensity, uniformAmbientColour, uniformDiffuseIntensity, uniformDirection);

        glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(unifornmView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
        glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

        glm::mat4 model(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 4.0f, -2.5f));
//        model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));

        // glUniform — Specify the value of a uniform variable for the current program object
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

        brickTexture.UserTexture();
        shinyMaterial.UseMateril(uniformSpecularIntesity, uniformShininess);
//        dullMaterial.UseMateril(uniformSpecularIntesity, uniformShininess);
        meshList[0]->RenderMesh();

        // glUseProgram — Installs a program object as part of current rendering state
        glUseProgram(0);

        window.SwapBuffers();
    }

    delete mesh;
    delete shader;

    return 0;
}