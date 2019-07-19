#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define DEBUG
#define false   0
#define true    1

#define BUFFER_SIZE     200
#define MAX_VARIABLES   100


#define strEqu(BUFFER, STR) strncmp(BUFFER, STR, strlen(STR)) == 0
#define explode(STR, CHR) divideStringByIndex(STR, findCharOnString(STR, CHR));
#define isdigit(CHR) (48 <= CHR && CHR <= 57)

#define CMD_PRINT   "print"
#define CMD_CLEAR   "clear"
#define CMD_GET     "$"


// TODO: replace strlen(CMD) with const number


struct GlobalVariables
{
    char **variableNames;
    int *variableValues;
    int maxVariables;
    int currentVariables;
}typedef GlobalVariables;

// The function assumes that there's no name collision
char GlobalVariables_addVar(GlobalVariables globals, char *name, int val){
    // if there's no more room
    if(globals.currentVariables == globals.maxVariables){
        return false;
    }
    
    char * nameAddr = *(globals.variableNames + globals.currentVariables);
    void * varAddr = globals.variableValues + globals.currentVariables;

    *(globals.variableNames + globals.currentVariables) = name;
    *(globals.variableValues + globals.currentVariables) = val;

    globals.currentVariables++;

    printf("success\n");

    return true;

}

// Globals
GlobalVariables globals;

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

    for (int i = 0; i < length; i++)
    {
        current = *(ptr + i);
        if(strncmp(current, text, strlen(current)) == 0){
            return i;
        }
    }
    return -1;
}

void init(){
    //variableNames = (char**)malloc(MAX_VARIABLES * sizeof(char**));
    //variableValues = (void*)malloc(MAX_VARIABLES * sizeof(void*));
    globals.maxVariables = MAX_VARIABLES;
    globals.currentVariables = 0;
    globals.variableNames = (char**)malloc(MAX_VARIABLES * sizeof(char**));
    globals.variableValues = (void*)malloc(MAX_VARIABLES * sizeof(void*));

    memset(globals.variableNames, 0, MAX_VARIABLES * sizeof(char**));
    memset(globals.variableValues, 0, MAX_VARIABLES * sizeof(void*));
}

void parseCommand(char *buffer){
    if(strEqu(buffer, CMD_PRINT)){
        fgets(buffer, BUFFER_SIZE, stdin);
        buffer++;
        printf("%s", buffer);
    }
    else if (strEqu(buffer, CMD_CLEAR))
    {
        system("clear");
    }
    else if (strEqu(buffer, CMD_GET))
    {
        printf("%d\n", getIndexOnList(globals.variableNames, globals.maxVariables, "asdads"));
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

                //int *valAddr = (int*)malloc(sizeof(int));
                //*valAddr = temp;

                //free vars[1]

                GlobalVariables_addVar(globals, *vars, temp);
            }

        }
        else{
            printf("Syntax error\n");
        }
    }
    
}


int main(){
    #ifndef DEBUG
    init();

    char buffer[BUFFER_SIZE];
    scanf("%s", buffer);

    while (strcmp(buffer, "exit") != 0)
    {
        parseCommand(buffer);
        scanf("%s", buffer);
    }
    
    return 0;
    #endif

}

