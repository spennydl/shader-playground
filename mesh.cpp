#include <memory>
#include <string>
#include <fstream>
#include <streambuf>
#include <sstream>
#include <iostream>

#include "mesh.h"
#include "texture.h"

template<typename Stream>
std::string read_whole_file(Stream& fs)
{
    std::string contents;

    fs.seekg(0, fs.end);
    contents.reserve(fs.tellg());
    fs.seekg(0, fs.beg);

    contents.assign((std::istreambuf_iterator<char>(fs)), std::istreambuf_iterator<char>());

    return contents;
}

std::unique_ptr<Mesh> MeshLoader::load_mesh(const std::string& filename)
{
    std::ifstream file(filename);
    if (!file) {
        return nullptr;
    }
    auto contents = read_whole_file(file);
    std::istringstream ss(contents);
    std::string line;

    auto mesh = std::make_unique<Mesh>();

    while (std::getline(ss, line)) {
        std::istringstream ls(line);
        std::string type;
        if (ls >> type) {
            if (type == "v") {
                read_vertex(ls);
            } else if (type == "vn") {
                read_normal(ls);
            } else if (type == "vt") {
                read_uv(ls);
            } else if (type == "f") {
                read_face(ls, mesh);
            } else if (type == "o" || type == "g") {
                read_new_mesh(ls, mesh);
            }
        }
    }
    clear();
    return mesh;
}

void MeshLoader::clear()
{
    positions.clear();
    normals.clear();
    uvs.clear();

    vertex_idx = 0;
}

void MeshLoader::read_vertex(std::istringstream& ss)
{
    float x, y, z;
    ss >> x >> y >> z;
    positions.emplace_back(x, y, z);
}

void MeshLoader::read_normal(std::istringstream& ss)
{
    float x, y, z;
    ss >> x >> y >> z;
    normals.emplace_back(x, y, z);
}

void MeshLoader::read_uv(std::istringstream& ss)
{
    float u, v;
    ss >> u >> v;
    uvs.emplace_back(u, v);
}

void MeshLoader::read_face(std::istringstream& ss, const std::unique_ptr<Mesh>& mesh)
{
    // we don't check if the faces are triangulated anywhere.
    // if it's not trianglated i don't want none
    std::string token;
    while (ss >> token) {
        int v_idx, no, t;
        std::istringstream vert(token);
        std::istringstream n;

        std::string s;
        std::getline(vert, s, '/');
        n.str(s);
        n >> v_idx;
        glm::vec3 v = positions[v_idx - 1];

        std::string tr;
        std::getline(vert, tr, '/');
        n.str(tr); n.clear();
        n >> t;
        glm::vec2 tex = uvs[t - 1];

        std::string nr;
        std::getline(vert, nr, '/');
        n.str(nr); n.clear();
        n >> no;
        glm::vec3 normal = normals[no - 1];

        MeshVertex mesh_vertex = {
            .position = v,
            .normal = normal,
            .uv = tex
        };
        mesh->vertices.push_back(mesh_vertex);
        mesh->indices.push_back(vertex_idx++);
    }
}

void MeshLoader::read_new_mesh(std::istringstream& ss, const std::unique_ptr<Mesh>& mesh)
{
    std::string mesh_name;
    ss >> mesh_name;

    mesh->meshes[mesh_name] = {
        .vertex_idx = mesh->vertices.size(),
        .indices_idx = mesh->indices.size()
    };
}
