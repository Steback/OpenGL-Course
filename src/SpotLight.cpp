#include <cstdio>

#include "SpotLight.h"
#include "Shader.h"

SpotLight::SpotLight(const glm::vec3& _colour, GLfloat _aIntensity, GLfloat _dIntensity, const glm::vec3& _position, GLfloat _con, GLfloat _lin, GLfloat _exp,
                     const glm::vec3& _direction, GLfloat _edge)
        : PointLight(_colour, _aIntensity, _dIntensity, _position, _con, _lin, _exp), direction(glm::normalize(_direction)),
          edge(_edge), procEdge(cosf(glm::radians(_edge))) {  }

SpotLight::~SpotLight() = default;

void SpotLight::UseLight(GLuint _ambientIntensityLocation, GLuint _ambienColourLocation, GLuint _diffuseIntensityLocation, GLuint _positionLocation,
        GLuint _directionLocationGLuint, GLuint _constantLocation, GLuint _linearLocation, GLuint _exponentLocation, GLuint _edgeLocation) const {
    glUniform3f(_ambienColourLocation, colour.x, colour.y, colour.z);
    glUniform1f(_ambientIntensityLocation, ambientIntensity);
    glUniform1f(_diffuseIntensityLocation, diffuseIntensity);

    glUniform3f(_positionLocation, position.x, position.y, position.z);
    glUniform1f(_constantLocation, constant);
    glUniform1f(_linearLocation, linear);
    glUniform1f(_exponentLocation, exponent);

    glUniform3f(_directionLocationGLuint, direction.x, direction.y, direction.z);
    glUniform1f(_edgeLocation, procEdge);
}

void SpotLight::GetUPointLight(const Shader &_shader, std::vector<UniformSpotLight> &_uSpotLight) {
    for (size_t i = 0; i < _uSpotLight.size(); i++) {
        char locBuff[100] = { '\0' };

        snprintf(locBuff, sizeof(locBuff), "spotLights[%zu].base.base.colour", i);
        _uSpotLight[i].uniformColour = _shader.GetUniformLocation(locBuff);

        snprintf(locBuff, sizeof(locBuff), "spotLights[%zu].base.base.ambientIntensity", i);
        _uSpotLight[i].uniformAmbientIntensity = _shader.GetUniformLocation(locBuff);

        snprintf(locBuff, sizeof(locBuff), "spotLights[%zu].base.base.diffuseIntensity", i);
        _uSpotLight[i].uniformDiffuseIntensity = _shader.GetUniformLocation(locBuff);

        snprintf(locBuff, sizeof(locBuff), "spotLights[%zu].base.position", i);
        _uSpotLight[i].uniformPosition = _shader.GetUniformLocation(locBuff);

        snprintf(locBuff, sizeof(locBuff), "spotLights[%zu].base.constant", i);
        _uSpotLight[i].uniformConstant = _shader.GetUniformLocation(locBuff);

        snprintf(locBuff, sizeof(locBuff), "spotLights[%zu].base.linear", i);
        _uSpotLight[i].uniformLinear = _shader.GetUniformLocation(locBuff);

        snprintf(locBuff, sizeof(locBuff), "spotLights[%zu].base.exponent", i);
        _uSpotLight[i].uniformExponent = _shader.GetUniformLocation(locBuff);

        snprintf(locBuff, sizeof(locBuff), "spotLights[%zu].direction", i);
        _uSpotLight[i].uniformDirection = _shader.GetUniformLocation(locBuff);

        snprintf(locBuff, sizeof(locBuff), "spotLights[%zu].edge", i);
        _uSpotLight[i].uniformEdge = _shader.GetUniformLocation(locBuff);
    }
}

void SpotLight::SetPointLights(std::vector<SpotLight> &_sLight, const std::vector<UniformSpotLight> &_uSpotLight, GLuint _uSpotLightCount) {
    glUniform1i(_uSpotLightCount, _sLight.size());

    for ( size_t i = 0; i < _sLight.size(); i++ ) {
        _sLight[i].UseLight(_uSpotLight[i].uniformAmbientIntensity, _uSpotLight[i].uniformColour,
                            _uSpotLight[i].uniformDiffuseIntensity, _uSpotLight[i].uniformPosition, _uSpotLight[i].uniformDirection,
                            _uSpotLight[i].uniformConstant, _uSpotLight[i].uniformLinear, _uSpotLight[i].uniformExponent,
                            _uSpotLight[i].uniformEdge);
    }
}

void SpotLight::SetFlash(glm::vec3 _pos, glm::vec3 _dir) {
    position = _pos;
    direction = _dir;
}