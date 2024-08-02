#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <ctype.h>
#include "linkedlist.h"



const char* code = "int a 1\nint b 2\nintout a\nlnout\ninc a !b\nintout a\nend";
const char* allowedChars = "abcdefghijklmnoprstquvwxyzABCDEFGHIJKLMNOPRSTQUVWXYZ./!1234567890+-?";

int enumerate_lines(const char* text) {
    int b = 1;
    for (int a = 0; a < strlen(text); a++) {
        if (text[a] == '\n')
            b++;
    }
    return b;
}

char* get_line(int line, const char* text) {
    if (line < 1) {
        return NULL;
    }

    const char* start = text;
    const char* end = text;
    int current_line = 1;
    while (*end != '\0') {
        if (*end == '\n') {
            if (current_line == line) {
                break;
            }
            current_line++;
            start = end + 1;
        }
        end++;
    }

    if (current_line != line) {
        return NULL; 
    }
    const char* line_end = strchr(start, '\n');
    if (line_end == NULL) {
        line_end = end; 
    }

    size_t line_length = line_end - start;
    char* result = (char*)malloc(line_length + 1);
    if (result == NULL) {
        return NULL;
    }

    strncpy(result, start, line_length);
    result[line_length] = '\0'; 

    return result;
}
int enumerate_arguments(char* line) {
    int ammount = 1;
    for (int a = 0; a < strlen(line); a++) {
        if (line[a] == ' ')
            ammount++;
    }
    return ammount;
}
int is_allowed(char letter) {
    for (int a = 0; a < strlen(allowedChars); a++) {
        if (letter == allowedChars[a])
            return 1;
    }
    return 0;
}
int get_string_length(char* string) {
    if (string == NULL) 
        return -1;
    char a = 1;
    int num = 0;
    while (a != '\0') {
        a = string[num];
        num++;
    }
    return num;
}

char** argumentize_line(char* line) {
    int max = enumerate_arguments(line);
    char** buffer = (char**)malloc((max + 1) * sizeof(char*));
    int arg = 0;
    int at = 0;
    int counter = 0;

    buffer[arg] = (char*)malloc(strlen(line) + 1);

    while (line[counter] != '\0') {
        if (is_allowed(line[counter])) {
            buffer[arg][at++] = line[counter];
        } else if (line[counter] == ' ' && at > 0) {
            buffer[arg][at] = '\0';
            arg++;
            at = 0;
            buffer[arg] = (char*)malloc(strlen(line) + 1);
        }
        counter++;
    }

    if (at > 0) {
        buffer[arg][at] = '\0';
        arg++;
    }

    buffer[arg] = NULL; // Null-terminate
    return buffer;
}

char *itoa(long n)
{
    int len = n==0 ? 1 : floor(log10l(labs(n)))+1;
    if (n<0) len++; // room for negative sign '-'

    char    *buf = calloc(sizeof(char), len+1); // +1 for null
    snprintf(buf, len+1, "%ld", n);
    return   buf;
}

/*
    char* teststring = "abc cdf";
    char** args = argumentize_line(teststring);
    printf("Arguments: %d\n", enumerate_arguments(teststring));
    printf("Length: %d\n", get_string_length(teststring));
    for (int a = 0; a < enumerate_arguments(teststring); a++) {
        printf("%d: %s\n", a, args[a]);
    }
    free(args);
*/



int main() {
    //RUNTIME DEFINITIONS
    struct Node* int_head = NULL;
    struct Node* jmp_head = NULL;
    int lines = enumerate_lines(code);
    int line = 1;
    int eargs = 0;

    printf("LINES: %d, running...\n", lines);


    //RUNNING
    while (line<lines) {
        char* line_str = get_line(line, code);
        //printf("LINE: %s... ", line_str);
        char** args = argumentize_line(line_str);
        int eargs = enumerate_arguments(line_str);
        for (int e = 0; e < eargs; e++) {
            if (args[e][0] == '!') {
                for (int f = 0; f < strlen(args[e]); f++) {
                    args[e][f] = args[e][f+1];
                }
                struct Node* tempnode = getNodeByName(int_head, args[e]);
                memset(args[e], '\0', strlen(args[e]));
                strcpy(args[e], itoa(tempnode->value));
            }
        }



        if (strcmp(args[0], "int") == 0) {
            int n = 0;
            if (args[2] != NULL) {
                append(&int_head, args[1], atoi(args[2]));
            } else {
                append(&int_head, args[1], 0);
            }
        } else if (strcmp(args[0], "intout") == 0) {
            struct Node* tempnode = getNodeByName(int_head, args[1]);
            if (tempnode == NULL) {
                printf("ERROR ON LINE %d, integer '%s' doesn't exist.", line, args[1]);
            } else {
                printf("%d", tempnode->value);
            }
        } else if (strcmp(args[0], "inc") == 0) {
            struct Node* tempnode = getNodeByName(int_head, args[1]);
            updateNodeByName(int_head, args[1], NULL, tempnode->value + atoi(args[2]));
        } else if (strcmp(args[0], "dec") == 0) {
            struct Node* tempnode = getNodeByName(int_head, args[1]);
            updateNodeByName(int_head, args[1], NULL, tempnode->value - atoi(args[2]));
        } else if (strcmp(args[0], "end") == 0) {
            printf("Ending..");
            break;
        } else if (strcmp(args[0], "renint") == 0) {
            //updateNodeByName(int_head, args[1], args[2], NULL);
        } else if (strcmp(args[0], "add") == 0) {
            struct Node* tempnode = getNodeByName(int_head, args[2]);
            struct Node* tempnode2 = getNodeByName(int_head, args[1]);
            if (tempnode != NULL) {
                updateNodeByName(int_head, args[1], NULL, tempnode->value + tempnode2->value);
            }
        } else if (strcmp(args[0], "lnout") == 0) {
            printf("\n");
        }
        line++;

        free(line_str);
        free(args);
        //printf("\n");
    }

    //POST-RUN CLEANUP
    while (int_head != NULL) {
        struct Node* temp = int_head;
        int_head = int_head->next;
        free(temp);
    }
}