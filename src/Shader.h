#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <iostream>
#include <fstream>

#include <GL/glew.h>

class Shader {
    public:
        Shader();
        ~Shader();
        void CreateFromString(std::string& _vertexCode, std::string& _fragmentCode);
        void CreateFormFiles(std::string& _vertexFilePath, std::string& _fragmentFilePath);
        static std::string ReadFile(std::string& _filePath);
        GLuint GetProjectionLocation();
        GLuint GetModelLocation();
        void UseShader();
        void ClearShader();

    private:
        GLuint shaderID{};
        GLuint uniformProjection{};
        GLuint uniformModel{};
        GLint result{};
        GLchar eLog[1024]{};

        void CompileShader(std::string& _vertexCode, std::string& _fragmentCode);
        void AddShader(GLuint _program, std::string& _shaderCode, GLenum _shaderType);
};

#endif
