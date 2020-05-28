#ifndef LIGHT_H
#define LIGHT_H

#include <GL/glew.h>

#include "glm/glm.hpp"

class ShadowMap;

class Light {
    public:
        explicit Light(GLuint _width, GLuint _height, const glm::vec3& _colour, GLfloat _aIntensity, GLfloat _dIntensity);
        ~Light();
        ShadowMap* GetShadowMap() const;

    protected:
        glm::vec3 colour;
        GLfloat ambientIntensity;
        GLfloat diffuseIntensity;
        ShadowMap* shadowMap;
        glm::mat4 lightProj;
};

#endif