#include "glm/gtc/matrix_transform.hpp"

#include "DirectionalLight.h"
#include "Shader.h"

DirectionalLight::DirectionalLight(GLfloat _width, GLfloat _height, const glm::vec3& _colour, GLfloat _aIntensity, GLfloat _dIntensity, const glm::vec3& _direction)
        : Light(_width, _height, _colour, _aIntensity, _dIntensity), direction(_direction) {
    lightProj = glm::ortho(-5.0f, 5.0f, -5.0f, 5.0f, 0.1f, 20.0f);
}

DirectionalLight::~DirectionalLight() = default;

void DirectionalLight::UseLight(GLfloat _ambientIntensityLocation, GLfloat _ambienColourLocation, GLfloat _diffuseIntensityLocation, GLfloat _directionLocation) const {
    // glUniform — Specify the value of a uniform variable for the current program object
    glUniform3f(_ambienColourLocation, colour.x, colour.y, colour.z);
    glUniform1f(_ambientIntensityLocation, ambientIntensity);
    glUniform1f(_diffuseIntensityLocation, diffuseIntensity);

    glUniform3f(_directionLocation, direction.x, direction.y, direction.z);
}

void DirectionalLight::GetUDirectionalLight(const Shader& _shader, UniformDirectionalLight& _uDirectionalLight) {
    _uDirectionalLight.uniformColour = _shader.GetUniformLocation("directionalLight.base.colour");
    _uDirectionalLight.uniformAmbientIntensity = _shader.GetUniformLocation("directionalLight.base.ambientIntensity");
    _uDirectionalLight.uniformDirection = _shader.GetUniformLocation("directionalLight.direction");
    _uDirectionalLight.uniformDiffuseIntensity = _shader.GetUniformLocation("directionalLight.base.diffuseIntensity");
}

void DirectionalLight::SetDirectionalLight(DirectionalLight &_dLight, const UniformDirectionalLight &_uDirectionalLight) {
    _dLight.UseLight(_uDirectionalLight.uniformAmbientIntensity, _uDirectionalLight.uniformColour,
                     _uDirectionalLight.uniformDiffuseIntensity, _uDirectionalLight.uniformDirection);
}

glm::mat4 DirectionalLight::CalcLightTransform() {
    return lightProj * glm::lookAt(-direction, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
}