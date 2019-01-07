#ifndef SPLAY_CANVAS_H
#define SPLAY_CANVAS_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>
#include "shader.h"

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 tex_coord;
};

class Canvas
{
  public:
    Canvas() : model_mat(1)
    {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(0);
        
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);
    }

    void run_shader(Shader shader)
    {
        glBindVertexArray(VAO);

        shader.set_m4("model", model_mat);
        shader.use();

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

  private:
    unsigned int VAO, VBO, EBO;

    Vertex vertices[12] = {
        { .position = { 1.2f,  1.0f,  0.0f}, .normal = { 0.0f, 0.0f, 0.0f }, .tex_coord = { 1.0f, 1.0f }},   // top right
        { .position = { 1.2f, -1.0f,  0.0f}, .normal = { 0.0f, 0.0f, 0.0f }, .tex_coord = { 1.0f, 0.0f }},   // bottom right
        { .position = {-1.2f, -1.0f,  0.0f}, .normal = { 0.0f, 0.0f, 0.0f }, .tex_coord = { 0.0f, 0.0f }},   // bottom left
        { .position = {-1.2f,  1.0f,  0.0f}, .normal = { 0.0f, 0.0f, 0.0f }, .tex_coord = { 0.0f, 1.0f }},   // top left 
    };

    unsigned int indices[6] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

    glm::mat4 model_mat;
};

#endif // SPLAY_CANVAS_H