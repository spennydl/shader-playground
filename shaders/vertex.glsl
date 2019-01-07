#version 330 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 norm;
layout (location = 2) in vec2 tex;

out vec2 tex_coord;
out vec3 frag_coord;
out vec3 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main()
{
    gl_Position = proj * view * model * vec4(pos, 1.0);
    tex_coord = tex;
    frag_coord = vec3(model * vec4(pos, 1.0));
    normal = mat3(transpose(inverse(model))) * norm;
}