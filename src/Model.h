#ifndef OPENGL_EXAMPLES_MODEL_H
#define OPENGL_EXAMPLES_MODEL_H

#include <vector>
#include <string>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Mesh;
class Texture;

class Model {
    public:
        Model();
        ~Model();
        void LoadModel(const std::string& _fileName);
        void RenderModel();
        void ClearModel();

private:
        std::vector<Mesh*> meshList;
        std::vector<Texture*> textureList;
        std::vector<unsigned int> meshToTex;

        void LoadNode(aiNode* _node, const aiScene* _scene);
        void LoadMesh(aiMesh* _mesh, const aiScene* _scene);
        void LoadMaterials(const aiScene* _scene);
};

#endif