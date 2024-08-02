#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LABELS 64
#define MAX_INTS 64
#define MAX_STRS 64
#define MAX_LINKS 8

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

struct link {
    int number;
    int position;
};

void createSubArray(int *original, int originalSize, int start, int length, int **subArray) {
    if (start < 0 || start >= originalSize || length <= 0 || start + length > originalSize) {
        printf("Invalid bounds\n");
        *subArray = NULL;
        return;
    }

    *subArray = (int *)malloc(length * sizeof(int));
    if (*subArray == NULL) {
        printf("Memory allocation failed\n");
        return;
    }

    for (int i = 0; i < length; i++) {
        (*subArray)[i] = original[start + i];
    }
}

int find_label_position(struct label* labels, int label_count, const char* labelname) {
    for (int i = 0; i < label_count; i++) {
        if (strcmp(labels[i].name, labelname) == 0) {
            return labels[i].position;
        }
    }
    return -1;
}

int isAllowedChar(char c) {
    return isalpha(c)  || isdigit(c) || c == ' ' || c == '\n' || c == '!' || c == '.';
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    // Open the file
    FILE *file = fopen(argv[1], "r");
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }

    // Determine the file size
    fseek(file, 0L, SEEK_END); // Move to the end of the file
    long fileSize = ftell(file); // Get the current file pointer (file size)
    fseek(file, 0L, SEEK_SET);
    //printf("%lo", fileSize);

    // Allocate memory to hold the file content
    char *fileContent = malloc(fileSize + 1); // +1 for the null terminator
    if (fileContent == NULL) {
        perror("Error allocating memory");
        fclose(file);
        return 1;
    }

    // Read the file content
    size_t bytesRead = fread(fileContent, 1, fileSize, file);
    if (bytesRead != fileSize) {
        perror("Error reading file");
        free(fileContent);
        fclose(file);
        return 1;
    }
    fileContent[fileSize] = '\0'; // Null-terminate the string


    // Close the file
    fclose(file);
    //printf("File: %s", fileContent);
    //getchar();
    //printf("%s", fileContent);
    //for (int i = 0; i < fileSize; i++) {
    //    printf("%d: %c", i, fileContent[i]);
    //}
    struct intvar* ints = (struct intvar*)malloc(MAX_INTS * sizeof(struct intvar));
    struct label* labels = (struct label*)malloc(MAX_LABELS * sizeof(struct label));
    struct strvar* strs = (struct strvar*)malloc(MAX_STRS * sizeof(struct strvar));

    //struct links* links = (struct link*)malloc(MAX_LINKS * sizeof(struct link));

    //const char* code = "int a\n.Numbers from 1 to 10:\nlnout\nlabel nigga unread\nadd a 1\nret\nread\nintset a 0\nlabel loop\ndo nigga\nintout a !\nintcmp a 10\njee\njump loop";
                       
    int line = 0;              //LINE COUNTER
    int at = 0;                //CHARACTER IN CODE
    int def_at = 0;            //WHICH ARGUMENT ARE WE AT
    int var_at = 0;            //WHICH CHARACTER IN THE ARGUMENT ARE WE AT
    int bufferline_at = 0;
    int int_def_at = 0;        //TOTAL INT DEFINITIONS
    int str_def_at = 0;        //TOTAL STRING DEFINITIONS
    int label_def_at = 0;      //TOTAL LABEL DEFINITIONS
    int comparison_result = 2; //0 = equal; 1 = greater than; -1 = smaller than; set as 2 as not to exit when running for the first time
    //                         IMPLEMENT RESETING AFTER JUMP
    char bufferline[127] = {'\0'};    //BUFFER FOR LINE
    char argbuffer[3][31] = {{'\0'}}; //BUFFER FOR ARGUMENTS
    char inputbuffer[127] = {'\0'};
    int newline = 1;           //ARE WE ON A NEW LINE?
    size_t linelen = 0;
    //int linkactive = 0; //IS LINKING ACTIVE? 0 - no, 1 - yes, 2 - link is running
    //int activelink = 0; //WHICH LINK IS ACTIVE? 1-4
    //int link_def_at = 0; //TOTAL LINKS
    int is_doing = 0; //ARE WE DOING SOMETHING?
    int do_jmp = 0; //WHERE DO WE GO AFTER WE DO THIS?
    int unread = 0; //ARE WE READING AN UNREAD LABEL?

    while (at < fileSize) {
        if (!isAllowedChar(fileContent[at])) {
            at++;
            continue;
        }
        if (fileContent[at] != '\n' && fileContent[at] != '\0') { // Add chars into bufferline until end of line or end of string
            if (fileContent[at] != '\t') {
                bufferline[bufferline_at] = fileContent[at];
                bufferline_at++;
            } else {

            }
            if (fileContent[at] != ' ') { // Add chars into argument
                argbuffer[def_at][var_at] = fileContent[at];
                var_at++;
            } else { // Raise argument pointer and reset position to put into the argument
                argbuffer[def_at][var_at] = '\0'; // Null-terminate the current argument
                def_at++;
                var_at = 0;
            }
            at++; // Raise character if not end of line
        } else {
            newline = 1;
            bufferline[bufferline_at + 1] = '\0'; // Null-terminate the line buffer
            argbuffer[def_at][var_at] = '\0'; // Null-terminate the last argument
            //printf("%s\n", bufferline);       // Silly little debugging, shows the entire line to be processed obviously
            if (bufferline[0] == '.') {
                //int *subarray = NULL;
                //createSubArray(bufferline, sizeof(bufferline) / sizeof(bufferline[0]), 1, 127, &subarray);
                for (int a = 0; a < bufferline_at; a++) {
                    bufferline[a] = bufferline[1 + a];
                }   
                printf("%s", bufferline);
            } else if (unread == 1 && strcmp(argbuffer[0], "read") != 0) {
                goto clean;
            } else if (unread == 1 && strcmp(argbuffer[0], "read") == 0) {
                unread = 0;
            } else if (strcmp(argbuffer[0], "do") == 0) {
                int jump_to = find_label_position(labels, label_def_at, argbuffer[1]);
                if (jump_to != -1) {
                    is_doing = 1;
                    do_jmp = at;
                    at = jump_to;
                } else {
                    printf("Error: Label %s not found\n", argbuffer[1]);
                }
            } else if (strcmp(argbuffer[0],"ret") == 0) {
                if (is_doing == 1) {
                    at = do_jmp;
                    is_doing = 0;
                }
            } else if (bufferline[0] == '!') {
                //linelen = strlen(bufferline);
                //bufferline[0] = '\0';
                //strcpy(inputbuffer, bufferline + 1);
                goto clean;
            }
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
                if (strcmp(argbuffer[2], "unread") == 0) {
                    unread = 1;
                }
            } else if (strcmp(argbuffer[0], "end") == 0) {
                break;
            } else if (strcmp(argbuffer[0], "intout") == 0) {
                for (int a = 0; a < int_def_at; a++) {
                    if (strcmp(argbuffer[1], ints[a].name) == 0) {
                        printf("%d", ints[a].value);
                        break;
                    }
                }
                    if (strcmp(argbuffer[2], "!") == 0) {
                        printf("\n");
                }
            } else if (strcmp(argbuffer[0], "strin") == 0) {
                for (int a = 0; a < str_def_at; a++) {
                    if (strcmp(argbuffer[1], strs[a].name) == 0) {
                        scanf("%s", &inputbuffer);
                        strcpy(strs[a].value, inputbuffer);
                    }
                }
            } else if (strcmp(argbuffer[0], "strout") == 0) {
                for (int a = 0; a < str_def_at; a++) {
                    if (strcmp(argbuffer[1], strs[a].name) == 0 ) {
                        printf("%s", strs[a].value);
                    }
                    if (strcmp(argbuffer[2], "!") == 0) {
                        printf("\n");
                    } 
                }
            } else if (strcmp(argbuffer[0], "intset") == 0) {
                for (int a = 0; a < int_def_at; a++) {
                    if (strcmp(argbuffer[1], ints[a].name) == 0) {
                        ints[a].value = atoi(argbuffer[2]);
                        break;
                    }
                }
            } else if (strcmp(argbuffer[0], "strset") == 0) {
                for (int a = 0; a < str_def_at; a++) {
                    if (strcmp(argbuffer[1], strs[a].name) == 0) {
                        // = strlen(strs[a].name);
                        //memset(bufferline, '\0', 7+linelen);
                        //strncpy(bufferline, strs[a].value, 127);
                        strcpy(strs[a].value, inputbuffer);
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
            } else if (strcmp(argbuffer[0], "intptr") == 0) {
                for (int a = 0; a < int_def_at; a++) {
                    if (strcmp(ints[a].name, argbuffer[1]) == 0) {
                        for (int b = 0; b < int_def_at; b++) {
                            if (strcmp(ints[b].name, argbuffer[2]) == 0) {
                                ints[a].value = b;
                            }
                        }
                    }
                }
            } else if (strcmp(argbuffer[0], "lnout") == 0) {
                printf("\n");
            }
            clean:
            //printf("\nCOMP RES %d", comparison_result);
            
            // Reset for the next line
            bufferline_at = 0;
            var_at = 0;
            def_at = 0;
            line++;
            at++; // Move to the next character after newline

            // Clear line buffer and argument buffer
            memset(bufferline, 0, strlen(bufferline));
            for (int n = 0; n < 3; n++) {
                memset(argbuffer[n], '\0', sizeof(argbuffer[n]));
            }
        }
    }
    free(fileContent);
    free(strs);
    free(ints);
    free(labels);

    return 0;
}
