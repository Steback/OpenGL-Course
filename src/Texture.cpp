#include "Texture.h"

Texture::Texture(std::string _filePath) : filePath(std::move(_filePath)), textureID(0), width(0), height(0), bitDepth(0) {  }

Texture::~Texture() { ClearTexture(); };

void Texture::LoadTexture() {
    unsigned char* textureData = stbi_load(filePath.c_str(), &width, &height, &bitDepth, 0);

    if ( !textureData ) printf("Failed to load texture %s\n", filePath.c_str());

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureData);
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE, 0);

    stbi_image_free(textureData);
}

void Texture::UserTexture() const {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE, textureID);
}

void Texture::ClearTexture() { glDeleteTextures(1, &textureID); }