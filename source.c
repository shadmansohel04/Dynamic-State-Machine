#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

struct node {
    char letter;
    struct node* before;
    struct node* next;
};

struct node states[8];

int state = 3; //D is initial state
char command[10];

int stateNum(char c){
    switch(c){
        case 'A':
            return 0;
        case 'B':
            return 1;
        case 'C':
            return 2;
        case 'D':
            return 3;
        case 'E':
            return 4;
        case 'F':
            return 5;
        case 'G':
            return 6;
        case 'H':
            return 7;
        default:
            printf("\nnot a valid state\n\n"); 
            return -1;
    }
}

void nextState(int next){
    if(next == 0){
        state = stateNum(states[state].before -> letter);
    }
    else if(next == 1){
        state = stateNum(states[state].next -> letter);
    }
    else{
        printf("\nInvalid Input\n\n");
    }
}

void changeState(int to, int change){    
    if(to == 0 && change >= 0 && change <= 7){
        states[state].before = &states[change];
    }
    else if(to == 1 && change >= 0 && change <= 7){
        states[state].next = &states[change];
    }
    else{
        printf("\nInvalid Input\n\n");
    }
}

void printer(){
    for(int i = 0; i < 8; i++){
        if(states[i].before != NULL && states[i].next != NULL){
            printf("%c %c %c\n", states[i].letter, states[i].before -> letter, states[i].next -> letter);
        }
    }
}

char nongarb[8];
char garb[] = "ABCDEFGH";

int garbageChecker(struct node current){
    for(int i = 0; i < 8; i++){
        if(current.letter == nongarb[i]){
            return 1;
        }
        if(nongarb[i] == '\0'){
            nongarb[i] = current.letter;
            break;
        }
    }
    return 0;
}

void garbage(struct node *current) {
    if (garbageChecker(*current) == 0) {
        garbage(current->before);
        garbage(current->next);
    }
}

int emptyGarb(){
    for(int i = 0; i < 8; i++){
        if(garb[i] != '\0'){
            return 1;
        }
    }
    return 0;
}

void garbagestates(){
    for(int i = 0; i < 8; i++){
        for(int j = 0; j <8 ; j++){
            if(garb[i] == nongarb[j]){
                garb[i] = '\0';
            }
        }
    }
    if(emptyGarb() == 0){
        printf("No Garbage\n");
    }

    else{
        printf("Garbage: ");
        for(int i = 0; i < sizeof(garb)/sizeof(garb[0]); i++){
            if(garb[i] != '\0'){
                printf("%c ", garb[i]);
            }
        }
        printf("\n");
    }


}

void deleter(char command[10]){
    if(strcmp(command, "d\n") == 0){
        memset(nongarb, '\0', sizeof(nongarb)); // Reset nongarb array
        strcpy(garb, "ABCDEFGH"); // Reset garb array
        garbage(&states[state]);
        garbagestates();
        if(emptyGarb() == 1){
            printf("\nDeleted: ");
            for(int i = 0; i < sizeof(garb)/sizeof(garb[0]); i++){
                for(int j = 0; j < 8; j++){
                    if(garb[i] == states[j].letter){
                        states[j].before = NULL;
                        states[j].next = NULL;
                        states[j].letter = ' ';
                    }
                }
                if(garb[i] != '\0')
                    printf("%c ", garb[i]);
            }
            printf("\n");
        }
        else{
            printf("No states deleted\n");
        }
    }
    int state = stateNum(command[2]);
    if(command[0] == 'd' && state >= 0 && state <= 7){
        if(states[state].before != NULL && states[state].next != NULL && states[state].letter != ' '){
            states[state].before = NULL;
            states[state].next = NULL;
            states[state].letter = ' ';
            printf("Deleted\n");
        }
        else{
            printf("Not Deleted\n");
        } 
    }

}

int main() {
    // Initialize nodes
    states[0].letter = 'A';
    states[1].letter = 'B';
    states[2].letter = 'C';
    states[3].letter = 'D';
    states[4].letter = 'E';
    states[5].letter = 'F';
    states[6].letter = 'G';
    states[7].letter = 'H';

    //A
    states[0].before = &states[4]; //E
    states[0].next = &states[3]; //D
    
    //B
    states[1].before = &states[2]; //C
    states[1].next = &states[1]; //B
    
    //C
    states[2].before = &states[7]; //H
    states[2].next = &states[4]; //E
    
    //D
    states[3].before = &states[6]; //G
    states[3].next = &states[4]; //F
    
    //E
    states[4].before = &states[5]; //F
    states[4].next = &states[7]; //H
    
    //F
    states[5].before = &states[3]; //D
    states[5].next = &states[1]; //B
    
    //G
    states[6].before = &states[1]; //B
    states[6].next = &states[0]; //A
    
    //H
    states[7].before = &states[0]; //A
    states[7].next = &states[6]; //G

    while(strcmp(command,"end\n") != 0){
        printf("Current state:\n%c\nEnter command now: ", states[state].letter);
        fgets(command, sizeof(command), stdin);

        //moving from state to state
        if(isdigit(command[0])){
            int next = atoi(command);
            nextState(next);
        }
        
        //change command
        else if (command[0] == 'c' && command[1] == ' ' && isdigit(command[2])) {
            int to = command[2] - '0';
            int change = stateNum(command[4]);
            changeState(to, change);
        }

        else if (command[0] == 'p'){
            printer();
        }

        else if(command[0] == 'g'){
            memset(nongarb, '\0', sizeof(nongarb)); // Reset nongarb array
            strcpy(garb, "ABCDEFGH"); // Reset garb array
            garbage(&states[state]);
            garbagestates();
        }

        else if(command[0] == 'd'){
            deleter(command);
        }

        else{
            printf("\nNot valid command\n\n");
        }
        
    }
           
    return 0;
}
