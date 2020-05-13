#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>

#include <GL/glew.h>

#include <stb_image.h>

class Texture {
    public:
        explicit Texture(std::string  _filePath);
        ~Texture();
        bool LoadTexture();
        bool LoadTextureA();
        void UseTexture() const;
        void ClearTexture();

    private:
        GLuint textureID;
        int width, height, bitDepth;
        std::string filePath;
};

#endif