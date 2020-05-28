#ifndef OPENGL_EXAMPLES_SHADOWMAP_H
#define OPENGL_EXAMPLES_SHADOWMAP_H

#include "glm/glm.hpp"

#include <GL/glew.h>

class Shader;

class ShadowMap {
    public:
        ShadowMap();
        ~ShadowMap();
        virtual bool Init(GLuint _width, GLuint _height);
        virtual void Write();
        virtual void Read(GLenum _textureUnit);
        GLuint GetShadowWidth() const;
        GLuint GetShadowHeight() const;
        static void SetTexture(GLuint _textureUnit, Shader* _shader);
        static void SetDirectionalShadowMap(GLuint _textureUnit, Shader* _shader);
        static void SetDirectionalLightTransform(const glm::mat4& _lTransform, Shader* _shader);

    protected:
        GLuint FBO, shadowMap;
        GLuint shadowWidth, shadowHeight;
};

#endif
