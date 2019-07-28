#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


#define EXECUTION_STDIN     "0"
#define EXECUTION_FILE      "1"


int main(int argc, char *argv[]){

    char *args[] = {"/home/dindibo4/Desktop/Krypton/kernal/kernal", "0",  (char*)0 };
    char *env_args[] = { (char*)0 };

    // stdin
    if(argc == 1){
        execve("/home/dindibo4/Desktop/Krypton/kernal/kernal", args, env_args);
    }
    // from file
    else if (argc == 2)
    {   
        

        args[1] = "1";
        execve("/home/dindibo4/Desktop/Krypton/kernal/kernal", args, env_args);
    }
    
}