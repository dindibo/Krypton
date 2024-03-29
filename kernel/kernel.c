#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//#define DEBUG
#define false   0
#define true    1

#define BUFFER_SIZE     200
#define MAX_VARIABLES   100


#define strEqu(BUFFER, STR) strncmp(BUFFER, STR, strlen(STR)) == 0
#define explode(STR, CHR) divideStringByIndex(STR, findCharOnString(STR, CHR));
#define isdigit(CHR) (48 <= CHR && CHR <= 57)


#define CMD_EXIT    "exit"
#define CMD_PRINT   "print"
#define CMD_CLEAR   "clear"
#define CMD_GET     "$"


#define RESERVED_TRUE       "true"
#define RESERVED_FALSE      "false"


#define EXECUTION_STDIN     "0"
#define EXECUTION_FILE      "1"


// TODO: replace strlen(CMD) with const number


struct GlobalVariables
{
    char **variableNames;
    int *variableValues;
    int maxVariables;
    int currentVariables;
}typedef GlobalVariables;

GlobalVariables globals;

// The function assumes that there's no name collision
GlobalVariables GlobalVariables_addVar(GlobalVariables var, char *name, int val){
    // if there's no more room
    if(var.currentVariables == var.maxVariables){
        return var;
    }

    char * nameAddr = *(var.variableNames + var.currentVariables);
    void * varAddr = var.variableValues + var.currentVariables;

    *(var.variableNames + var.currentVariables) = name;
    *(var.variableValues + var.currentVariables) = val;

    var.currentVariables++;


    return var;
}

// Globals

char isNumber(char *s){
    int len = strlen(s);
    for (int i = 0; i < len; i++) {
        if(i == 0 && (s[i] == '+' || s[i] == '-')){
            continue;
        }
        if(i == len - 1 && s[i] == '\n'){
            continue;
        }
        if (isdigit(s[i]) == false) 
            return false; 
    }

    return true; 
}

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

// Returns the first index of chr in str, else returns -1
int findCharOnString(char *str, char chr){
    char stop = false;
    int i = 0;
    int result = -1;
    for (; *(str + i) != 0 && stop == false; i++)
    {
        if(*(str + i) == chr){
            result = i;
            stop = true;
        }
    }
    return result;
}

char **divideStringByIndex(char *str, int index){
    int length = strlen(str);

    char *buffer1 = (char*)malloc(index);
    char *buffer2 = (char*)malloc(length - index - 1);

    char *ptr = str;
    strncpy(buffer1, ptr, index);
    ptr += index + 1;
    strcpy(buffer2, ptr);

    char **dual = (char**)malloc(sizeof(char**));

    *dual = buffer1;
    *(dual + 1) = buffer2;
    return dual;
}

// finds the index of text in lst, else -1
int getIndexOnList(char **lst, int length, char *text){
    char **ptr = lst;
    char *current;
    char null = false;

    for (int i = 0; i < length && null == false; i++)
    {
        ptr = lst + i;
        current = *ptr;

        if(current == NULL){
            return -1;
        }

        if(*current == '\x00'){
            null = true;
            continue;
        }

        if(strcmp(text, current) == 0){
            return i;
        }
    }
    return -1;
}

char isReservedWord(char *word){
    if(
    strEqu(word, RESERVED_TRUE) ||
    strEqu(word, RESERVED_FALSE)
    )
    {
        return true;
    }
    return false;
}

// The function gets a spaced string and copy the string to the heap without spaces
char *stripString(char *text){
    // TODO: don't remove spaces on strings

    char buffer[strlen(text)];
    char *ptr;
    int newLen = 0;
    
    for(ptr = text; *ptr != 0; ptr++){
        if(!(*ptr == ' ' || *ptr == '\t')){
            *(buffer + newLen) = *ptr;
            newLen++;
        }
    }

    // terminate last char with null byte
    *(buffer + newLen) = 0;

    char *newStr = (char*)malloc(newLen);
    strncpy(newStr, buffer, newLen);

    return newStr;
}

void init(){
    globals.maxVariables = MAX_VARIABLES;
    globals.currentVariables = 0;
    globals.variableNames = (char**)malloc(MAX_VARIABLES * sizeof(char**));
    globals.variableValues = (void*)malloc(MAX_VARIABLES * sizeof(void*));

    memset(globals.variableNames, 0, MAX_VARIABLES * sizeof(char**));
    memset(globals.variableValues, 0, MAX_VARIABLES * sizeof(int*));
}

void parseCommand(char *buffer){
    // print
    if(strEqu(buffer, CMD_PRINT)){
        fgets(buffer, BUFFER_SIZE, stdin);
        buffer++;
        printf("%s", buffer);
    }
    // clear
    else if (strEqu(buffer, CMD_CLEAR))
    {
        system("clear");
    }
    // get
    else if (strEqu(buffer, CMD_GET))
    {
        int index = getIndexOnList(globals.variableNames, globals.maxVariables, buffer+1);
        if(index == -1){
            printf("%s is not defined\n", buffer+1);
        }
        else{
            int value = *(globals.variableValues + index);
            printf("%d\n", value);
        }
    }
    else{
        int i;
        i = findCharOnString(buffer, '=');
        // Check if '=' in line
        if(i != -1){
            char **vars = divideStringByIndex(buffer, i);
            
            // If we've got a number
            if(isNumber(*(vars+1))){
                int temp = atoi(*(vars+1));
                // add the variable and save it in globals
                globals = GlobalVariables_addVar(globals, *vars, temp);
                free(vars[1]);
            }

        }
        else{
            printf("Syntax error\n");
        }
    }
    
}


int main(int argc, char *argv[]){
    if(argc != 2)
        exit(1);

    #ifndef DEBUG
    init();
    if(strEqu(argv[1], EXECUTION_STDIN)){
        // scan for input
        char buffer[BUFFER_SIZE];
        printf("(krypton) ");
        scanf("%s", buffer);

        // while not CMD_EXIT, parseCommand
        while (strcmp(buffer, CMD_EXIT) != 0)
        {
            parseCommand(buffer);
            printf("(krypton) ");
            scanf("%s", buffer);
        }
    }
    else if (strEqu(argv[1], EXECUTION_FILE))
    {
        // scan for input
        char buffer[BUFFER_SIZE];
        scanf("%s", buffer);

        // while not CMD_EXIT, parseCommand
        while (strcmp(buffer, CMD_EXIT) != 0)
        {
            parseCommand(buffer);
            scanf("%s", buffer);
        }
    }
    else
        exit(1);
    
    #else
    char buffer[100];
    fgets(buffer, 100, stdin);

    char *newStr = stripString(buffer);
    printf(newStr);
    #endif

    return 0;
}