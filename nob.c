#define NOB_IMPLEMENTATION
#define NOB_STRIP_PREFIX
#include "nob.h"

#define CFLAGS "-Wall", "-Wextra", "-g"
#define LINKS "-lSDL2"

bool compile_project(Nob_Cmd *cmd, char *file){

    nob_log(INFO, "Compiling whole project\n");
    nob_cmd_append(cmd, "clang++", CFLAGS, file, "-o", "uma", LINKS);
    return cmd_run_sync_and_reset(cmd);
}

int main(int argc, char **argv){
    Nob_Cmd cmd = {0};

    if(argc < 2){
        if(!compile_project(&cmd, "uma.cpp")){
            nob_log(ERROR, "Could not compile project");
            return 1;
        }
    }
    nob_log(INFO, "compiled the project successfully\n");
    return 0;
}