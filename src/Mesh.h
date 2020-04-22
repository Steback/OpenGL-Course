#ifndef MESH_H
#define MESH_H

#include <GL/glew.h>

class Mesh {
    public:
        Mesh();
        ~Mesh();
        void CreateMesh(GLfloat* _vertices, unsigned int* _indices, unsigned int _numOfVertices, unsigned int _numOfIndices);
        void RenderMesh() const;
        void ClearMesh();

    private:
        GLuint VAO{}, VBO{}, IBO{};
        GLsizei indexCount{};
};

#endif
