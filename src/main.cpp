#include <vector>
#include <memory>

#include "../lib/glm/glm.hpp"
#include "../lib/glm/gtc/matrix_transform.hpp"
#include "../lib/glm/gtc/type_ptr.hpp"

#include "Mesh.h"
#include "Window.h"
#include "Camera.h"
#include "Shader.h"
#include "DirectionalLight.h"
#include "Material.h"
#include "Constans.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Texture.h"
#include "Model.h"

const float toRadians = 3.14159265f / 180.0f;

std::unique_ptr<Window> window;

std::vector<Mesh*> meshList;
std::vector<Shader*> shaderList;

std::unique_ptr<Camera> camera;

std::unique_ptr<Texture> brickTexture;
std::unique_ptr<Texture> plainTexture;

std::unique_ptr<Material> shinyMaterial;
std::unique_ptr<Material> dullMaterial;

DirectionalLight* directionalLight;
UniformDirectionalLight* uniformDirectionalLight;

std::vector<PointLight> pointLights;
std::vector<UniformPointLight> uniformPointLight;

std::vector<SpotLight> spotLights;
std::vector<UniformSpotLight> uniformSpotLight;

std::unique_ptr<Model> xwing;
std::unique_ptr<Model> blackhack;

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;

GLuint uniformModel = 0, uniformProjection = 0, unifornmView = 0, uniformEyePosition = 0;
GLuint uniformSpecularIntesity = 0, uniformShininess = 0;

void calcAverageNormals(const std::vector<GLuint>& indices, std::vector<Shape>& vertices) {
    for ( size_t i = 0; i < indices.size(); i += 3 ) {
        unsigned int in0 = indices[i];
        unsigned int in1 = indices[i + 1];
        unsigned int in2 = indices[i + 2];

        glm::vec3 v1(vertices[in1].position.x - vertices[in0].position.x,
                     vertices[in1].position.y - vertices[in0].position.y,
                     vertices[in1].position.z - vertices[in0].position.z);

        glm::vec3 v2(vertices[in2].position.x - vertices[in0].position.x,
                     vertices[in2].position.y - vertices[in0].position.y,
                     vertices[in2].position.z - vertices[in0].position.z);

        glm::vec3 normal = glm::cross(v1, v2);
        normal = glm::normalize(normal);

        vertices[in0].normal.x += normal.x; vertices[in0].normal.y += normal.y; vertices[in0].normal.z += normal.z;
        vertices[in1].normal.x += normal.x; vertices[in1].normal.y += normal.y; vertices[in1].normal.z += normal.z;
        vertices[in2].normal.x += normal.x; vertices[in2].normal.y += normal.y; vertices[in2].normal.z += normal.z;
    }

    for (auto & vertice : vertices)
    {
        glm::vec3 vec(vertice.normal.x, vertice.normal.y, vertice.normal.z);
        vec = glm::normalize(vec);
        vertice.normal.x = vec.x; vertice.normal.y = vec.y; vertice.normal.z = vec.z;
    }
}

void createObjects() {
    std::vector<GLuint> indices {
            0, 3, 1,
            1, 3, 2,
            2, 3, 0,
            0, 1, 2
    };

    std::vector<Shape> vertices {
            {-1.0f, -1.0f, -0.6f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f },
            { 0.0f, -1.0f, 1.0f,	0.5f, 0.0f,	0.0f, 0.0f, 0.0f },
            { 1.0f, -1.0f, -0.6f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f },
            { 0.0f, 1.0f, 0.0f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f }
    };

    std::vector<GLuint> floorIndices {
            0, 2, 1,
            1, 2, 3
    };

    std::vector<Shape> floorVertices {
            {-10.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f},
            {10.0f, 0.0f, -10.0f,	10.0f, 0.0f,	0.0f, -1.0f, 0.0f},
            {-10.0f, 0.0f, 10.0f,	0.0f, 10.0f,	0.0f, -1.0f, 0.0f},
            {10.0f, 0.0f, 10.0f,		10.0f, 10.0f,	0.0f, -1.0f, 0.0f}
    };

    calcAverageNormals(indices,vertices);

    Mesh* mesh = new Mesh();
    mesh->CreateMesh(vertices, indices);
    meshList.push_back(mesh);

    Mesh* mesh2 = new Mesh();
    mesh2->CreateMesh(floorVertices, floorIndices);
    meshList.push_back(mesh2);
}

void CreateShaders() {
    auto shader = new Shader();
    shader->CreateFormFiles( "Shaders/shader.vert", "Shaders/shader.frag");
    shaderList.push_back(shader);
}

void RenderScene() {
    glm::mat4 model(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.5f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
    brickTexture->UseTexture();
    shinyMaterial->UseMateril(uniformSpecularIntesity, uniformShininess);
    meshList[0]->RenderMesh();

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
    plainTexture->UseTexture();
    dullMaterial->UseMateril(uniformSpecularIntesity, uniformShininess);
    meshList[1]->RenderMesh();

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-12.0f, 0.0f, 12.0f));
    model = glm::scale(model, glm::vec3(0.01f, 0.01f, 0.01f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
    shinyMaterial->UseMateril(uniformSpecularIntesity, uniformShininess);
    xwing->RenderModel();

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, 5.0f));
    model = glm::rotate(model, -90.0f * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
    shinyMaterial->UseMateril(uniformSpecularIntesity, uniformShininess);
    blackhack->RenderModel();
}

int main() {
    window = std::make_unique<Window>(1366, 768);
    window->Initialise();

    createObjects();
    CreateShaders();

    camera = std::make_unique<Camera>(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 5.0f, 0.5f);

    brickTexture = std::make_unique<Texture>("Textures/brick.png");
    brickTexture->LoadTextureA();

    plainTexture = std::make_unique<Texture>("Textures/dirt.png");
    plainTexture->LoadTextureA();

    shinyMaterial = std::make_unique<Material>(4.0f, 256);
    dullMaterial = std::make_unique<Material>(0.3f, 4);

    xwing = std::make_unique<Model>();
    xwing->LoadModel("Models/x-wing.obj");

    blackhack = std::make_unique<Model>();
    blackhack->LoadModel("Models/uh60.obj");

    directionalLight = new DirectionalLight(glm::vec3(1.0f, 1.0f, 1.0f), 0.3f, 0.6f, glm::vec3(0.0f, 0.0f, -1.0f));

    uniformDirectionalLight = new UniformDirectionalLight();
    DirectionalLight::GetUDirectionalLight(*shaderList[0], *uniformDirectionalLight);

    pointLights = {
            { glm::vec3(0.0f, 0.0f, 1.0f), 0.0f, 0.1f,
              glm::vec3(0.0f, 0.0f, 0.0f), 0.3f, 0.2f, 0.1f },
            { glm::vec3(0.0f, 1.0f, 0.0f), 0.0f, 0.1f,
              glm::vec3(-4.0f, 2.0f, 0.0f), 0.3f, 0.1f, 0.1f }
    };

    uniformPointLight = std::vector<UniformPointLight>(MAX_POINT_LIGHTS);
    PointLight::GetUPointLight(*shaderList[0], uniformPointLight);

    spotLights = {
            { glm::vec3(1.0f, 1.0f, 1.0f), 0.0f, 2.0f, glm::vec3(0.0f, 0.0f, 0.0f),
              1.0f, 0.0f, 0.0f, glm::vec3(0.0f, -1.0f, 0.0f), 20.0f },
            { glm::vec3(1.0f, 1.0f, 1.0f), 0.0f, 1.0f, glm::vec3(0.0f, -1.5f, 0.0f),
              1.0f, 0.0f, 0.0f, glm::vec3(-100.0f, -1.0f, 0.0f), 20.0f }
    };

    uniformSpotLight = std::vector<UniformSpotLight>(MAX_POINT_LIGHTS);
    SpotLight::GetUPointLight(*shaderList[0], uniformSpotLight);

    glm::mat4 projection = glm::perspective(45.0f, static_cast<GLfloat>(window->GetBufferWidth()) / static_cast<GLfloat>(window->GetBufferHeight()),
            0.1f, 100.0f);

    // Loop until window closed
    while ( window->getShouldClose() ) {

        // This function returns the value of the GLFW timer.
        GLfloat now = glfwGetTime();
        deltaTime = now - lastTime;
        lastTime = now;

        // This function processes only those events that are already in the event queue and then returns immediately.
        // Processing events will cause the window and input callbacks associated with those events to be called.
        glfwPollEvents();

        camera->KeyControl(window->getKeys(), deltaTime);
        camera->MouseControl(window->getXChange(), window->getYChange());

        // glClearColor — specify clear values for the color buffers
        glClearColor(0, 0, 0, 1);

        // GL_COLOR_BUFFER_BIT - Indicates the buffers currently enabled for color writing.
        // GL_DEPTH_BUFFER_BIT - Indicates the depth buffer.
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shaderList[0]->UseShader();
        uniformModel = shaderList[0]->GetModelLocation();
        uniformProjection = shaderList[0]->GetProjectionLocation();
        unifornmView = shaderList[0]->GetViewLocation();
        uniformEyePosition = shaderList[0]->GetUniformLocation("eyePosition");
        uniformSpecularIntesity = shaderList[0]->GetUniformLocation("material.specularIntensity");
        uniformShininess = shaderList[0]->GetUniformLocation("material.shininess");

//        spotLights[0].SetFlash(camera.getCameraPosition(), camera.getCameraDirection());

        DirectionalLight::SetDirectionalLight(*directionalLight, *uniformDirectionalLight);
        PointLight::SetPointLights(pointLights, uniformPointLight, shaderList[0]->GetUniformLocation("pointLightCount"));
        SpotLight::SetPointLights(spotLights, uniformSpotLight, shaderList[0]->GetUniformLocation("spotLightCount"));

        glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(unifornmView, 1, GL_FALSE, glm::value_ptr(camera->calculateViewMatrix()));
        glUniform3f(uniformEyePosition, camera->getCameraPosition().x, camera->getCameraPosition().y, camera->getCameraPosition().z);

        RenderScene();

        // glUseProgram — Installs a program object as part of current rendering state
        glUseProgram(0);

        window->SwapBuffers();
    }

    for ( auto& mesh : meshList ) {
        delete mesh;
    }

    for ( auto& shader : shaderList ) {
        delete shader;
    }

    delete directionalLight;
    delete uniformDirectionalLight;

    return 0;
}