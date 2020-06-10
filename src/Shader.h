#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <iostream>
#include <vector>

#include <GL/glew.h>

class Shader {
    public:
        Shader();
        ~Shader();
        void CreateFormFiles(const std::string& _vertexFilePath, const std::string& _fragmentFilePath);
        void CreateFormFiles(const std::string &_vertexFilePath,  const std::string &_geometryFilePath, const std::string &_fragmentFilePath);
        static std::string ReadFile(const std::string& _filePath);
        GLuint GetUniformLocation(const std::string& _name) const;
        GLuint GetProjectionLocation() const;
        GLuint GetModelLocation() const;
        GLuint GetViewLocation() const;
        void UseShader() const;
        void ClearShader();
        void Validate() const;

    private:
        GLuint shaderID{};
        GLuint uniformProjection{};
        GLuint uniformModel{};
        GLuint uniformView{};

        void CompileShader(std::string& _vertexCode, std::string& _fragmentCode);
        void CompileShader(std::string& _vertexCode, std::string& _geometryCode, std::string& _fragmentCode);
        static void AddShader(GLuint _program, std::string& _shaderCode, GLenum _shaderType);
        void CompileProgram();
};

#endif
