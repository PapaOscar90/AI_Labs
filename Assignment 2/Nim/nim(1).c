#include <stdio.h>
#include <stdlib.h>

#define MAX 1
#define MIN -1
#define INFINITY 9999999

int recursiveBestMove;
int beginingState;
int lookUpTable[100];

int negaMaxDecision(int state, int color) {
    int result;

    if (state == 1) {
        lookUpTable[state]=-1*color;
        return (-1*color);
    }


    int v = -INFINITY;
    for (int i = 1; i < 4; i++) {
        if (state - i >=0) {
            if (state - i == 0){
                result = -1*color;
            }else {
                if (lookUpTable[state-i] !=0){
                    result = color * lookUpTable[state-i];
                }else {
                    result = -1 * negaMaxDecision(state - i, -1 * color);
                }
            }
            if (v <= result) {
                if(beginingState == state) {
                    recursiveBestMove = i;// Thought this would just trace back with the best moves until the last one, then return i
                }
                v = result;
            }
        }
    }
    lookUpTable[state]=v*color;
    return v;
}

void playNim(int state) {
    int turn = 1;
    while (state > 1) {
        beginingState=state;
        recursiveBestMove=1;
        int action = negaMaxDecision(state, turn);
        printf("%d: %s takes %d\n", state,
               (turn == MAX ? "Max" : "Min"), recursiveBestMove);
        state = state - recursiveBestMove;
        turn = -1*turn;
    }
    printf("1: %s looses\n", (turn == MAX ? "Max" : "Min"));
}

int main(int argc, char *argv[]) {
    int state;
    if ((argc != 2) || (atoi(argv[1]) < 3)) {
        fprintf(stderr, "Usage: %s <number of sticks>, where ", argv[0]);
        fprintf(stderr, "<number of sticks> must be at least 3!\n");
        return -1;
    }

    state = (atoi(argv[1]));
    playNim(state);

    return 0;
}
