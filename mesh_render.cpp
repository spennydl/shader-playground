#include <GL/glew.h>
#include <iostream>
#include "mesh.h"

void MeshRenderer::render_mesh(const std::unique_ptr<Mesh>& mesh, const Shader& shader, const Texture2D& tex, const glm::mat4& model_matrix) const
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex.tex_id);

    glBindVertexArray(mesh->vao);

    shader.set_i1("tex", 0);
    shader.set_m4("model", model_matrix);
    
    shader.set_v3("model_material.ambient", tex.material.ambient);
    shader.set_v3("model_material.diffuse", tex.material.diffuse);
    shader.set_v3("model_material.specular", tex.material.specular);
    shader.set_uf("model_material.shininess", tex.material.shininess);
    shader.use();

    glDrawElements(GL_TRIANGLES, mesh->indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void MeshRenderer::buffer_mesh(const std::unique_ptr<Mesh>& mesh) const
{
    if (mesh->vao == (unsigned int) -1) {
        glGenVertexArrays(1, &mesh->vao);

        glGenBuffers(1, &mesh->vbo);
        glGenBuffers(1, &mesh->ebo);

        glBindVertexArray(mesh->vao);

        glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo);
        glBufferData(GL_ARRAY_BUFFER, mesh->vertices.size() * sizeof(MeshVertex), mesh->vertices.data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->indices.size() * sizeof(unsigned int), mesh->indices.data(), GL_STATIC_DRAW);

        // x y z xn yn zn u v
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(0);
        
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);
    } else {
        std::cout << "not buffering?" << std::endl;
    }
}