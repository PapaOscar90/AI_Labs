#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define N 500   /* N times N chessboard */

int actions[8][2] = {  /* knight moves */
  {-2, -1}, {-2, 1}, {-1, -2}, {-1, 2}, {1,-2}, {1,2}, {2, -1}, {2, 1}
};
int costShortestPath[N][N];
unsigned long statesVisited = 0;

void doubleQSize(struct Queue q);

typedef struct position{
    int x,y;
    int visited;
    struct position *camefrom;
    int costRemaining;
}position;

typedef struct Queue{
    position *array;
    int back;
    int front;
    int size;
}Queue;

Queue newQueue(int s){
    Queue q;
    q.array=malloc(s*sizeof(position));
    assert(q.array != NULL);
    q.back = 0;
    q.front = 0;
    q.size = s;
    return
}

void enqueue(Queue q, position p){
    q.back +=1;
    if(q.back >= q.size){
        doubleQSize(q);
    }
    q.array[q.back] = p;
    q.size++;
}

position dequeue(Queue q){
    q.front += 1;
    q.size--;
    return q.array[q.front-1];
}

position dequeueLeast(Queue q){
    int min=99999999;
    int minIndex;
    for(int i = 0; i<q.size; i++){
        if(q.array[i]<min){
            minIndex = i;
            min = q.array[i];
        }
    }
    position returnPosition = q.array[minIndex];

    for (int i = minIndex+1; i<q.size; i++){
        i>0 ? (q.array[i - 1] = q.array[i]) : (q.array[q.front] += 1);
    }
    q.size--;
    return returnPosition;
}

void doubleQSize(Queue q) {
    q.array=realloc((q.size*2), sizeof(position));
    assert(q.array != NULL);
    q.size *=2;
}

int isValidLocation(int x, int y) {
  return (0<=x && x < N && 0<= y && y < N);
}

void initialize() {
  int r, c;
  for (r=0; r < N; r++) {
    for (c=0; c < N; c++) {
      costShortestPath[r][c] = 999999; /* represents infinity */
    }
  }
}


int knightDLS(int cost, int limit, int row, int column, int rowGoal, int columnGoal) {
  int act;
  statesVisited++;
  if (row == rowGoal && column == columnGoal) {
    return 1;  /* goal reached */
  }
  if (cost == limit || cost >= costShortestPath[row][column]) {
    return 0; /* limit reached, or we've been here before via a 'cheaper' path */
  }
  costShortestPath[row][column] = cost;
  for (act=0; act < 8; act++) {
    int r = row + actions[act][0];
    int c = column + actions[act][1];
    if (isValidLocation(r, c) && knightDLS(cost+1, limit, r, c, rowGoal, columnGoal) == 1) {
      return 1;
    }
  }
  return 0;
}

int knightIDS(int row, int column, int rowGoal, int columnGoal) {
  int limit = 0;
  printf ("limit=0"); fflush(stdout);
  initialize();
  while (knightDLS(0, limit, row, column, rowGoal, columnGoal) == 0) {
    initialize();
    limit++;
    printf(",%d", limit); fflush(stdout);
  }
  printf("\n");
  return limit;
}

int aStar(int row, int column, int rowGoal, int columnGoal, int goalDistance){
    Queue checkedList = newQueue(0);
    Queue toCheckList = newQueue(0);
    position start;
    start.x = row;
    start.y = column;
    start.camefrom = NULL;
    start.visited=0;
    start.costRemaining = (rowGoal-row) + (columnGoal-column);

    enqueue(toCheckList, start);
    position currentPosition;
    position child;
    while (Queue.size >= 0) {
        if (Queue.size == 0){
            return 0;
        }
        currentPosition = dequeueLeast(toCheckList);
        if (currentPosition.x == rowGoal && currentPosition.y == columnGoal){
            printTheSolution(currentPosition);
        }
        currentPosition.visited=1;
        for(int i=0; i<8; i++){
            child.x = currentPosition.x + actions[i][0];
            child.y = currentPosition.y + actions[i][1];
            if (isValidLocation(child.x, child.y) && child.visited==0){
                enqueue(toCheckList, child);
            }
        }
    }

}

void printTheSolution(position p){
    while (p->camefrom != NULL){
        printf("(%d,%d)-> ", p.x, p.y);
    }
    printf("Start");
}

int main(int argc, char *argv[]) {
  int x0,y0, x1,y1;
  do {
    printf("Start location (x,y) = "); fflush(stdout);
    scanf("%d %d", &x0, &y0);
  } while (!isValidLocation(x0,y0));
  do {
    printf("Goal location (x,y)  = "); fflush(stdout);
    scanf("%d %d", &x1, &y1);
  } while (!isValidLocation(x1,y1));

  printf("Length shortest path: %d\n", knightIDS(x0,y0, x1,y1));
  printf("#visited states: %lu\n", statesVisited);
  return 0;
}
