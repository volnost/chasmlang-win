#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LABELS 64
#define MAX_INTS 64
#define MAX_STRS 64

struct intvar {
    char name[32];
    int value;
};

struct strvar {
    char name[32];
    char value[128];
};

int main() {
    struct intvar* ints = (struct intvar*)malloc(MAX_INTS * sizeof(struct intvar));
    struct intvar* jmps = (struct intvar*)malloc(MAX_LABELS * sizeof(struct intvar));
    struct strvar* strs = (struct strvar*)malloc(MAX_STRS * sizeof(struct strvar));

    const char* code = "int a\n"
                       "set a 10\n"
                       "intout a\n"
                       "end\n";
                       
    int line = 0;
    int at = 0;
    int def_at = 0;
    int var_at = 0;
    int int_def_at = 0;
    int str_def_at = 0;
    int jmp_def_at = 0;
    char bufferline[127] = {'\0'};
    char argbuffer[3][31] = {{'\0'}};

    while (1) {
        if (code[at] != '\n' && code[at] != '\0') { // Add chars into bufferline until end of line or end of string
            bufferline[var_at] = code[at];
            if (code[at] != ' ') { // Add chars into argument
                argbuffer[def_at][var_at] = code[at];
                var_at++;
            } else { // Raise argument pointer and reset position to put into the argument
                argbuffer[def_at][var_at] = '\0'; // Null-terminate the current argument
                def_at++;
                var_at = 0;
            }
            at++; // Raise character if not end of line
        } else {
            bufferline[var_at] = '\0'; // Null-terminate the line buffer
            argbuffer[def_at][var_at] = '\0'; // Null-terminate the last argument
            
            if (strcmp(argbuffer[0], "int") == 0 && int_def_at < MAX_INTS) {
                strcpy(ints[int_def_at].name, argbuffer[1]);
                ints[int_def_at].value = 0;
                int_def_at++;
            } else if (strcmp(argbuffer[0], "str") == 0 && str_def_at < MAX_STRS) {
                strcpy(strs[str_def_at].name, argbuffer[1]);
                str_def_at++;
            } else if (strcmp(argbuffer[0], "label") == 0 && jmp_def_at < MAX_LABELS) {
                strcpy(jmps[jmp_def_at].name, argbuffer[1]);
                jmps[jmp_def_at].value = at;
                jmp_def_at++;
            } else if (strcmp(argbuffer[0], "end") == 0) {
                break;
            } else if (strcmp(argbuffer[0], "intout") == 0) {
                for (int a = 0; a < int_def_at; a++) {
                    if (strcmp(argbuffer[1], ints[a].name) == 0) {
                        printf("%d\n", ints[a].value);
                        break;
                    }
                }
            } else if (strcmp(argbuffer[0], "set") == 0) { // Corrected command from "intset" to "set"
                for (int a = 0; a < int_def_at; a++) {
                    if (strcmp(argbuffer[1], ints[a].name) == 0) {
                        ints[a].value = atoi(argbuffer[2]);
                        break;
                    }
                }
            }
            
            // Reset for the next line
            var_at = 0;
            def_at = 0;
            line++;
            at++; // Move to the next character after newline

            // Clear line buffer and argument buffer
            memset(bufferline, '\0', sizeof(bufferline));
            for (int n = 0; n < 3; n++) {
                memset(argbuffer[n], '\0', sizeof(argbuffer[n]));
            }
        }
    }

    free(strs);
    free(ints);
    free(jmps);

    return 0;
}
