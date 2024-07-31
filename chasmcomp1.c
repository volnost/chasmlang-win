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

struct label {
    char name[32];
    int position;
};

int find_label_position(struct label* labels, int label_count, const char* labelname) {
    for (int i = 0; i < label_count; i++) {
        if (strcmp(labels[i].name, labelname) == 0) {
            return labels[i].position;
        }
    }
    return -1;
}

int main() {
    struct intvar* ints = (struct intvar*)malloc(MAX_INTS * sizeof(struct intvar));
    struct label* labels = (struct label*)malloc(MAX_LABELS * sizeof(struct label));
    struct strvar* strs = (struct strvar*)malloc(MAX_STRS * sizeof(struct strvar));

    const char* code = "int a\n"   //CODE FOR TESTING PURPOSES
                       "label start\n"
                       "intout a\n"
                       "add a 1\n"
                       "intcmp a 64\n"
                       "jee\n"
                       "jump start\n"
                       "end\n";
                       
    int line = 0;              //LINE COUNTER
    int at = 0;                //CHARACTER IN CODE
    int def_at = 0;            //WHICH ARGUMENT ARE WE AT
    int var_at = 0;            //WHICH CHARACTER IN THE ARGUMENT ARE WE AT
    int int_def_at = 0;        //TOTAL INT DEFINITIONS
    int str_def_at = 0;        //TOTAL STRING DEFINITIONS
    int label_def_at = 0;      //TOTAL LABEL DEFINITIONS
    int comparison_result = 2; //0 = equal; 1 = greater than; -1 = smaller than; set as 2 as not to exit when running for the first time
    //                         IMPLEMENT RESETING AFTER JUMP
    char bufferline[127] = {'\0'};    //BUFFER FOR LINE
    char argbuffer[3][31] = {{'\0'}}; //BUFFER FOR ARGUMENTS

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
            } else if (strcmp(argbuffer[0], "label") == 0 && label_def_at < MAX_LABELS) {
                strcpy(labels[label_def_at].name, argbuffer[1]);
                labels[label_def_at].position = at; // Store the position after the label
                label_def_at++;
            } else if (strcmp(argbuffer[0], "end") == 0) {
                break;
            } else if (strcmp(argbuffer[0], "intout") == 0) {
                for (int a = 0; a < int_def_at; a++) {
                    if (strcmp(argbuffer[1], ints[a].name) == 0) {
                        printf("%d\n", ints[a].value);
                        break;
                    }
                }
            } else if (strcmp(argbuffer[0], "set") == 0) {
                for (int a = 0; a < int_def_at; a++) {
                    if (strcmp(argbuffer[1], ints[a].name) == 0) {
                        ints[a].value = atoi(argbuffer[2]);
                        break;
                    }
                }
            } else if (strcmp(argbuffer[0], "!add") == 0) {
                for (int a = 0; a < int_def_at; a++) {
                    if (strcmp(argbuffer[1], ints[a].name) == 0) {
                        for (int b = 0; b < int_def_at; b++) {
                            if (strcmp(argbuffer[1], ints[b].name) == 0) {
                                ints[a].value += ints[b].value;
                            }
                        }
                    }
                }
            } else if (strcmp(argbuffer[0], "add") == 0) {
                for (int a = 0; a < int_def_at; a++) {
                    if (strcmp(argbuffer[1], ints[a].name) == 0) {
                        ints[a].value += atoi(argbuffer[2]);
                    }
                }
            } else if (strcmp(argbuffer[0], "intcmp") == 0) {
                for (int a = 0; a < int_def_at; a++) {
                    if (strcmp(argbuffer[1], ints[a].name) == 0) {
                        if (ints[a].value == atoi(argbuffer[2])) {
                            comparison_result = 0;
                        } else if (ints[a].value > atoi(argbuffer[2])) {
                            comparison_result = 1;
                        } else {
                            comparison_result = -1;
                        }
                    }
                }
            } else if (strcmp(argbuffer[0], "jump") == 0) {
                int jump_to = find_label_position(labels, label_def_at, argbuffer[1]);
                if (jump_to != -1) {
                    at = jump_to; // Set the position to the label's position
                    //continue; // Skip the resetting of buffers and directly go to the next iteration
                } else {
                    printf("Error: Label %s not found\n", argbuffer[1]);
                }
            } else if (strcmp(argbuffer[0], "je") == 0) {
                int jump_to = find_label_position(labels, label_def_at, argbuffer[1]);
                if (jump_to != -1 && comparison_result == 0) {
                    at = jump_to;
                    continue;
                } else {
                    printf("Error: Label %s not found\n", argbuffer[1]);
                }
            } else if (strcmp(argbuffer[0], "jee") == 0) {
                if (comparison_result == 0) {
                    break;
                }
            }
            //printf("\nCOMP RES %d", comparison_result);
            
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
    free(labels);

    return 0;
}
