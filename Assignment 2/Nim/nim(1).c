#include <stdio.h>
#include <stdlib.h>

#define MAX 1
#define MIN -1

#define INFINITY 9999999

int recursiveBestMove;

// TODO: Fix Everything. Crashes on run. Must be an infinite loop or trying to do something it shouldn't. I think maybe going below 1.

// The following functions are from the OLD minimax algorithm that worked. Must implement these into a single recursive
// NEGAMAX function.
/*
int minValue(int state);

int maxValue(int state) {
    int move, max = -INFINITY;
    // terminal state ?
    if (state == 1) {
        return -1; // Min wins if max is in a terminal state
    }
    // non-terminal state
    for (move = 1; move <= 3; move++) {
        if (state - move > 0) { // legal move
            int m = minValue(state - move);
            if (m > max) max = m;
        }
    }
    return max;
}

int minValue(int state) {
    int move, min = INFINITY;
    // terminal state ?
    if (state == 1) {
        return 1; // Max wins if min is in a terminal state
    }
    // non-terminal state
    for (move = 1; move <= 3; move++) {
        if (state - move > 0) { // legal move
            int m = maxValue(state - move);
            if (m < min) min = m;
        }
    }
    return min;
}

int minimaxDecision(int state, int turn) {
    int move, bestmove, max, min;
    if (turn == MAX) {
        max = -INFINITY;
        for (move = 1; move <= 3; move++) {
            if (state - move > 0) { // legal move
                int m = minValue(state - move);
                if (m > max) {
                    max = m;
                    bestmove = move;
                }
            }
        }
        return bestmove;
    }
    // turn == MIN
    min = INFINITY;
    for (move = 1; move <= 3; move++) {
        if (state - move > 0) { // legal move
            int m = maxValue(state - move);
            if (m < min) {
                min = m;
                bestmove = move;
            }
        }
    }
    return bestmove;
}
*/


int negaMaxDecision(int state, int color) {
    int result;

    if (state == 1) {
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

    return v;
}

void playNim(int state) {
    int turn = 1;
    while (state != 1) {
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
