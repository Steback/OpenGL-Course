#ifndef DIRECTIONALLIGHT_H
#define DIRECTIONALLIGHT_H

class Shader;

#include "Light.h"

struct UniformDirectionalLight {
    GLuint uniformColour;
    GLuint uniformAmbientIntensity;
    GLuint uniformDiffuseIntensity;

    GLuint uniformDirection;
};

class DirectionalLight : public Light {
    public:
        DirectionalLight(GLuint _width, GLuint _height, const glm::vec3& _colour, GLfloat _aIntensity, GLfloat _dIntensity, const glm::vec3& _direction);
        ~DirectionalLight();
        void UseLight(GLfloat _ambientIntensityLocation, GLfloat _ambienColourLocation, GLfloat _diffuseIntensityLocation, GLfloat _directionLocation) const;
        static void GetUDirectionalLight(const Shader& _shader, UniformDirectionalLight& _uDirectionalLight);
        static void SetDirectionalLight(DirectionalLight& _dLight, const UniformDirectionalLight& _uDirectionalLight);
        glm::mat4 CalcLightTransform();

    private:
        glm::vec3 direction;
};

#endif