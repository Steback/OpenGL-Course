#include <vector>

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

#define STB_IMAGE_IMPLEMENTATION
#include "Texture.h"

std::vector<Mesh*> meshList;
std::vector<Shader*> shaderList;

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;

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
    Material dullMaterial(0.3f, 4);

    DirectionalLight directionalLight(glm::vec3(1.0f, 1.0f, 1.0f), 0.1f, 0.1f, glm::vec3(0.0f, 0.0f, -1.0f));

    UniformDirectionalLight uniformDirectionalLight{};
    DirectionalLight::GetUDirectionalLight(*shaderList[0], uniformDirectionalLight);

    std::vector<PointLight> pointLights {
            { glm::vec3(0.0f, 0.0f, 1.0f), 0.0f, 0.1f,
              glm::vec3(0.0f, 0.0f, 0.0f), 0.3f, 0.2f, 0.1f },
            { glm::vec3(0.0f, 1.0f, 0.0f), 0.0f, 0.1f,
              glm::vec3(-4.0f, 2.0f, 0.0f), 0.3f, 0.1f, 0.1f }
    };

    std::vector<UniformPointLight> uniformPointLight(MAX_POINT_LIGHTS);
    PointLight::GetUPointLight(*shaderList[0], uniformPointLight);

    std::vector<SpotLight> spotLights {
            { glm::vec3(1.0f, 1.0f, 1.0f), 0.0f, 2.0f, glm::vec3(0.0f, 0.0f, 0.0f),
              1.0f, 0.0f, 0.0f, glm::vec3(0.0f, -1.0f, 0.0f), 20.0f },
    };

    std::vector<UniformSpotLight> uniformSpotLight(MAX_POINT_LIGHTS);
    SpotLight::GetUPointLight(*shaderList[0], uniformSpotLight);

    GLuint uniformModel = 0, uniformProjection = 0, unifornmView = 0, uniformEyePosition = 0;
    GLuint uniformSpecularIntesity = 0, uniformShininess = 0;

    glm::mat4 projection = glm::perspective(45.0f, static_cast<GLfloat>(window.GetBufferWidth()) / static_cast<GLfloat>(window.GetBufferHeight()),
            0.1f, 100.0f);

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
        uniformEyePosition = shaderList[0]->GetUniformLocation("eyePosition");
        uniformSpecularIntesity = shaderList[0]->GetUniformLocation("material.specularIntensity");
        uniformShininess = shaderList[0]->GetUniformLocation("material.shininess");

        DirectionalLight::SetDirectionalLight(directionalLight, uniformDirectionalLight);
        PointLight::SetPointLights(pointLights, uniformPointLight, shaderList[0]->GetUniformLocation("pointLightCount"));
        SpotLight::SetPointLights(spotLights, uniformSpotLight, shaderList[0]->GetUniformLocation("spotLightCount"));

        glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(unifornmView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
        glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

        glm::mat4 model(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.5f));
//        model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        brickTexture.UserTexture();
        dullMaterial.UseMateril(uniformSpecularIntesity, uniformShininess);
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