#ifndef OMNI_SHADOW_MAP_H
#define OMNI_SHADOW_MAP_H

#include <vector>

#include "ShadowMap.h"

class shader;

struct UniformOmniShadowMap {
    GLuint shadowMap;
    float farPlane;
};

class OmniShadowMap : public ShadowMap {
    public:
        OmniShadowMap();
        ~OmniShadowMap();
        bool Init(GLuint _width, GLuint _height) override;
        void Write() override;
        void Read(GLenum _textureUnit) override;
        static void SetLightMatrices(Shader* _shader, const std::vector<glm::mat4>& _matrices);
        static void GetUniformsOmniShadowMap(std::vector<UniformOmniShadowMap>& _uOmniShadowMap, Shader* _shader);

    private:

};

#endif