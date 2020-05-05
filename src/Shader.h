#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <iostream>
#include <vector>

#include <GL/glew.h>

struct UniformDirectionalLight {
    GLuint uniformColour;
    GLuint uniformAmbientIntensity;
    GLuint uniformDiffuseIntensity;

    GLuint uniformDirection;
};

struct UniformPointLight {
    GLuint uniformColour;
    GLuint uniformAmbientIntensity;
    GLuint uniformDiffuseIntensity;

    GLuint uniformPosition;
    GLuint uniformConstant;
    GLuint uniformLinear;
    GLuint uniformExponent;
};

class Shader {
    public:
        Shader();
        ~Shader();
        void CreateFromString(std::string& _vertexCode, std::string& _fragmentCode);
        void CreateFormFiles(const std::string& _vertexFilePath, const std::string& _fragmentFilePath);
        static std::string ReadFile(const std::string& _filePath);
        GLuint GetUniformLocation(const std::string& _name) const;
        GLuint GetProjectionLocation() const;
        GLuint GetModelLocation() const;
        GLuint GetViewLocation() const;
        void UseShader() const;
        void ClearShader();

    private:
        GLuint shaderID{};
        GLuint uniformProjection{};
        GLuint uniformModel{};
        GLuint uniformView{};
        GLint result{};
        GLchar eLog[1024]{};

        void CompileShader(std::string& _vertexCode, std::string& _fragmentCode);
        void AddShader(GLuint _program, std::string& _shaderCode, GLenum _shaderType);
};

#endif
