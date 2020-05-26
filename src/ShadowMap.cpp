#include <iostream>

#include "glm/gtc/type_ptr.hpp"

#include "ShadowMap.h"

ShadowMap::ShadowMap() : FBO(0), shadowMap(0), shadowWidth(0), shadowHeight(0) {  }

ShadowMap::~ShadowMap() {
    if ( FBO ) {
        glDeleteFramebuffers(1, &FBO);
    }

    if ( shadowMap ) {
        glDeleteTextures(1, &shadowMap);
    }
}

bool ShadowMap::Init(GLuint _width, GLuint _height) {
    shadowWidth = _width; shadowHeight = _height;

    glGenFramebuffers(1, &FBO);

    glGenTextures(1, &shadowMap);
    glBindTexture(GL_TEXTURE_2D, shadowMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, shadowWidth, shadowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT,nullptr);

    glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowMap, 0);

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

void ShadowMap::Write() {
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
}

void ShadowMap::Read(GLenum _textureUnit) {
    glActiveTexture(_textureUnit);
    glBindTexture(GL_TEXTURE_2D, shadowMap);
}

GLuint ShadowMap::GetShadowWidth() const { return shadowWidth; }

GLuint ShadowMap::GetShadowHeight() const { return shadowHeight; }

void ShadowMap::SetTexture(GLuint _textureUnit, GLuint _uTexture) {
    glUniform1f(_uTexture, _textureUnit);
}

void ShadowMap::SetDirectionalShadowMap(GLuint _textureUnit, GLuint _uDirectionalShadowMap) {
    glUniform1f(_uDirectionalShadowMap, _textureUnit);
}

void ShadowMap::SetDirectionalLightTransform(const glm::mat4& _lTransform, GLuint _uDirectionalLightTransform) {
    glUniformMatrix4fv(_uDirectionalLightTransform, 1, GL_FALSE, glm::value_ptr(_lTransform));
}