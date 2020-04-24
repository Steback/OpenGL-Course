#include "Light.h"

Light::Light(GLfloat _red, GLfloat _green, GLfloat _blue, GLfloat _ambientIntensity, GLfloat _xDir, GLfloat _yDir, GLfloat _zDir, GLfloat _dIntensity)
    : colour(glm::vec3(_red, _green, _blue)), ambientIntensity(_ambientIntensity), direction(glm::vec3(_xDir, _yDir, _zDir)), diffuseIntensity(_dIntensity) {  }

Light::~Light() = default;

void Light::useLisht(GLfloat _ambientIntensityLocation, GLfloat _ambienColourLocation, GLfloat _diffuseIntensityLocation, GLfloat _directionLocation) const {
    // glUniform — Specify the value of a uniform variable for the current program object
    glUniform3f(_ambienColourLocation, colour.x, colour.y, colour.z);
    glUniform1f(_ambientIntensityLocation, ambientIntensity);

    glUniform3f(_directionLocation, direction.x, direction.y, direction.z);
    glUniform1f(_diffuseIntensityLocation, diffuseIntensity);
}