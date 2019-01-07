#include <iostream>
#include <fstream>
#include <streambuf>

#include <SDL2/SDL.h>

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "sdl_util.h"
#include "canvas.h"
#include "shader.h"
#include "camera.h"
#include "mesh.h"
#include "light.h"

void set_lighting_uniforms(const Shader& shader, const Light& light)
{
    shader.set_v3("light.position", light.position);
    shader.set_v3("light.material.ambient", light.material.ambient);
    shader.set_v3("light.material.diffuse", light.material.diffuse);
    shader.set_v3("light.material.specular", light.material.specular);

    shader.set_uf("light.constant_term", light.attenuation.constant);
    shader.set_uf("light.linear_term", light.attenuation.linear);
    shader.set_uf("light.quad_term", light.attenuation.quadratic);
}

void run(SDLManager &sdl)
{
    bool run = true;

    Canvas canvas;
    Shader shader("vertex.glsl", "fragment.glsl");
    glm::vec3 camera_pos(-80.0f, 60.0f, 65.0f);
    Camera camera(camera_pos, 0.0, -90.0);

    MeshLoader mesh_loader;
    auto plane = mesh_loader.load_mesh("models/plane.obj");
    if (!plane) {
        std::cout << "didn't work" << std::endl;
        return;
    }

    auto sphere = mesh_loader.load_mesh("models/sphere.obj");
    if (!sphere) {
        std::cout << "sphere didn't work" << std::endl;
        return;
    }

    auto cube = mesh_loader.load_mesh("models/cube.obj");
    if (!cube) {
        std::cout << "cube didn't work" << std::endl;
        return;
    }

    auto cyl = mesh_loader.load_mesh("models/cyl.obj");
    if (!cube) {
        std::cout << "cyl didn't work" << std::endl;
        return;
    }

    MeshRenderer renderer;

    renderer.buffer_mesh(plane);
    renderer.buffer_mesh(sphere);
    renderer.buffer_mesh(cube);
    renderer.buffer_mesh(cyl);

    TextureLoader tex_loader;
    auto plane_texture = tex_loader.load_texture("models/plane.png");
    plane_texture.material = {
        { 0.31f, 0.31f, 0.5f },
        { 1.0f, 1.0f, 1.0f },
        { 0.5f, 0.5f, 0.5f },
        32.0f
    };

    auto sphere_texture = tex_loader.load_texture("models/sphere.png");
    sphere_texture.material = {
        { 0.31f, 0.31f, 0.5f },
        { 1.0f, 1.0f, 1.0f },
        { 0.5f, 0.5f, 0.5f },
        32.0f
    };

    auto cube_texture = tex_loader.load_texture("models/cube.png");
    cube_texture.material = {
        { 0.31f, 0.31f, 0.5f },
        { 1.0f, 1.0f, 1.0f },
        { 0.5f, 0.5f, 0.5f },
        32.0f
    };

    auto cyl_texture = tex_loader.load_texture("models/cyl.png");
    cyl_texture.material = {
        { 0.31f, 0.31f, 0.5f },
        { 1.0f, 1.0f, 1.0f },
        { 0.5f, 0.5f, 0.5f },
        32.0f
    };

    glm::vec3 light_pos(-48.0, 96.0f, 16.0f);
    glm::vec3 light_color(1.0, 1.0, 1.0);
    Material light_material{
        { 0.2f, 0.2f, 0.2f },
        { 0.8f, 0.8f, 0.8f },
        { 1.0f, 1.0f, 1.0f },
        0.0f
    };
    LightAttenuation light_attenuation{
        1.0, 0.014, 0.0007
    };
    Light light{
        light_pos,
        light_material,
        light_attenuation
    };

    while (run)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                run = false;
            }
            else if (event.type == SDL_KEYDOWN)
            {
                switch (event.key.keysym.sym)
                {
                case SDLK_ESCAPE:
                {
                    run = false;
                }
                break;
                case SDLK_r:
                {
                    shader.reload();
                }
                default:
                    break;
                }
            }
            else if (event.type == SDL_MOUSEMOTION)
            {
                camera.update(event.motion.yrel, event.motion.xrel);
            }

        }

        glClearColor(0.0, 0.0, 0.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        unsigned int t = SDL_GetTicks();
        double ts = t / 1000.0; // convert to seconds

        shader.set_uf("u_time", ts);

        set_lighting_uniforms(shader, light);
        shader.set_v3("camera_pos", camera_pos);

        // view matrix
        glm::mat4 view = camera.calculate_view_matrix();

        shader.set_m4("view", view);

        // perspective
        glm::mat4 proj;
        proj = glm::perspective(glm::radians(45.0f), 1280.0f / 720.0f, 0.1f, 250.0f);
        shader.set_m4("proj", proj);

        glm::mat4 model_matrix(1); // cheating!

        glm::vec3 sphere_pos = { -32.0f, 0.0f, -32.0f };
        glm::mat4 sphere_matrix = glm::translate(glm::mat4(1.0f), sphere_pos);

        glm::vec3 cube_pos = { 30.0f, 0.0f, -10.0f };
        glm::mat4 cube_matrix = glm::translate(glm::mat4(1.0f), cube_pos);

        glm::vec3 cyl_pos = { 20.0f, 0.0f, 40.0f };
        glm::mat4 cyl_matrix = glm::translate(glm::mat4(1.0f), cyl_pos);

        renderer.render_mesh(cube, shader, cube_texture, cube_matrix);
        renderer.render_mesh(sphere, shader, sphere_texture, sphere_matrix);
        renderer.render_mesh(cyl, shader, cyl_texture, cyl_matrix);
        renderer.render_mesh(plane, shader, plane_texture, model_matrix);

        // projection matrix
        //canvas.run_shader(shader);

        sdl.swap_window();
    }
}

int main()
{
    try
    {
        SDLManager sdl(1280, 720);
        run(sdl);
    }
    catch (std::exception ex)
    {
        std::cout << "failed to do stuff: " << ex.what() << std::endl;
    }
}