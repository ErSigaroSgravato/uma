#include<GL/glew.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<SDL2/SDL.h>
#include<iostream>
#include<fstream>
#include "camera.hpp"

#define HEIGHT 480.0f
#define WIDTH 640.0f




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

std::string read_file_to_string(const char *filename){
    std::ifstream file_reader(filename, std::ios::binary);
    if(!file_reader.is_open()){
        printf("ERROR: could not open file %s\n", filename);
        return "";
    }

    std::string content((std::istreambuf_iterator<char>(file_reader)),
                        std::istreambuf_iterator<char>());
    file_reader.close();

    return content;
}


int main(){

    SDL_Window* window = NULL;
    Camera camera;
    glm::mat4 projection = glm::perspective(glm::radians(45.0f),WIDTH/HEIGHT, 0.1f, 100.0f);

    init_camera(&camera);
    
    if(SDL_Init(SDL_INIT_VIDEO) < 0){
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }



    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    window = SDL_CreateWindow("Hello, World!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

    if(window == NULL){
        printf("SDL could not create window! SDL_Error: %s\n", SDL_GetError());    
        SDL_Quit();
        return 1;
    }

    SDL_GLContext gl_context = SDL_GL_CreateContext(window);
    if(!gl_context){
        printf("SDL Could not create opengl context: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
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
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
    
    if(SDL_GL_SetSwapInterval(1) < 0){
        printf("Warning: Could not set VSync: %s", SDL_GetError());
    }

    std::string vertex_shader_source = read_file_to_string("vertex.shader");
    std::string fragment_shader_source = read_file_to_string("fragment.shader");
    unsigned int vertex_shader = 0;

    if(!compile_shader(&vertex_shader, GL_VERTEX_SHADER, vertex_shader_source.c_str())){
        SDL_GL_DeleteContext(gl_context);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
    unsigned int fragment_shader = 0;
    if(!compile_shader(&fragment_shader, GL_FRAGMENT_SHADER, fragment_shader_source.c_str())){
        SDL_GL_DeleteContext(gl_context);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f, 0.5f, 0.0f
    };

    unsigned int program;
    program = glCreateProgram();

    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);

    int success;
    char log[512];
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if(!success){
        glGetProgramInfoLog(program, 512, NULL, log);
        printf("ERROR could not link program: %s", log);
        glDeleteShader(vertex_shader);
        glDeleteShader(fragment_shader);
        SDL_GL_DeleteContext(gl_context);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    glUseProgram(program);
    
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    unsigned int VBO;
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);


    
    SDL_Event e;
    bool quit = false;


    unsigned int model_loc = glGetUniformLocation(program, "model");
    unsigned int view_loc = glGetUniformLocation(program, "view");
    unsigned int projection_loc = glGetUniformLocation(program, "projection");
    
    while(!quit){
        while(SDL_PollEvent(&e)){
            if(e.type == SDL_QUIT){
                quit = true;
            }
            if(e.type == SDL_KEYDOWN){
                switch(e.key.keysym.sym){
                case SDLK_w: camera.pos += CAMERA_SPEED * camera.dir; break;
                case SDLK_s: camera.pos -= CAMERA_SPEED * camera.dir; break;
                case SDLK_d: camera.pos += glm::normalize(glm::cross(camera.dir, camera.up)) * CAMERA_ROTATION; break;
                case SDLK_a: camera.pos -= glm::normalize(glm::cross(camera.dir, camera.up)) * CAMERA_ROTATION; break;
                }
            }
        }

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, -3.0f));
        glm::mat4 view = calculate_view_matrix(&camera);

        glUseProgram(program);

        glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(view_loc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projection_loc, 1, GL_FALSE, glm::value_ptr(projection));
        
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        SDL_GL_SwapWindow(window);
    }

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
