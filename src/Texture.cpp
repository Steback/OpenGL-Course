#include "Texture.h"

Texture::Texture(std::string _filePath) : filePath(std::move(_filePath)), textureID(0), width(0), height(0), bitDepth(0) {  }

Texture::~Texture() { ClearTexture(); };

void Texture::LoadTexture() {
    unsigned char* textureData = stbi_load(filePath.c_str(), &width, &height, &bitDepth, 0);

    if ( !textureData ) printf("Failed to load texture %s\n", filePath.c_str());

    // glGenTextures returns n texture names in textures.
    glGenTextures(1, &textureID);

    // glBindTexture lets you create or use a named texture.
    glBindTexture(GL_TEXTURE_2D, textureID);

    // glTexParameter, glTextureParameter — set texture parameters
    // GL_TEXTURE_WRAP_S - Sets the wrap parameter for texture coordinate s to either GL_CLAMP_TO_EDGE, GL_MIRRORED_REPEAT, or GL_REPEAT. GL_CLAMP_TO_EDGE
    // causes s coordinates to be clamped to the range 1 2N 1 - 1 2N , where N is the size of the texture in the direction of clamping.
    glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);

    // GL_TEXTURE_WRAP_T - Sets the wrap parameter for texture coordinate t to either GL_CLAMP_TO_EDGE, GL_MIRRORED_REPEAT, or GL_REPEAT.
    glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // GL_TEXTURE_MIN_FILTER - The texture minifying function is used whenever the pixel being textured maps to an area greater than one texture element.
    // GL_LINEAR - Returns the weighted average of the four texture elements that are closest to the center of the pixel being textured.
    glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    // GL_TEXTURE_MAG_FILTER - The texture magnification function is used when the pixel being textured maps to an area less than or equal to one texture element.
    glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // glTexImage2D — specify a two-dimensional texture image
    // GL_RGBA, GL_BGRA - Each element contains all four components. Each component is clamped to the range [0,1].
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureData);

    // glGenerateMipmap and glGenerateTextureMipmap generates mipmaps for the specified texture object.
    glGenerateMipmap(GL_TEXTURE_2D);

    // glBindTexture — bind a named texture to a texturing target
    glBindTexture(GL_TEXTURE, 0);

    stbi_image_free(textureData);
}

void Texture::UserTexture() const {
    // glActiveTexture selects which texture unit subsequent texture state calls will affect.
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE, textureID);
}

// glDeleteTextures deletes n textures named by the elements of the array textures.
void Texture::ClearTexture() { glDeleteTextures(1, &textureID); }