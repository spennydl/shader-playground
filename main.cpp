#include <iostream>
#include <fstream>
#include <streambuf>

#include <SDL2/SDL.h>

#include "sdl_util.h"
#include "canvas.h"
#include "shader.h"


void run(SDLManager &sdl)
{
    bool run = true;

    Canvas canvas;
    Shader shader("vertex.glsl", "fragment.glsl");
    int mouse_x = 0, mouse_y = 0;

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
                mouse_x = event.motion.x;
                mouse_y = event.motion.y;
            }

        }

        glClearColor(0.0, 0.0, 0.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);

        unsigned int t = SDL_GetTicks();
        double ts = t / 1000.0; // convert to seconds

        shader.set_uf("u_time", ts);
        shader.set_u2f("u_mouse", mouse_x, mouse_y);
        shader.set_u2f("u_resolution", 1280, 720);

        canvas.run_shader(shader);

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