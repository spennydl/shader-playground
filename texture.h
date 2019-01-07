#ifndef SPLAY_TEXTURE_H
#define SPLAY_TEXTURE_H

#include <string>

#include "stb_image.h"
#include <glm/vec3.hpp>

struct Material {
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shininess;
};

struct Texture2D {
    unsigned int tex_id;
    Material material;
};

class TextureLoader {
  public:
    Texture2D load_texture(const std::string& filename);
};

#endif // SPLAY_TEXTURE_H