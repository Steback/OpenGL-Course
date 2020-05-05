#include "DirectionalLight.h"

DirectionalLight::DirectionalLight(GLfloat _red, GLfloat _green, GLfloat _blue, GLfloat _ambientIntensity,GLfloat _dIntensity, GLfloat _xDir, GLfloat _yDir, GLfloat _zDir)
        : Light(_red, _green, _blue, _ambientIntensity, _dIntensity), direction(glm::vec3(_xDir, _yDir, _zDir)) {  }

DirectionalLight::~DirectionalLight() = default;

void DirectionalLight::useLight(GLfloat _ambientIntensityLocation, GLfloat _ambienColourLocation, GLfloat _diffuseIntensityLocation, GLfloat _directionLocation) const {
    // glUniform — Specify the value of a uniform variable for the current program object
    glUniform3f(_ambienColourLocation, colour.x, colour.y, colour.z);
    glUniform1f(_ambientIntensityLocation, ambientIntensity);
    glUniform1f(_diffuseIntensityLocation, diffuseIntensity);

    glUniform3f(_directionLocation, direction.x, direction.y, direction.z);
}