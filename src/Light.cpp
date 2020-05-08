#include "Light.h"

Light::Light(const glm::vec3& _colour, GLfloat _aIntensity, GLfloat _dIntensity)
        : colour(_colour), ambientIntensity(_aIntensity), diffuseIntensity(_dIntensity) {  }

Light::~Light() = default;