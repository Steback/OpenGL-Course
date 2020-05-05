#include <iostream>
#include <vector>

#include "../lib/glm/glm.hpp"
#include "../lib/glm/gtc/matrix_transform.hpp"
#include "../lib/glm/gtc/type_ptr.hpp"

#include "Mesh.h"
#include "Window.h"
#include "Camera.h"
#include "DirectionalLight.h"
#include "Material.h"
#include "Constans.h"
#include "PointLight.h"
#include "SpotLight.h"

#define STB_IMAGE_IMPLEMENTATION
#include "Texture.h"

std::vector<Mesh*> meshList;
std::vector<Shader*> shaderList;

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

    unsigned int floorIndices[] = {
            0, 2, 1,
            1, 2, 3
    };

    GLfloat floorVertices[] = {
            -10.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
            10.0f, 0.0f, -10.0f,	10.0f, 0.0f,	0.0f, -1.0f, 0.0f,
            -10.0f, 0.0f, 10.0f,	0.0f, 10.0f,	0.0f, -1.0f, 0.0f,
            10.0f, 0.0f, 10.0f,		10.0f, 10.0f,	0.0f, -1.0f, 0.0f
    };

    calcAverageNormals(indices, 12, vertices, 32, 8, 5);

    Mesh* mesh = new Mesh();
    mesh->CreateMesh(vertices, indices, 32, 12);
    meshList.push_back(mesh);

    Mesh* mesh2 = new Mesh();
    mesh2->CreateMesh(floorVertices, floorIndices, 32, 6);
    meshList.push_back(mesh2);
}

void CreateShaders() {
    auto shader = new Shader();
    shader->CreateFormFiles( "Shaders/shader.vert", "Shaders/shader.frag");
    shaderList.push_back(shader);
}

int main() {
    Window window(1366, 768);
    window.Initialise();

    createObjects();
    CreateShaders();

    Camera camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 5.0f, 0.5f);

    Texture brickTexture("assets/images/brick.png");
    brickTexture.LoadTexture();

    Texture plainTexture("assets/images/plain.png");
    plainTexture.LoadTexture();

    Material shinyMaterial(4.0f, 256);

    unsigned int pointLightCount = 0, spotLightCount = 0;

    DirectionalLight directionalLight(1.0f, 1.0f, 1.0f, 0.1f, 0.1f, 0.0f, 0.0f, -1.0f);

    std::vector<PointLight> pointLights {
            { 0.0f, 0.0f, 1.0f, 0.0f, 0.1f, 0.0f, 0.0f, 0.0f, 0.3f, 0.2f, 0.1f },
            { 0.0f, 1.0f, 0.0f, 0.0f, 0.1f, -4.0f, 2.0f, 0.0f, 0.3f, 0.1f, 0.1f }
    };

    pointLightCount = pointLights.size();


    std::vector<SpotLight> spotLights {
            { 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
              0.0f, 1.0f, 0.0f, 0.3f, 0.2f, 0.1f, 20.0f },
    };

    spotLightCount = spotLights.size();

    GLuint uniformModel = 0, uniformProjection = 0, unifornmView = 0, uniformEyePosition = 0;
    GLuint uniformSpecularIntesity = 0, uniformShininess = 0, uniformPointLightCount = 0, uniformSpotLightCount;

    glm::mat4 projection = glm::perspective(45.0f, static_cast<GLfloat>(window.GetBufferWidth()) / static_cast<GLfloat>(window.GetBufferHeight()), 0.1f, 100.0f);

    UniformDirectionalLight uniformDirectionalLight{};
    std::vector<UniformPointLight> uniformPointLight(MAX_POINT_LIGHTS);
    std::vector<UniformSpotLight> uniformSpotLight(MAX_SPOT_LIGHTS);

    for (size_t i = 0; i < MAX_POINT_LIGHTS; i++) {
        char locBuff[100] = { '\0' };

        snprintf(locBuff, sizeof(locBuff), "pointLights[%zu].base.colour", i);
        uniformPointLight[i].uniformColour = shaderList[0]->GetUniformLocation(locBuff);

        snprintf(locBuff, sizeof(locBuff), "pointLights[%zu].base.ambientIntensity", i);
        uniformPointLight[i].uniformAmbientIntensity = shaderList[0]->GetUniformLocation(locBuff);

        snprintf(locBuff, sizeof(locBuff), "pointLights[%zu].base.diffuseIntensity", i);
        uniformPointLight[i].uniformDiffuseIntensity = shaderList[0]->GetUniformLocation(locBuff);

        snprintf(locBuff, sizeof(locBuff), "pointLights[%zu].position", i);
        uniformPointLight[i].uniformPosition = shaderList[0]->GetUniformLocation(locBuff);

        snprintf(locBuff, sizeof(locBuff), "pointLights[%zu].constant", i);
        uniformPointLight[i].uniformConstant = shaderList[0]->GetUniformLocation(locBuff);

        snprintf(locBuff, sizeof(locBuff), "pointLights[%zu].linear", i);
        uniformPointLight[i].uniformLinear = shaderList[0]->GetUniformLocation(locBuff);

        snprintf(locBuff, sizeof(locBuff), "pointLights[%zu].exponent", i);
        uniformPointLight[i].uniformExponent = shaderList[0]->GetUniformLocation(locBuff);
    }

    SpotLight::getUniformLocation(shaderList[0], uniformSpotLight);

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

        shaderList[0]->UseShader();
        uniformModel = shaderList[0]->GetModelLocation();
        uniformProjection = shaderList[0]->GetProjectionLocation();
        unifornmView = shaderList[0]->GetViewLocation();
        uniformDirectionalLight.uniformColour = shaderList[0]->GetUniformLocation("directionalLight.base.colour");
        uniformDirectionalLight.uniformAmbientIntensity = shaderList[0]->GetUniformLocation("directionalLight.base.ambientIntensity");
        uniformDirectionalLight.uniformDirection = shaderList[0]->GetUniformLocation("directionalLight.direction");
        uniformDirectionalLight.uniformDiffuseIntensity = shaderList[0]->GetUniformLocation("directionalLight.base.diffuseIntensity");
        uniformEyePosition = shaderList[0]->GetUniformLocation("eyePosition");
        uniformSpecularIntesity = shaderList[0]->GetUniformLocation("mateial.specularIntensity");
        uniformShininess = shaderList[0]->GetUniformLocation("mateial.shininess");
        uniformPointLightCount = shaderList[0]->GetUniformLocation("pointLightCount");

        directionalLight.useLight(uniformDirectionalLight.uniformAmbientIntensity,  uniformDirectionalLight.uniformColour,
                                 uniformDirectionalLight.uniformDiffuseIntensity, uniformDirectionalLight.uniformDirection);

        if ( pointLightCount > MAX_POINT_LIGHTS ) pointLightCount = MAX_POINT_LIGHTS;

        glUniform1i(uniformPointLightCount, pointLightCount);

        for ( size_t i = 0; i < pointLightCount; i++ ) {
            pointLights[i].useLight(uniformPointLight[i].uniformAmbientIntensity, uniformPointLight[i].uniformColour,
                                    uniformPointLight[i].uniformDiffuseIntensity, uniformPointLight[i].uniformPosition,
                                    uniformPointLight[i].uniformConstant, uniformPointLight[i].uniformLinear, uniformPointLight[i].uniformExponent);
        }

        SpotLight::setSpotLights(shaderList[0], spotLights, uniformSpotLight, spotLightCount);

        glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(unifornmView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
        glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

        glm::mat4 model(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.5f));
//        model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        brickTexture.UserTexture();
        shinyMaterial.UseMateril(uniformSpecularIntesity, uniformShininess);
        meshList[0]->RenderMesh();

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
//        model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        plainTexture.UserTexture();
        shinyMaterial.UseMateril(uniformSpecularIntesity, uniformShininess);
        meshList[1]->RenderMesh();

        // glUseProgram — Installs a program object as part of current rendering state
        glUseProgram(0);

        window.SwapBuffers();
    }

    for ( auto& mesh : meshList ) {
        delete mesh;
    }

    for ( auto& shader : shaderList ) {
        delete shader;
    }

    return 0;
}