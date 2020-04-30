#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <iostream>

#include <GL/glew.h>

#include "DirectionalLight.h"
#include "PointLight.h"

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
        GLuint GetAmbientIntensityLocation() const;
        GLuint GetAmbientColourLocation() const;
        GLuint GetDiffuseIntensityLocation() const;
        GLuint GetDirectionLocation() const;
        void UseShader() const;
        void ClearShader();

    private:
        int pointLight{};
        GLuint shaderID{};
        GLuint uniformProjection{};
        GLuint uniformModel{};
        GLuint uniformView{};
        GLuint uniformAmbientIntensity{};
        GLuint uniformAmbientColour{};
        GLuint uniformDiffuseIntensity{};
        GLuint uniformDirection{};
        GLint result{};
        GLchar eLog[1024]{};

        void CompileShader(std::string& _vertexCode, std::string& _fragmentCode);
        void AddShader(GLuint _program, std::string& _shaderCode, GLenum _shaderType);
};

#endif
