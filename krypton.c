#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#define BUFFER_SIZE 400

#define     true    1
#define     false   0

char *getDirectory(){
    char *buffer = (char*)malloc(BUFFER_SIZE);
    if (getcwd(buffer, BUFFER_SIZE) != NULL) {
        return buffer;
    }
    else {
        perror("getcwd() error");
        return (char*)0;
        exit(1);
    }
}

char *getKernalPath(){
    char *folderPath = getDirectory();
    char path[] = "/kernal/kernal";
    int path_len = strlen(path);
    if(path_len + strlen(folderPath) + 1 > 200){
        perror("path is too big");
        return NULL;
    }
    else{
        strcat(folderPath, path);
        return folderPath;
    }
}

char isFileExist(char *filename){
    if(access(filename, F_OK) != -1){
        return true;
    }
    else{
        return false;
    }
}

int main(int argc, char *argv[]){
    // first part of file command
    char *sys_cmd1 = "cat ";
    // second part of file command
    char *sys_cmd2 = " | ./kernel/kernel 1";

    int sys_cmd1_len = strlen(sys_cmd1);
    int sys_cmd2_len = strlen(sys_cmd2);

    char *currentDir = getDirectory();

    char *args[] = {"./kernel/kernel", "0",  (char*)0 };
    char *env_args[] = { (char*)0 };


    // stdin
    if(argc == 1){
        execve("./kernel/kernel", args, env_args);
    }
    // from file
    else if (argc == 2)
    {   
        int arg_len = strlen(argv[1]);
        int buffer_length = sys_cmd1_len + sys_cmd2_len + arg_len + 1;

        char buffer[buffer_length];

       
        if(isFileExist(argv[1]) != true){
            printf("No such file");
            return EXIT_FAILURE;
        }

        strcpy(buffer, sys_cmd1);
        strcat(buffer, argv[1]);
        strcat(buffer, sys_cmd2);

        system(buffer);
    }

    return EXIT_SUCCESS;
}