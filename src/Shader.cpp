#include "Shader.h"

Shader::Shader() = default;

Shader::~Shader() { ClearShader(); };

void Shader::CreateFromString(std::string &_vertexCode, std::string &_fragmentCode) {
    CompileShader(_vertexCode, _fragmentCode);
}

void Shader::CreateFormFiles(std::string &_vertexFilePath, std::string &_fragmentFilePath) {
    std::string vertexString = ReadFile(_vertexFilePath);
    std::string fragmentString = ReadFile(_fragmentFilePath);

    CompileShader(vertexString, fragmentString);
}

std::string Shader::ReadFile(std::string &_filePath) {
    std::string content;
    std::fstream fileStream(_filePath, std::ios::in);

    if ( !fileStream.is_open() ) {
        std::cerr << "Fail to read " << _filePath << " file" << std::endl;
        return "";
    }

    std::string line;

    while ( !fileStream.eof() ) {
        std::getline(fileStream, line);
        content.append(line + "\n");
    }

    fileStream.close();

    return content;
}

GLuint Shader::GetProjectionLocation() { return uniformProjection; }

GLuint Shader::GetModelLocation() { return uniformModel; }

void Shader::UseShader() {
    glUseProgram(shaderID);
}

void Shader::ClearShader() {
    if ( shaderID != 0 ) {
        glDeleteProgram(shaderID);
    }

    uniformModel = 0;
    uniformProjection = 0;
}

void Shader::CompileShader(std::string &_vertexCode, std::string &_fragmentCode) {
    shaderID = glCreateProgram();

    if ( !shaderID ) {
        std::cerr << "Error creating shader program" << std::endl;
        return ;
    }

    AddShader(shaderID, _vertexCode, GL_VERTEX_SHADER);
    AddShader(shaderID, _fragmentCode, GL_FRAGMENT_SHADER);

    glLinkProgram(shaderID);
    glGetProgramiv(shaderID, GL_LINK_STATUS, &result);

    if ( !result ) {
        glGetProgramInfoLog(shaderID, sizeof(eLog), nullptr, eLog);
        std::cerr << "Error linking program: " << eLog << std::endl;
        return ;
    }

    glValidateProgram(shaderID);
    glGetProgramiv(shaderID, GL_VALIDATE_STATUS, &result);

    if ( !result ) {
        glGetProgramInfoLog(shaderID, sizeof(eLog), nullptr, eLog);
        std::cerr << "Error validating program: " << eLog << std::endl;
        return ;
    }

    uniformModel = glGetUniformLocation(shaderID, "model");
    uniformProjection = glGetUniformLocation(shaderID, "projection");
}

void Shader::AddShader(GLuint _program, std::string& _shaderCode, GLenum _shaderType) {
    GLuint newShader = glCreateShader(_shaderType);

    const GLchar* shaderCode[1];
    shaderCode[0] = _shaderCode.c_str();

    GLint codeLenght[1];
    codeLenght[0] = _shaderCode.length();

    glShaderSource(newShader, 1, shaderCode, codeLenght);
    glCompileShader(newShader);

    glGetShaderiv(newShader, GL_COMPILE_STATUS, &result);

    if ( !result ) {
        glGetShaderInfoLog(shaderID, sizeof(eLog), nullptr, eLog);
        std::cerr << "Error compiling " << _shaderType << " shader: " <<  eLog << std::endl;
        return ;
    }

    glAttachShader(_program, newShader);
}