#ifndef SPLAY_LIGHT_H
#define SPLAY_LIGHT_H

#include "texture.h"
#include <glm/vec3.hpp>

struct LightAttenuation {
    float constant;
    float linear;
    float quadratic;
};

struct Light {
    glm::vec3 position;

    Material material;
    LightAttenuation attenuation;
};

#endif // SPLAY_LIGHT_H