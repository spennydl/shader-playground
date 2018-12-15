#include "sdl_util.h"

SDLManager::SDLManager(int win_width, int win_height)
    : main_window(nullptr), main_ctx()
{
    init(win_width, win_height);
    if (!main_window) {
        check_error(std::cout);
        throw std::runtime_error("Could not initialize SDL and OPENGL");
    }
}

SDLManager::~SDLManager()
{
    SDL_GL_DeleteContext(main_ctx);
    SDL_DestroyWindow(main_window);
    SDL_Quit();
}

void SDLManager::configure_opengl()
{
    // Set our OpenGL version.
    // SDL_GL_CONTEXT_CORE gives us only the newer version, deprecated functions are disabled
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    // 3.2 is part of the modern versions of OpenGL, but most video cards whould be able to run it
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

    // Turn on double buffering with a 24bit Z buffer.
    // You may need to change this to 16 or 32 for your system
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    // synchronizes to monitor refresh rate
    SDL_GL_SetSwapInterval(1);
}

void SDLManager::init(int win_width, int win_height)
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        return;
    }

    main_window = SDL_CreateWindow("Shader Playground",
                                    SDL_WINDOWPOS_CENTERED,
                                    SDL_WINDOWPOS_CENTERED,
                                    win_width,
                                    win_height,
                                    SDL_WINDOW_OPENGL);

    if (!main_window) {
        return;
    }

    main_ctx = SDL_GL_CreateContext(main_window);

    configure_opengl();

    GLenum err = glewInit();
    if (GLEW_OK != err) {
        std::cout << "Couldn't init glew" << std::endl;
    }
}

void SDLManager::check_error(std::ostream& os)
{
    std::string err = SDL_GetError();

    if (err != "") {
        os << "SDL ERROR: " << err << std::endl;
        SDL_ClearError();
    }
}

void SDLManager::swap_window()
{
    SDL_GL_SwapWindow(main_window);
}