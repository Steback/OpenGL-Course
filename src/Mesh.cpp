#include "Mesh.h"

Mesh::Mesh() = default;

Mesh::~Mesh() { ClearMesh(); };

void Mesh::CreateMesh(GLfloat *_vertices, unsigned int *_indices, unsigned int _numOfVertices, unsigned int _numOfIndices) {
    indexCount = _numOfIndices;

    // glGenVertexArrays returns n vertex array object names in arrays. There is no guarantee that the names form a contiguous set of integers; however,
    // it is guaranteed that none of the returned names was in use immediately before the call to glGenVertexArrays.
    glGenVertexArrays(1, &VAO);

    // glBindVertexArray binds the vertex array object with name array. array is the name of a vertex array object previously returned from a call to glGenVertexArrays,
    // or zero to break the existing vertex array object binding.
    glBindVertexArray(VAO);

    // glGenBuffers returns n buffer object names in buffers. There is no guarantee that the names form a contiguous set of integers; however,
    // it is guaranteed that none of the returned names was in use immediately before the call to glGenBuffers.
    glGenBuffers(1, &IBO);

    // glBindBuffer binds a buffer object to the specified buffer binding point. Calling glBindBuffer with target set to one of
    // the accepted symbolic constants and buffer set to the name of a buffer object binds that buffer object name to the target.
    // GL_ELEMENT_ARRAY_BUFFER - Vertex array indices
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

    // glBufferData and glNamedBufferData create a new data store for a buffer object. In case of glBufferData, the buffer object currently bound to target is used.
    // For glNamedBufferData, a buffer object associated with ID specified by the caller in buffer will be used instead.
    // GL_STATIC_DRAW - The data store contents will be modified once and used many times as the source for GL drawing commands.
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(_indices[0]) * _numOfIndices, _indices, GL_STATIC_DRAW);

    glGenBuffers(1, &VBO);

    // GL_ARRAY_BUFFER - Vertex attributes
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(_vertices[0]) * _numOfVertices, _vertices, GL_STATIC_DRAW);

    // glVertexAttribPointer — define an array of generic vertex attribute data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(_vertices[0]) * 5, nullptr);

    // glEnableVertexAttribArray — Enable or disable a generic vertex attribute array
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(_vertices[0]) * 5, (void*)(sizeof(_vertices[0]) * 3));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glBindVertexArray(0);
}

void Mesh::RenderMesh() const {
    glBindVertexArray(VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

    // glDrawElements specifies multiple geometric primitives with very few subroutine calls. Instead of calling a GL function to pass each individual vertex,
    // normal, texture coordinate, edge flag, or color, you can prespecify separate arrays of vertices, normals, and so on, and use them to construct a sequence
    // of primitives with a single call to glDrawElements.
    // GL_TRIANGLES - Treats each triplet of vertices as an independent triangle. Vertices 3n - 2 , 3n - 1 , and 3n define triangle n. N / 3 triangles are drawn.
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, nullptr);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Mesh::ClearMesh() {
    if ( IBO != 0 ) {
        glDeleteBuffers(1, &IBO);
        IBO = 0;
    }

    if ( VBO != 0 ) {
        glDeleteBuffers(1, &VBO);
        VBO = 0;
    }

    if ( VAO != 0 ) {
        glDeleteBuffers(1, &VAO);
        VAO = 0;
    }

    indexCount = 0;
}