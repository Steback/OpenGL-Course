#include "Light.h"

Light::Light(GLfloat _red, GLfloat _green, GLfloat _blue, GLfloat _ambientIntensity, GLfloat _dIntensity)
    : colour(glm::vec3(_red, _green, _blue)), ambientIntensity(_ambientIntensity), diffuseIntensity(_dIntensity) {  }

Light::~Light() = default;