#ifndef SPLAY_MESH_H
#define SPLAY_MESH_H

#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <map>
#include <vector>
#include <memory>

#include "shader.h"
#include "texture.h"

struct MeshVertex
{
    /*
    MeshVertex(const glm::vec3& pos, const glm::vec3& norm, const glm::vec2& t)
        : position(pos), normal(norm), uv(t)
        {}
*/
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 uv;
};

struct MeshLocation
{
    unsigned long vertex_idx;
    unsigned long indices_idx;
};

struct Mesh
{
    Mesh() 
        : vertices(), indices(), meshes(), vao((unsigned int) -1), vbo((unsigned int) -1), ebo((unsigned int) -1)
    {}

    std::vector<MeshVertex> vertices;
    std::vector<unsigned int> indices;
    std::map<std::string, MeshLocation> meshes;
    unsigned int vao;
    unsigned int vbo;
    unsigned int ebo;
};

class MeshLoader
{
  public:
    std::unique_ptr<Mesh> load_mesh(const std::string& filename);

    MeshLoader() 
        : positions(), normals(), uvs(), vertex_idx(0)
    {}


  private:

    void read_vertex  (std::istringstream& ss);
    void read_normal  (std::istringstream& ss);
    void read_uv      (std::istringstream& ss);
    void read_face    (std::istringstream& ss, const std::unique_ptr<Mesh>& mesh);
    void read_new_mesh(std::istringstream& ss, const std::unique_ptr<Mesh>& mesh);
    void clear();

    std::vector<glm::vec3> positions;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> uvs;
    
    unsigned int vertex_idx;

};

class MeshRenderer
{
  public:
    void buffer_mesh(const std::unique_ptr<Mesh>& mesh) const;
    void render_mesh(const std::unique_ptr<Mesh>& mesh, const Shader& shader, const Texture2D& tex, const glm::mat4& model_matrix) const;
  private:
};

#endif // SPLAY_MESH_H