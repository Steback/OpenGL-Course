#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "stb_image.h"

#include "SkyBox.h"
#include "Shader.h"
#include "Mesh.h"

SkyBox::SkyBox(const std::vector<std::string>& _faceLocations) : textureID() {
    skyShader = std::make_unique<Shader>();
    skyShader->CreateFormFiles("Shaders/SkyBox.vert", "Shaders/SkyBox.frag");

    uniformProjection = skyShader->GetProjectionLocation();
    uniformView = skyShader->GetViewLocation();

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, bitDepth;

    for ( size_t i = 0; i < _faceLocations.size(); i++ ) {
        unsigned char* textureData = stbi_load(_faceLocations[i].c_str(), &width, &height, &bitDepth, 0);

        if ( !textureData ) {
            std::cerr << "Failed to load texture: " << _faceLocations[i] << '\n';

            return ;
        }

        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData);

        stbi_image_free(textureData);
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    skyMesh = std::make_unique<Mesh>();
    skyMesh->CreateMesh( std::vector<Shape> {
            { -1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f },
            { -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f },
            { 1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f },
            { 1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f },

            { -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f },
            { 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f },
            { -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f },
            { 1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0. }
    }, std::vector<GLuint> {
            // front
            0, 1, 2,
            2, 1, 3,
            // right
            2, 3, 5,
            5, 3, 7,
            // back
            5, 7, 4,
            4, 7, 6,
            // left
            4, 6, 0,
            0, 6, 1,
            // top
            4, 0, 5,
            5, 0, 2,
            // bottom
            1, 6, 3,
            3, 6, 7
    } );
}

SkyBox::~SkyBox() = default;

void SkyBox::DrawSkyBox(glm::mat4 _viewMatrix, glm::mat4 _projectionMatrix) {
    _viewMatrix = glm::mat4(glm::mat3(_viewMatrix));

    glDepthMask(GL_FALSE);

    skyShader->UseShader();

    glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(_projectionMatrix));
    glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(_viewMatrix));

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    skyShader->Validate();

    skyMesh->RenderMesh();

    glDepthMask(GL_TRUE);
}
