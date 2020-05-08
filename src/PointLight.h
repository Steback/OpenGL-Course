#ifndef POINTLIGHT_H
#define POINTLIGHT_H

#include <vector>

class Shader;

#include "Light.h"

struct UniformPointLight {
    GLuint uniformColour{};
    GLuint uniformAmbientIntensity{};
    GLuint uniformDiffuseIntensity{};

    GLuint uniformPosition{};
    GLuint uniformConstant{};
    GLuint uniformLinear{};
    GLuint uniformExponent{};
};

class PointLight : public Light  {
    public:
        PointLight(const glm::vec3& _colour, GLfloat _aIntensity, GLfloat _dIntensity, const glm::vec3& _position, GLfloat _con, GLfloat _lin, GLfloat _exp);
        ~PointLight();
        void UseLight(GLuint _ambientIntensityLocation, GLuint _ambienColourLocation, GLuint _diffuseIntensityLocation, GLuint _positionLocation,
                      GLuint _constantLocation, GLuint _linearLocation, GLuint _exponentLocation) const;
        static void GetUPointLight(const Shader& _shader, std::vector<UniformPointLight>& _uPointLight);
        static void SetPointLights(std::vector<PointLight>& _pLight, const std::vector<UniformPointLight>& _uPointLight, GLuint _uPointLightCount);

    protected:
        glm::vec3 position;
        GLfloat constant, linear, exponent;
};

#endif