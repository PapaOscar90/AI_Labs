#include <stdio.h>
#include <stdlib.h>

#define MAX 1
#define MIN -1

#define INFINITY 9999999

typedef struct State {
    int N;
    int K;
} State;

int minValue(int state); /* forward declaration: mutual recursion */

int maxValue(int state) {
    int move, max = -INFINITY;
    /* terminal state ? */
    if (state == 1) {
        return -1; /* Min wins if max is in a terminal state */
    }
    /* non-terminal state */
    for (move = 1; move <= 3; move++) {
        if (state - move > 0) { /* legal move */
            int m = minValue(state - move);
            if (m > max) max = m;
        }
    }
    return max;
}

int minValue(int state) {
    int move, min = INFINITY;
    /* terminal state ? */
    if (state == 1) {
        return 1; /* Max wins if min is in a terminal state */
    }
    /* non-terminal state */
    for (move = 1; move <= 3; move++) {
        if (state - move > 0) { /* legal move */
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
            if (state - move > 0) { /* legal move */
                int m = minValue(state - move);
                if (m > max) {
                    max = m;
                    bestmove = move;
                }
            }
        }
        return bestmove;
    }
    /* turn == MIN */
    min = INFINITY;
    for (move = 1; move <= 3; move++) {
        if (state - move > 0) { /* legal move */
            int m = maxValue(state - move);
            if (m < min) {
                min = m;
                bestmove = move;
            }
        }
    }
    return bestmove;
}

int negaMaxDecision(State state, int turn) {
    int result;

    if (state.N == 1) {
        return (-1 * turn);
    }

    int v = -INFINITY;
    for (int i = 3; i > 0; i--) {
        state.N = -1*(state.N - i);
        result = negaMaxDecision(state, turn++);
        if (v < result) {
            v = result;
        }
        state.N = state.N +i;
    }

    return v;
}

void playNim(State state) {
    int turn = 1;
    while (state.N != 1) {
        int action = negaMaxDecision(state, turn);
        printf("%d: %s takes %d\n", state,
               (turn == MAX ? "Max" : "Min"), action);
        state.N = state.N - action;
        turn = -1*turn;
    }
    printf("1: %s looses\n", (turn == MAX ? "Max" : "Min"));
}

int main(int argc, char *argv[]) {
    State state;
    if ((argc != 2) || (atoi(argv[1]) < 3)) {
        fprintf(stderr, "Usage: %s <number of sticks>, where ", argv[0]);
        fprintf(stderr, "<number of sticks> must be at least 3!\n");
        return -1;
    }

    state.N = (atoi(argv[1]));
    state.K = 0;
    playNim2(state);

    return 0;
}
