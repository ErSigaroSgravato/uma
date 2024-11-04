#include<GL/glew.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<SDL2/SDL.h>
#include "camera.hpp"
#include "opengl.hpp"
#include "file_reader.hpp"

#define HEIGHT 480.0f
#define WIDTH 640.0f

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

    window = SDL_CreateWindow("UMA", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

    if(window == NULL){
        printf("SDL could not create window! SDL_Error: %s\n", SDL_GetError());    
        SDL_Quit();
        return 1;
    }

    SDL_GLContext gl_context = NULL;
    if(!configure_opengl(window, gl_context)){
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
    
    if(SDL_GL_SetSwapInterval(1) < 0){
        printf("Warning: Could not set VSync: %s", SDL_GetError());
    }

    std::string vertex_shader_source = read_file_to_string("vertex.glsl");
    std::string fragment_shader_source = read_file_to_string("fragment.glsl");
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
        -0.5f, -0.5f, 0.0f, //up-left
        0.5f, -0.5f, 0.0f, //down-left
        0.0f, 0.5f, 0.0f, //down-right
        0.0f, 0.5f, -0.5f, //down-right-deep
        0.5f, -0.5f, -0.5f //down-left-deep
    };

    unsigned int indices[] = {
        0, 1, 2, //front
        0, 1, 4, //left
        0, 3, 4, //back
        0, 2, 3
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

    unsigned int EBO;
    glGenBuffers(1, &EBO);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    
    
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
        glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0); //ERROR IN DRAWING
        SDL_GL_SwapWindow(window);
    }

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
