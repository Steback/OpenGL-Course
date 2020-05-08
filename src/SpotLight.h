#ifndef SPOTLIGHT_H
#define SPOTLIGHT_H

#include "PointLight.h"

struct UniformSpotLight {
    GLuint uniformColour;
    GLuint uniformAmbientIntensity;
    GLuint uniformDiffuseIntensity;

    GLuint uniformPosition;
    GLuint uniformConstant;
    GLuint uniformLinear;
    GLuint uniformExponent;

    GLuint uniformDirection;
    GLuint uniformEdge;
};

class SpotLight : public PointLight {
    public:
        SpotLight(const glm::vec3& _colour, GLfloat _aIntensity, GLfloat _dIntensity, const glm::vec3& _position, GLfloat _con, GLfloat _lin, GLfloat _exp,
                  const glm::vec3& _direction, GLfloat _edge);
        ~SpotLight();
        void UseLight(GLuint _ambientIntensityLocation, GLuint _ambienColourLocation, GLuint _diffuseIntensityLocation, GLuint _positionLocation,
                      GLuint _directionLocationGLuint, GLuint _constantLocation, GLuint _linearLocation, GLuint _exponentLocation, GLuint _edgeLocation) const;
        static void GetUPointLight(const Shader& _shader, std::vector<UniformSpotLight>& _uSpotLight);
        static void SetPointLights(std::vector<SpotLight> &_sLight, const std::vector<UniformSpotLight> &_uSpotLight, GLuint _uSpotLightCount);

    private:
        glm::vec3 direction;
        GLfloat edge{}, procEdge{};
};

#endif