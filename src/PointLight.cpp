#include <cstdio>

#include "PointLight.h"
#include "Shader.h"

PointLight::PointLight(const glm::vec3& _colour, GLfloat _aIntensity, GLfloat _dIntensity, const glm::vec3& _position, GLfloat con, GLfloat lin, GLfloat exp)
        : Light(1024, 1024, _colour, _aIntensity, _dIntensity), position(_position), constant(con), linear(lin), exponent(exp) {  }

PointLight::~PointLight() = default;

void PointLight::UseLight(GLuint _ambientIntensityLocation, GLuint _ambienColourLocation, GLuint _diffuseIntensityLocation,
                          GLuint _positionLocation, GLuint _constantLocation, GLuint _linearLocation, GLuint _exponentLocation) const {
    // glUniform — Specify the value of a uniform variable for the current program object
    glUniform3f(_ambienColourLocation, colour.x, colour.y, colour.z);
    glUniform1f(_ambientIntensityLocation, ambientIntensity);
    glUniform1f(_diffuseIntensityLocation, diffuseIntensity);

    glUniform3f(_positionLocation, position.x, position.y, position.z);
    glUniform1f(_constantLocation, constant);
    glUniform1f(_linearLocation, linear);
    glUniform1f(_exponentLocation, exponent);
}

void PointLight::GetUPointLight(const Shader &_shader, std::vector<UniformPointLight> &_uPointLight) {
    for ( size_t i = 0; i < _uPointLight.size(); i++ ) {
        char locBuff[100] = { '\0' };

        snprintf(locBuff, sizeof(locBuff), "pointLights[%zu].base.colour", i);
        _uPointLight[i].uniformColour = _shader.GetUniformLocation(locBuff);

        snprintf(locBuff, sizeof(locBuff), "pointLights[%zu].base.ambientIntensity", i);
        _uPointLight[i].uniformAmbientIntensity = _shader.GetUniformLocation(locBuff);

        snprintf(locBuff, sizeof(locBuff), "pointLights[%zu].base.diffuseIntensity", i);
        _uPointLight[i].uniformDiffuseIntensity = _shader.GetUniformLocation(locBuff);

        snprintf(locBuff, sizeof(locBuff), "pointLights[%zu].position", i);
        _uPointLight[i].uniformPosition = _shader.GetUniformLocation(locBuff);

        snprintf(locBuff, sizeof(locBuff), "pointLights[%zu].constant", i);
        _uPointLight[i].uniformConstant = _shader.GetUniformLocation(locBuff);

        snprintf(locBuff, sizeof(locBuff), "pointLights[%zu].linear", i);
        _uPointLight[i].uniformLinear = _shader.GetUniformLocation(locBuff);

        snprintf(locBuff, sizeof(locBuff), "pointLights[%zu].exponent", i);
        _uPointLight[i].uniformExponent = _shader.GetUniformLocation(locBuff);
    }
}

void PointLight::SetPointLights(std::vector<PointLight> &_pLight, const std::vector<UniformPointLight> &_uPointLight, GLuint _uPointLightCount) {
    glUniform1i(_uPointLightCount, _pLight.size());

    for ( size_t i = 0; i < _pLight.size(); i++ ) {
        _pLight[i].UseLight(_uPointLight[i].uniformAmbientIntensity, _uPointLight[i].uniformColour,
                            _uPointLight[i].uniformDiffuseIntensity, _uPointLight[i].uniformPosition,
                            _uPointLight[i].uniformConstant, _uPointLight[i].uniformLinear, _uPointLight[i].uniformExponent);
    }
}