#include <iostream>

#include "Model.h"
#include "Mesh.h"
#include "Texture.h"

Model::Model() = default;

Model::~Model() { ClearModel(); }

void Model::LoadModel(const std::string &_fileName) {
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(_fileName, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals
        | aiProcess_JoinIdenticalVertices );

    if( !scene ) {
        std::cerr << "Model \"" << _fileName << "\" failed to load: " << importer.GetErrorString() << '\n';
    }

    LoadNode(scene->mRootNode, scene);
    LoadMaterials(scene);
}

void Model::RenderModel() {
    for ( size_t i = 0; i < meshList.size(); i++ ) {
        if ( meshToTex[i] < textureList.size() && textureList[meshToTex[i]] ) {
            textureList[meshToTex[i]]->UseTexture();
        }

        meshList[i]->RenderMesh();
    }
}

void Model::ClearModel() {
    for (auto & mesh : meshList) {
        if (mesh) {
            delete mesh;
            mesh = nullptr;
        }
    }

    for (auto & texture : textureList) {
        if (texture) {
            delete texture;
            texture = nullptr;
        }
    }
}

void Model::LoadNode(aiNode *_node, const aiScene *_scene) {
    for( size_t i = 0; i < _node->mNumMeshes; i++ ) {
        LoadMesh(_scene->mMeshes[_node->mMeshes[i]], _scene);
    }

    for( size_t i = 0; i < _node->mNumChildren; i++ ) {
        LoadNode(_node->mChildren[i], _scene);
    }
}

void Model::LoadMesh(aiMesh *_mesh, const aiScene *_scene) {
    std::vector<Shape> vertices;
    std::vector<unsigned int> indices;

    for ( size_t i = 0; i < _mesh->mNumVertices; i++ ) {
        vertices.insert(vertices.end(), {
                _mesh->mVertices[i].x, _mesh->mVertices[i].y, _mesh->mVertices[i].z,
                ( _mesh->mTextureCoords[0] ? _mesh->mTextureCoords[0][i].x : 0.0f ),
                ( _mesh->mTextureCoords[0] ? _mesh->mTextureCoords[0][i].y : 0.0f ),
                -_mesh->mNormals[i].x, -_mesh->mNormals[i].y, -_mesh->mNormals[i].z
        } );
    }

    for ( size_t i = 0; i < _mesh->mNumFaces; i++ ) {
        aiFace face = _mesh->mFaces[i];

        for ( size_t j = 0; j < face.mNumIndices; j++ ) {
            indices.push_back(face.mIndices[j]);
        }
    }

    Mesh* mesh = new Mesh();
    mesh->CreateMesh(vertices, indices);
    meshList.push_back(mesh);
    meshToTex.push_back(_mesh->mMaterialIndex);
}

void Model::LoadMaterials(const aiScene *_scene) {
    textureList.resize(_scene->mNumMaterials);

    for ( size_t i = 0; i < _scene->mNumMaterials; i++ ) {
        textureList[i] = nullptr;

        if ( _scene->mMaterials[i]->GetTextureCount(aiTextureType_DIFFUSE) ) {
            aiString path;

            if ( _scene->mMaterials[i]->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS ) {
                int idx = std::string(path.data).rfind('\\');
                std::string filename = std::string(path.data).substr(idx + 1);
                std::string texPath = std::string("Textures/") + filename;

                textureList[i] = new Texture(texPath);

                if ( !textureList[i]->LoadTexture() ) {
                    std::cerr << "Failed to load texture at: " << texPath << '\n';
                    delete textureList[i];
                    textureList[i] = nullptr;
                }
            }
        }

        if ( !textureList[i] ) {
            textureList[i] = new Texture("Textures/plain.png");
            textureList[i]->LoadTextureA();
        }
    }
}