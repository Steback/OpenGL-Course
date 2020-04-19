#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>

#include <GL/glew.h>

#include "../lib/stb/stb_image.h"

class Texture {
    public:
        explicit Texture(std::string  _filePath);
        ~Texture();
        void LoadTexture();
        void UserTexture() const;
        void ClearTexture();

    private:
        GLuint textureID;
        int width, height, bitDepth;
        std::string filePath;
};

#endif