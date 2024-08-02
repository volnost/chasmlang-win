#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Node {
    char name[32];
    int value;
    struct Node* next;
};


struct Node* createNode(const char* name, int value) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    strncpy(newNode->name, name, 31);
    newNode->name[31] = '\0';
    newNode->value = value;
    newNode->next = NULL;
    return newNode;
}

void append(struct Node** headRef, const char* name, int value) {
    struct Node* newNode = createNode(name, value);
    struct Node* last = *headRef;
    
 
    if (*headRef == NULL) {
        *headRef = newNode;
        return;
    }


    while (last->next != NULL) {
        last = last->next;
    }

    last->next = newNode;
}


struct Node* getNodeAtPosition(struct Node* head, int position) {
    struct Node* current = head;
    int count = 0;
    
    while (current != NULL) {
        if (count == position) {
            return current;
        }
        count++;
        current = current->next;
    }


    printf("Position out of bounds\n");
    return NULL;
}

struct Node* getNodeByName(struct Node* head, const char* name) {
    struct Node* current = head;
    
    while (current != NULL) {
        if (strcmp(current->name, name) == 0) {
            return current;
        }
        current = current->next;
    }

    //printf("Node with name %s not found\n", name);
    return NULL;
}

int updateNodeByName(struct Node* head, const char* name, const char* newName, int newValue) {
    struct Node* current = head;
    
    while (current != NULL) {
        if (strcmp(current->name, name) == 0) {
            if (newName != NULL) {
                strncpy(current->name, newName, 31);
                current->name[31] = '\0';  
            }
            if(newValue!=NULL) {
                current->value = newValue;
            }
            return 1; 
        }
        current = current->next;
    }
}