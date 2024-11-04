#pragma once
#include<GL/glew.h>
#include<SDL2/SDL.h>
#include<iostream>

bool compile_shader(unsigned int *shader, int shader_type, const char* source);
bool configure_opengl(SDL_Window* window, SDL_GLContext gl_context);
