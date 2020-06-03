#include "Light.h"
#include "ShadowMap.h"

Light::Light(GLuint _width, GLuint _height, const glm::vec3& _colour, GLfloat _aIntensity, GLfloat _dIntensity)
        : colour(_colour), ambientIntensity(_aIntensity), diffuseIntensity(_dIntensity) {
    shadowMap = std::make_shared<ShadowMap>();
    shadowMap->Init(_width, _height);
}

Light::~Light() = default;

std::shared_ptr<ShadowMap> Light::GetShadowMap() const { return shadowMap; }