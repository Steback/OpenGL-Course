#include <fstream>

#include "Shader.h"

Shader::Shader() = default;

Shader::~Shader() { ClearShader(); }

void Shader::CreateFormFiles(const std::string &_vertexFilePath, const std::string &_fragmentFilePath) {
    std::string vertexString = ReadFile(_vertexFilePath);
    std::string fragmentString = ReadFile(_fragmentFilePath);

    CompileShader(vertexString, fragmentString);
}

void Shader::CreateFormFiles(const std::string &_vertexFilePath,  const std::string &_geometryFilePath, const std::string &_fragmentFilePath) {
    std::string vertexString = ReadFile(_vertexFilePath);
    std::string geometryString = ReadFile(_geometryFilePath);
    std::string fragmentString = ReadFile(_fragmentFilePath);

    CompileShader(vertexString, geometryString, fragmentString);
}

std::string Shader::ReadFile(const std::string &_filePath) {
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

GLuint Shader::GetUniformLocation(const std::string &_name) const {
    return glGetUniformLocation(shaderID, _name.c_str());
}

GLuint Shader::GetProjectionLocation() const { return uniformProjection; }

GLuint Shader::GetModelLocation() const { return uniformModel; }

GLuint Shader::GetViewLocation() const { return uniformView; }

void Shader::UseShader() const {
    // glGetUniformLocation returns an integer that represents the location of a specific uniform variable within a program object.
    glUseProgram(shaderID);
}

void Shader::ClearShader() {
    if ( shaderID != 0 ) {
        // glDeleteProgram frees the memory and invalidates the name associated with the program object specified by program.
        glDeleteProgram(shaderID);
    }

    uniformModel = 0;
    uniformProjection = 0;
}

void Shader::Validate() const {
    GLint result{};
    GLchar eLog[1024]{};

    // glValidateProgram checks to see whether the executables contained in program can execute given the current OpenGL state.
    glValidateProgram(shaderID);

    // GL_VALIDATE_STATUS - params returns GL_TRUE or if the last validation operation on program was successful, and GL_FALSE otherwise.
    glGetProgramiv(shaderID, GL_VALIDATE_STATUS, &result);

    if ( !result ) {
        glGetProgramInfoLog(shaderID, sizeof(eLog), nullptr, eLog);
        std::cerr << "Error validating program: " << eLog << std::endl;
        return ;
    }
}

void Shader::CompileShader(std::string &_vertexCode, std::string &_fragmentCode) {
    // glCreateProgram creates an empty program object and returns a non-zero value by which it can be referenced.
    // A program object is an object to which shader objects can be attached. This provides a mechanism to specify the shader objects that will be linked to create a program.
    shaderID = glCreateProgram();

    if ( !shaderID ) {
        std::cerr << "Error creating shader program" << std::endl;
        return ;
    }

    // GL_VERTEX_SHADER - A shader of type GL_VERTEX_SHADER is a shader that is intended to run on the programmable vertex processor
    AddShader(shaderID, _vertexCode, GL_VERTEX_SHADER);

    // GL_FRAGMENT_SHADER - A shader of type GL_FRAGMENT_SHADER is a shader that is intended to run on the programmable fragment processor.
    AddShader(shaderID, _fragmentCode, GL_FRAGMENT_SHADER);

    // glLinkProgram links the program object specified by program. If any shader objects of type GL_VERTEX_SHADER are attached to program,
    // they will be used to create an executable that will run on the programmable vertex processor.
    glLinkProgram(shaderID);

    CompileProgram();
}

void Shader::CompileShader(std::string& _vertexCode, std::string& _geometryCode, std::string& _fragmentCode) {
    shaderID = glCreateProgram();

    if ( !shaderID ) {
        std::cerr << "Error creating shader program" << std::endl;
        return ;
    }

    AddShader(shaderID, _vertexCode, GL_VERTEX_SHADER);
    AddShader(shaderID, _geometryCode, GL_GEOMETRY_SHADER);
    AddShader(shaderID, _fragmentCode, GL_FRAGMENT_SHADER);

    glLinkProgram(shaderID);

    CompileProgram();
}

void Shader::AddShader(GLuint _program, std::string& _shaderCode, GLenum _shaderType) {
    // glCreateShader creates an empty shader object and returns a non-zero value by which it can be referenced.
    // A shader object is used to maintain the source code strings that define a shader. shaderType indicates the type of shader to be created.
    GLuint shader = glCreateShader(_shaderType);

    const GLchar* shaderCode[1];
    shaderCode[0] = _shaderCode.c_str();

    GLint codeLenght[1];
    codeLenght[0] = _shaderCode.length();

    // glShaderSource sets the source code in shader to the source code in the array of strings specified by string.
    glShaderSource(shader, 1, shaderCode, codeLenght);

    // glCompileShader compiles the source code strings that have been stored in the shader object specified by shader.
    glCompileShader(shader);

    GLint result{};
    GLchar eLog[1024]{};

    // glGetShaderiv — return a parameter from a shader object
    glGetShaderiv(shader, GL_COMPILE_STATUS, &result);

    if ( !result ) {
        // glGetShaderInfoLog — Returns the information log for a shader object
        glGetShaderInfoLog(shader, sizeof(eLog), nullptr, eLog);
        std::cerr << "Error compiling " << _shaderType << " shader: " <<  eLog << std::endl;
        return ;
    }

    // glAttachShader — Attaches a shader object to a program object
    glAttachShader(_program, shader);
}

void Shader::CompileProgram() {
    GLint result{};
    GLchar eLog[1024]{};

    // glGetProgramiv returns in params the value of a parameter for a specific program object.
    // GL_LINK_STATUS - params returns GL_TRUE if the last link operation on program was successful, and GL_FALSE otherwise.
    glGetProgramiv(shaderID, GL_LINK_STATUS, &result);

    if ( !result ) {
        // glGetProgramInfoLog — return the information log for a program object
        glGetProgramInfoLog(shaderID, sizeof(eLog), nullptr, eLog);
        std::cerr << "Error linking program: " << eLog << std::endl;
        return ;
    }

    // glGetUniformLocation returns an integer that represents the location of a specific uniform variable within a program object.
    uniformModel = glGetUniformLocation(shaderID, "model");
    uniformProjection = glGetUniformLocation(shaderID, "projection");
    uniformView = glGetUniformLocation(shaderID, "view");
}