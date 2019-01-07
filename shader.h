#ifndef SPLAY_SHADER_H
#define SPLAY_SHADER_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <fstream>
#include <streambuf>
#include <iostream>

class Shader
{
  public:
    Shader(const std::string &vfile, const std::string &ffile)
        : shader_id(0), v_filename(vfile), f_filename(ffile)
    {
        init();
    }

    void use() const
    {
        glUseProgram(shader_id);
    }

    void set_uf(const std::string &name, double n) const
    {
        int u_location = glGetUniformLocation(shader_id, name.c_str());
        use();
        glUniform1f(u_location, n);
    }

    void set_u2f(const std::string &name, double n1, double n2) const
    {
        int u_location = glGetUniformLocation(shader_id, name.c_str());
        use();
        glUniform2f(u_location, n1, n2);
    }

    void set_m4(const std::string &name, glm::mat4 m) const
    {
        int u_location = glGetUniformLocation(shader_id, name.c_str());
        use();
        glUniformMatrix4fv(u_location, 1, GL_FALSE, glm::value_ptr(m));
    }

    void set_i1(const std::string &name, int val) const
    {
        int u_location = glGetUniformLocation(shader_id, name.c_str());
        use();
        glUniform1i(u_location, val);
    }

    void set_v3(const std::string& name, glm::vec3 v) const
    {
        int u_location = glGetUniformLocation(shader_id, name.c_str());
        use();
        glUniform3f(u_location, v.x, v.y, v.z);
    }

    void reload()
    {
        if (shader_id)
        {
            glDeleteProgram(shader_id);
        }
        init();
    }

  private:
    std::string load_shader_src(const std::string &filename)
    {
        std::ifstream file("./shaders/" + filename);
        if (file)
        {
            return std::string(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
        }
        return "";
    }

    void init()
    {
        std::string vert_src = load_shader_src(v_filename);
        std::string frag_src = load_shader_src(f_filename);

        unsigned int vert_shader = glCreateShader(GL_VERTEX_SHADER);
        const char *vsrc = vert_src.c_str();
        glShaderSource(vert_shader, 1, &vsrc, nullptr);
        glCompileShader(vert_shader);
        int success;
        char infoLog[512];
        glGetShaderiv(vert_shader, GL_COMPILE_STATUS, &success);

        if (!success)
        {

            glGetShaderInfoLog(vert_shader, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
                      << infoLog << std::endl;
        }

        unsigned int frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
        const char *fsrc = frag_src.c_str();
        glShaderSource(frag_shader, 1, &fsrc, nullptr);
        glCompileShader(frag_shader);
        glGetShaderiv(frag_shader, GL_COMPILE_STATUS, &success);

        if (!success)
        {

            glGetShaderInfoLog(frag_shader, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
                      << infoLog << std::endl;
        }

        shader_id = glCreateProgram();
        glAttachShader(shader_id, vert_shader);
        glAttachShader(shader_id, frag_shader);
        glLinkProgram(shader_id);
        glGetProgramiv(shader_id, GL_LINK_STATUS, &success);
        if (!success) 
        {
            glGetProgramInfoLog(shader_id, 512, nullptr, infoLog);
            std::cout << "ERROR::SHADER::LINK\n"
                      << infoLog << std::endl;
        }

        glDeleteShader(vert_shader);
        glDeleteShader(frag_shader);
    }

    unsigned int shader_id;
    std::string v_filename;
    std::string f_filename;
};

#endif // SPLAY_SHADER_H