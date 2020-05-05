#ifndef SPOTLIGHT_H
#define SPOTLIGHT_H

#include "PointLight.h"
#include "Shader.h"

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
        SpotLight(GLfloat _red, GLfloat _green, GLfloat _blue, GLfloat _ambientIntensity, GLfloat _dIntensity, GLfloat _xPos, GLfloat _yPos, GLfloat _zPos,
                GLfloat _xDir, GLfloat _yDir, GLfloat _zDir, GLfloat _cons, GLfloat _lin, GLfloat _exp, GLfloat _edge);
        ~SpotLight();
        void useLight(GLuint _ambientIntensityLocation, GLuint _ambienColourLocation, GLuint _diffuseIntensityLocation, GLuint _positionLocation,
                      GLuint _directionLocationGLuint, GLuint _constantLocation, GLuint _linearLocation, GLuint _exponentLocation, GLuint _edgeLocation) const;
        static void getUniformLocation(Shader* _shader, std::vector<UniformSpotLight>& _uniformSpotLight);
        static void setSpotLights(Shader* _shader, const std::vector<SpotLight>& _sLight, const std::vector<UniformSpotLight>& _uniformSpotLight, unsigned int _spotLightCount);

    private:
        glm::vec3 direction;
        GLfloat edge{}, procEdge{};
};

#endif