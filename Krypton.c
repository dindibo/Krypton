#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#define BUFFER_SIZE 100
#define strEqu(BUFFER, STR) strncmp(BUFFER, STR, strlen(STR)) == 0


#define CMD_PRINT "print"


#define INTERPRET_ERROR         0
#define INTERPRET_ARITHMETIC    1
#define INTERPRET_STRING        2
#define INTERPRET_NUMBER        3



#define INTERPRETATION_DIGIT            0
#define INTERPRETATION_MATH_OPERATOR    1

// TODO: replace strlen(CMD) with const number

char *findStringLiteral(char *str){
    char started = 0;
    char *ptr = NULL;
    char *start = NULL;
    for(ptr = str; *ptr != 0; ptr++){
        if(*ptr == '\''){
            if(started == 1){
                *ptr = 10;
                *(ptr+1) = 0;
            }
            else{
                start = ptr + 1;
                started = 1;
            }
        }
    }

    return start;
}

void interpret(char *buffer){

    char option = INTERPRET_ERROR;
    char interpret_ops[BUFFER_SIZE];

    // TODO: Implement

}

void parseCommand(char *buffer){
    if(strEqu(buffer, CMD_PRINT)){
        fgets(buffer, BUFFER_SIZE, stdin);
        char *txt = findStringLiteral(buffer);
        printf("%s", txt);
    }
    else{
        interpret(buffer);
    }
}

int main(){

    char buffer[BUFFER_SIZE];
    scanf("%s", buffer);

    while (strcmp(buffer, "exit") != 0)
    {
        parseCommand(buffer);
        scanf("%s", buffer);
    }
    
    return 0;
}
