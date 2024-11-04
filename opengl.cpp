#include "opengl.hpp"

bool compile_shader(unsigned int *shader, int shader_type, const char* source){

    *shader = glCreateShader(shader_type);

    if(*shader == 0){
        printf("ERROR: wrong shader type inputted\n");
        return false;
    }
    glShaderSource(*shader, 1, &source, NULL);
    glCompileShader(*shader);

    int success;
    char info_log[512];
    glGetShaderiv(*shader, GL_COMPILE_STATUS, &success);

    if(!success){
        glGetShaderInfoLog(*shader, 512, NULL, info_log);
        printf("ERROR in compiling shader %s\n", info_log);
        return false;
    }
    return true;
}

bool configure_opengl(SDL_Window* window, SDL_GLContext gl_context){

    gl_context = SDL_GL_CreateContext(window);
    if(!gl_context){
        printf("ERROR, could not create context, SDL_ERROR: %s", SDL_GetError());
        return false;
    }

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
    
    SDL_GL_MakeCurrent(window, gl_context);

    GLenum err = glewInit();
    
    if(err != GLEW_OK){
        printf("GLEW initialization failed %s\n", glewGetErrorString(err));
        SDL_GL_DeleteContext(gl_context);
        return false;
    }

    return true;
    

}
