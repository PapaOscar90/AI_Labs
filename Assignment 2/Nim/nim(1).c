#include <stdio.h>
#include <stdlib.h>

#define MAX 1
#define MIN -1
#define INFINITY 9999999

int recursiveBestMove=3;
int lookUpTable[100];

int negaMaxDecision(int state, int color) {
    int result;

    if(lookUpTable[state] != 0){
        return (lookUpTable[state]);
    }

    if (state == 1) {
        lookUpTable[state]=-1;
        return (-1 * color);
    }

    int v = -INFINITY;
    for (int i = 1; i <4; i++) {
        if (state - i >0) {
            result = -1 * negaMaxDecision(state - i, -1 * color);
            if (v <= result) {
                v = result;
                recursiveBestMove = i;// Thought this would just trace back with the best moves until the last one, then return i
            }
        }
    }
    lookUpTable[state]=v;
    return v;
}

void playNim(int state) {
    int turn = 1;
    while (state > 1) {
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
