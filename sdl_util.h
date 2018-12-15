#ifndef SPLAY_SDL_UTIL_H
#define SPLAY_SDL_UTIL_H

#include <SDL2/SDL.h>
#include <tuple>
#include <iostream>

#define GL3_PROTOTYPES 1
#include <GL/glew.h>

class SDLManager {
public:
    SDLManager(int win_width, int win_height);
    ~SDLManager();

    SDLManager(SDLManager& other) = delete;
    SDLManager(const SDLManager& other) = delete;

    void init(int win_width, int win_height);
    
    void check_error(std::ostream& os);

    void swap_window();

private:
    void configure_opengl();

    SDL_Window *main_window;
    SDL_GLContext main_ctx;
};

#endif // SPLAY_SDL_UTIL_H