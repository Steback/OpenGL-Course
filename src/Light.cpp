#include "Light.h"

Light::Light(GLfloat _red, GLfloat _green, GLfloat _blue, GLfloat _ambientIntensity)
    : colour(glm::vec3(_red, _green, _blue)), ambientIntensity(_ambientIntensity) {  }

Light::~Light() = default;

void Light::useLisht(GLfloat _ambientIntensityLocation, GLfloat _ambienColourLocation) const {
    // glUniform — Specify the value of a uniform variable for the current program object
    glUniform3f(_ambienColourLocation, colour.x, colour.y, colour.z);
    glUniform1f(_ambientIntensityLocation, ambientIntensity);
}