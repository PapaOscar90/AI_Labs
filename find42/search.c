#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "state.h"
#include "fringe.h"

#define RANGE 1000000
#define MAXV 500000 // Max Visited List size


int find(State *array, int size, int element){
	for (int i = 0; i < size; i++){
		if (array[i].value == element){
			return 1;
		}
	}
	return 0;
}

Fringe insertValidSucc(Fringe fringe, int value, State prevState, int cost, State **visitedStates, int *visited) {
  State s;
  if ((value < 0) || (value > RANGE) || find(*visitedStates, *visited, value)) {
    /* ignore states that are out of bounds, or
     * are already visited */
    return fringe;
  }
  s.value = value;
  s.prevValue = prevState.value;
  s.cost = prevState.cost + cost;
  s.length = prevState.length + 1;
  (*visitedStates)[(*visited)] = s;
  (*visited)++;
  return insertFringe(fringe, s);
}

void printFormattedPath(int length, int cost, int goal, int *path, int pathSize){
	for (int i = pathSize-1; i >= 0; i--) {
		printf("%d ", path[i]);
		if (path[i] != goal){
			int diff = path[i-1] - path[i];
			int mul = path[i-1]/path[i];
			int div = path[i]/path[i-1];
			printf("(");
			if (diff == 1){
				printf("+1");
			} else if (diff == -1){
				printf("-1");
			} else if (mul == 2){
				printf("*2");
			} else if (mul == 3){
				printf("*3");
			} else if (div == 2){
				printf("/2");
			} else if (div == 3){
				printf("/3");
			}
			printf(")-> ");
		}
	}
	printf("\n");
	printf("length = %d, cost = %d\n", length, cost);
}
// An algorithm to find a path, using the state's current value, previous
// value, and the visited states, then prints the path
void makePath(int start, State state, State *visitedStates, int visitedStatesSize) {
	int path[10000];
	int pathSize = 0;
	int current = state.value;
	int goal = state.value;
	int length = state.length;
	int cost = state.cost;
	path[0] = state.value;
	pathSize++;
	while (current != start) {
		for (int i = visitedStatesSize - 1; i > 0; i--) {
			if (visitedStates[i].value == state.prevValue) {
				path[pathSize] = state.prevValue;
				state = visitedStates[i];
				pathSize++;
			}
		}
		current = state.value;
		break;
	}
	path[pathSize] = start;
	pathSize++;
	printFormattedPath(length, cost, goal, path, pathSize);
}

void search(int mode, int start, int goal) {
  Fringe fringe;
  State state;
  int goalReached = 0;
  int visited = 0;
  int visitedStatesSize = 0;
  State *visitedStates = malloc(MAXV*sizeof(State));
  int value;

  fringe = makeFringe(mode);
  state.value = start;
  state.prevValue = -1;
  state.cost = 0;
  state.length = -1;  //insertValidSucc increments this to 0 when it inserts this state
  fringe = insertValidSucc(fringe, state.value, state, 0,  &visitedStates, &visitedStatesSize);
  while (!isEmptyFringe(fringe)) {
    /* get a state from the fringe */
    fringe = removeFringe(fringe, &state);
    /* is state the goal? */
    value = state.value;
    visited++;
    if (value == goal) {
      goalReached = 1;
      break;
    }
    /* insert neighbouring states */
    fringe = insertValidSucc(fringe, value+1, state, 1, &visitedStates, &visitedStatesSize); /* rule n->n + 1      */
    fringe = insertValidSucc(fringe, 2*value, state, 2, &visitedStates, &visitedStatesSize); /* rule n->2*n        */
    fringe = insertValidSucc(fringe, 3*value, state, 2, &visitedStates, &visitedStatesSize); /* rule n->3*n        */
    fringe = insertValidSucc(fringe, value-1, state, 1, &visitedStates, &visitedStatesSize); /* rule n->n - 1      */
    fringe = insertValidSucc(fringe, value/2, state, 3, &visitedStates, &visitedStatesSize); /* rule n->floor(n/2) */
    fringe = insertValidSucc(fringe, value/3, state, 3, &visitedStates, &visitedStatesSize); /* rule n->floor(n/3) */
  }
  if (goalReached == 0) {
    printf("goal not reachable ");
  } else {
    printf("goal reached \n");
     makePath(start, state, visitedStates, visitedStatesSize);
  }
  printf("(%d nodes visited)\n", visited);
  showStats(fringe);
  deallocFringe(fringe);
  free(visitedStates);
}

int main(int argc, char *argv[]) {
  int start, goal, fringetype;
  if ((argc == 1) || (argc > 4)) {
    fprintf(stderr, "Usage: %s <STACK|FIFO|HEAP> [start] [goal]\n", argv[0]);
    return EXIT_FAILURE;
  }
  fringetype = 0;
  
  if ((strcmp(argv[1], "STACK") == 0) || (strcmp(argv[1], "LIFO") == 0)) {
    fringetype = STACK;
  } else if (strcmp(argv[1], "FIFO") == 0) {
    fringetype = FIFO;
  } else if ((strcmp(argv[1], "HEAP") == 0) || (strcmp(argv[1], "PRIO") == 0)) {
    fringetype = HEAP;
  }
  if (fringetype == 0) {
    fprintf(stderr, "Usage: %s <STACK|FIFO|HEAP> [start] [goal]\n", argv[0]);
    return EXIT_FAILURE;
  }

  start = 0;
  goal = 42;
  if (argc == 3) {
    goal = atoi(argv[2]);
  } else if (argc == 4) {
    start = atoi(argv[2]);
    goal = atoi(argv[3]);
  }

  printf("Problem: route from %d to %d\n", start, goal);
  search(fringetype, start, goal); 
  return EXIT_SUCCESS;
}
