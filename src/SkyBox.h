#ifndef SKY_BOX_H
#define SKY_BOX_H

#include <vector>
#include <string>
#include <memory>

#include "GL/glew.h"
#include "glm/glm.hpp"

class Shader;
class Mesh;

class SkyBox {
    public:
        explicit SkyBox(const std::vector<std::string>& _faceLocations);
        ~SkyBox();
        void DrawSkyBox(glm::mat4 _viewMatrix, glm::mat4 _projectionMatrix);

    private:
        std::unique_ptr<Mesh> skyMesh;
        std::unique_ptr<Shader> skyShader;
        GLuint textureID;
        GLuint uniformProjection, uniformView;
};

#endif