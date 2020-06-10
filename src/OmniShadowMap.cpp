#include <iostream>

#include "glm/gtc/type_ptr.hpp"

#include "OmniShadowMap.h"
#include "Shader.h"

OmniShadowMap::OmniShadowMap() : ShadowMap() {  }

OmniShadowMap::~OmniShadowMap() = default;

bool OmniShadowMap::Init(GLuint _width, GLuint _height) {
    shadowWidth = _width; shadowHeight = _height;

    glGenFramebuffers(1, &FBO);

    glGenTextures(1, &shadowMap);
    glBindTexture(GL_TEXTURE_CUBE_MAP, shadowMap);

    for ( size_t i = 0; i < 6; ++i ) {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, shadowWidth, shadowHeight,
                0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    }

    glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, shadowMap, 0);

    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);

    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

    if ( status != GL_FRAMEBUFFER_COMPLETE ) {
        std::cerr << "Framebuffer Error: " << status << '\n';
        return false;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    return true;
}

void OmniShadowMap::Write() {
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
}

void OmniShadowMap::Read(GLenum _textureUnit) {
    glActiveTexture(_textureUnit);
    glBindTexture(GL_TEXTURE_CUBE_MAP, shadowMap);
}

void OmniShadowMap::SetLightMatrices(Shader *_shader, const std::vector<glm::mat4>& _matrices) {
    for ( size_t i = 0; i < 6; ++i ) {
        std::string lightMatrixLocation = "lightMatrices[" + std::to_string(i) + "]";
        glUniformMatrix4fv( _shader->GetUniformLocation(lightMatrixLocation), 1, GL_FALSE, glm::value_ptr(_matrices[i]) );
    }
}

void OmniShadowMap::GetUniformsOmniShadowMap(std::vector<UniformOmniShadowMap> &_uOmniShadowMap, Shader *_shader) {
    for ( size_t i = 0; i < _uOmniShadowMap.size(); i++ ) {
        std::string uniform = "omniShadowMaps[" + std::to_string(i) + "].shadowMap";
        _uOmniShadowMap[i].shadowMap = _shader->GetUniformLocation(uniform);

        uniform = "omniShadowMaps[" + std::to_string(i) + "].farPlane";
        _uOmniShadowMap[i].farPlane = _shader->GetUniformLocation(uniform);
    }
}