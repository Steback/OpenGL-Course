#ifndef MESH_H
#define MESH_H

#include <vector>

#include <GL/glew.h>

#include "glm/glm.hpp"

struct Shape {
    glm::vec3 position;
    glm::vec2 texCoord;
    glm::vec3 normal;
    Shape() : position(glm::vec3(0.0f, 0.0f, 0.0f)), texCoord(0.0f, 0.0f), normal(0.0f, 0.0f, 0.0f) {  }
    Shape(GLfloat _x, GLfloat _y, GLfloat _z, GLfloat _u, GLfloat _v, GLfloat _nx, GLfloat _ny, GLfloat _nz)
            : position(glm::vec3(_x, _y, _z)), texCoord(_u, _v), normal(_nx, _ny, _nz) {  }
};

class Mesh {
    public:
        Mesh();
        ~Mesh();
        void CreateMesh(const std::vector<Shape>& _vertices, const std::vector<GLuint> & _indices);
        void RenderMesh() const;
        void ClearMesh();

    private:
        GLuint VAO{}, VBO{}, IBO{};
        GLsizei indexCount{};
};

#endif
