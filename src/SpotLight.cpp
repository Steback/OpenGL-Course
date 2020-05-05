#include "SpotLight.h"
#include "Constans.h"

SpotLight::SpotLight(GLfloat _red, GLfloat _green, GLfloat _blue, GLfloat _ambientIntensity, GLfloat _dIntensity, GLfloat _xPos, GLfloat _yPos, GLfloat _zPos,
        GLfloat _xDir, GLfloat _yDir, GLfloat _zDir, GLfloat _cons, GLfloat _lin, GLfloat _exp, GLfloat _edge)
        : PointLight(_red, _green, _blue, _ambientIntensity, _dIntensity, _xPos, _yPos, _zPos, _cons, _lin, _exp),
        direction(glm::vec3(_xDir, _yDir, _zDir)), edge(_edge), procEdge(glm::radians(_edge)) {  }

SpotLight::~SpotLight() = default;

void SpotLight::useLight(GLuint _ambientIntensityLocation, GLuint _ambienColourLocation, GLuint _diffuseIntensityLocation, GLuint _positionLocation,
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

void SpotLight::getUniformLocation(Shader *_shader, std::vector<UniformSpotLight>& _uniformSpotLight) {
    for (size_t i = 0; i < _uniformSpotLight.size(); i++) {
        char locBuff[100] = { '\0' };

        snprintf(locBuff, sizeof(locBuff), "pointLights[%zu].base.base.colour", i);
        _uniformSpotLight[i].uniformColour = _shader->GetUniformLocation(locBuff);

        snprintf(locBuff, sizeof(locBuff), "pointLights[%zu].base.base.ambientIntensity", i);
        _uniformSpotLight[i].uniformAmbientIntensity = _shader->GetUniformLocation(locBuff);

        snprintf(locBuff, sizeof(locBuff), "pointLights[%zu].base.base.diffuseIntensity", i);
        _uniformSpotLight[i].uniformDiffuseIntensity = _shader->GetUniformLocation(locBuff);

        snprintf(locBuff, sizeof(locBuff), "pointLights[%zu].base.position", i);
        _uniformSpotLight[i].uniformPosition = _shader->GetUniformLocation(locBuff);

        snprintf(locBuff, sizeof(locBuff), "pointLights[%zu].base.constant", i);
        _uniformSpotLight[i].uniformConstant = _shader->GetUniformLocation(locBuff);

        snprintf(locBuff, sizeof(locBuff), "pointLights[%zu].base.linear", i);
        _uniformSpotLight[i].uniformLinear = _shader->GetUniformLocation(locBuff);

        snprintf(locBuff, sizeof(locBuff), "pointLights[%zu].base.exponent", i);
        _uniformSpotLight[i].uniformExponent = _shader->GetUniformLocation(locBuff);

        snprintf(locBuff, sizeof(locBuff), "pointLights[%zu].direction", i);
        _uniformSpotLight[i].uniformDirection = _shader->GetUniformLocation(locBuff);

        snprintf(locBuff, sizeof(locBuff), "pointLights[%zu].edge", i);
        _uniformSpotLight[i].uniformEdge = _shader->GetUniformLocation(locBuff);
    }
}

void SpotLight::setSpotLights(Shader* _shader, const std::vector<SpotLight>& _sLight, const std::vector<UniformSpotLight> &_uniformSpotLight, unsigned int _spotLightCount) {
    if ( _spotLightCount > MAX_POINT_LIGHTS ) _spotLightCount = MAX_POINT_LIGHTS;

    glUniform1i(_shader->GetUniformLocation("spotLightCount"), _spotLightCount);

    for ( size_t i = 0; i < _spotLightCount; i++ ) {
        _sLight[i].useLight(_uniformSpotLight[i].uniformAmbientIntensity, _uniformSpotLight[i].uniformColour, _uniformSpotLight[i].uniformDiffuseIntensity,
                _uniformSpotLight[i].uniformPosition, _uniformSpotLight[i].uniformDirection, _uniformSpotLight[i].uniformConstant, _uniformSpotLight[i].uniformLinear,
                _uniformSpotLight[i].uniformExponent, _uniformSpotLight[i].uniformEdge);
    }
}