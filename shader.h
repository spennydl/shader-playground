#ifndef SPLAY_SHADER_H
#define SPLAY_SHADER_H

#include <GL/glew.h>
#include <string>
#include <fstream>
#include <streambuf>

class Shader
{
  public:
    Shader(const std::string& vfile, const std::string& ffile)
        : shader_id(0), v_filename(vfile), f_filename(ffile)
    {
        init();
    }

    void use()
    {
        glUseProgram(shader_id);
    }

    void set_uf(std::string name, double n)
    {
        int u_location = glGetUniformLocation(shader_id, name.c_str());
        use();
        glUniform1f(u_location, n);
    }

    void set_u2f(std::string name, double n1, double n2)
    {
        int u_location = glGetUniformLocation(shader_id, name.c_str());
        use();
        glUniform2f(u_location, n1, n2);
    }

    void reload()
    {
        if (shader_id) {
            glDeleteProgram(shader_id);
        }
        init();
    }

  private:
    std::string load_shader_src(const std::string& filename)
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

        unsigned int frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
        const char *fsrc = frag_src.c_str();
        glShaderSource(frag_shader, 1, &fsrc, nullptr);
        glCompileShader(frag_shader);

        shader_id = glCreateProgram();
        glAttachShader(shader_id, vert_shader);
        glAttachShader(shader_id, frag_shader);
        glLinkProgram(shader_id);

        glDeleteShader(vert_shader);
        glDeleteShader(frag_shader);
    }

    unsigned int shader_id;
    std::string v_filename;
    std::string f_filename;

};

#endif // SPLAY_SHADER_H