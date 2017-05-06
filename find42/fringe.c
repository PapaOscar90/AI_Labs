#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>

#include "fringe.h"

/* Returns an empty fringe.
 * The mode can be LIFO(=STACK), FIFO, or PRIO(=HEAP) */
Fringe makeFringe(int mode) {
    Fringe f;
    if ((mode != LIFO) && (mode != STACK) && (mode != FIFO) &&
        (mode != PRIO) && (mode != HEAP)) {
        fprintf(stderr, "makeFringe(mode=%d): incorrect mode. ", mode);
        fprintf(stderr, "(mode <- [LIFO,STACK,FIFO,PRIO,HEAP])\n");
        exit(EXIT_FAILURE);
    }
    f.mode = mode;
    f.size = f.front = f.rear = 0; /* front+rear only used in FIFO mode */
    f.head = 1;
    f.root = 1;  /* head+root only used in HEAP mode */
    f.states = malloc(MAXF * sizeof(State));
    if (f.states == NULL) {
        fprintf(stderr, "makeFringe(): memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }
    f.maxSize = f.insertCnt = f.deleteCnt = 0;
    return f;
}

void deallocFringe(Fringe fringe) {
    /* Frees the memory allocated for the fringe */
    free(fringe.states);
}

int getFringeSize(Fringe fringe) {
    /* Returns the number of elements in the fringe
     */
    return fringe.size;
}

int isEmptyFringe(Fringe fringe) {
    /* Returns 1 if the fringe is empty, otherwise 0 */
    return (fringe.size == 0 ? 1 : 0);
}

// Upheap function for the fringe in HEAP mode
void upHeap(Fringe *fringe, int index) {
    if (index == fringe->root) {
        return;
    }
    State current = fringe->states[index];
    State parent = fringe->states[index / 2];
    if (current.cost < parent.cost) {
        fringe->states[index] = parent;
        fringe->states[index / 2] = current;
    }
    upHeap(fringe, index / 2);
}

// Downheap function for the fringe in HEAP mode
void downHeap(Fringe *fringe, int index) {
    if (index >= fringe->head) {
        return;
    }
    if (index * 2 < fringe->head) {
        State current = fringe->states[index];
        State left = fringe->states[index * 2];
        if (left.cost < current.cost) {
            fringe->states[index] = left;
            fringe->states[index * 2] = current;
        }
        downHeap(fringe, index * 2);
    }
    if ((index * 2 + 1) < fringe->head) {
        State current = fringe->states[index];
        State right = fringe->states[index * 2 + 1];
        if (right.cost < current.cost) {
            fringe->states[index] = right;
            fringe->states[index * 2 + 1] = current;
        }
        downHeap(fringe, index * 2 + 1);
    }
}

/* Inserts s in the fringe, and returns the new fringe.
*/
Fringe insertFringe(Fringe fringe, State s) {
    if (fringe.size == MAXF) {
        fprintf(stderr, "insertFringe(..): fatal error, out of memory.\n");
        exit(EXIT_FAILURE);
    }
    fringe.insertCnt++;

    // Inserts the item based on the mode of the fringe
    switch (fringe.mode) {
        case LIFO: /* LIFO == STACK */
        case STACK:
            fringe.states[fringe.size] = s;
            break;
        case FIFO:
            fringe.states[fringe.rear++] = s;
            fringe.rear %= MAXF;
            break;
        case PRIO: /* PRIO == HEAP */
        case HEAP:
            fringe.states[fringe.head] = s;
            upHeap(&fringe, fringe.head);
            fringe.head++;
            break;
    }
    fringe.size++;
    if (fringe.size > fringe.maxSize) {
        fringe.maxSize = fringe.size;
    }
    return fringe;
}

/* Removes an element from the fringe, and returns it in s.
 * Moreover, the new fringe is returned. */
Fringe removeFringe(Fringe fringe, State *s) {
    if (fringe.size < 1) {
        fprintf(stderr, "removeFringe(..): fatal error, empty fringe.\n");
        exit(EXIT_FAILURE);
    }
    fringe.deleteCnt++;
    fringe.size--;
    switch (fringe.mode) {
        case LIFO: /* LIFO == STACK */
        case STACK:
            *s = fringe.states[fringe.size];
            break;
        case FIFO:
            *s = fringe.states[fringe.front++];
            fringe.front %= MAXF;
            break;
        case PRIO: /* PRIO == HEAP */
        case HEAP:
            *s = fringe.states[fringe.root];
            fringe.states[fringe.root] = fringe.states[fringe.head - 1];
            fringe.head--;
            downHeap(&fringe, fringe.root);
            break;
    }
    return fringe;
}

/* Shows fringe statistics */
void showStats(Fringe fringe) {
    printf("#### fringe statistics:\n");
    printf(" #size        : %7d\n", fringe.size);
    printf(" #maximum size: %7d\n", fringe.maxSize);
    printf(" #insertions  : %7d\n", fringe.insertCnt);
    printf(" #deletions   : %7d\n", fringe.deleteCnt);
    printf("####\n");
}
