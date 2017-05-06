//
// Created by PhilO on 05-May-17.
//

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#define N 500

// The possible actions, TODO: This can be reduced to only moving in directions towards the goal
int actions[8][2] = {  /* knight moves */
        {-2, -1}, {-2, 1}, {-1, -2}, {-1, 2}, {1,-2}, {1,2}, {2, -1}, {2, 1}
};
unsigned long statesVisited = 0;



// The tiles of the board
typedef struct Tile{
    int x,y;
    int state;
    double costRemaining;
    int pathLength;
}Tile;


// Creates a new node with the passed in arguments and returns it
Tile createNewTile(int x, int y, int state, double remainingDistance, int pathLength){
    Tile n;
    n.x=x;
    n.y=y;
    n.state=state;
    n.costRemaining= remainingDistance;
    n.pathLength = pathLength;
    return n;
}

Tile** initialize(Tile** board) {
    board = malloc(sizeof(Tile*)*N);
    for (int i=0; i<N; i++){
        board[i] = malloc(sizeof(Tile)*N);
        for (int j=0; j < N; j++){
            board[i][j] = createNewTile(j,i, 0,0, 999999);
        }
    }
    return board;
}

// The list node
typedef struct ListNode{
    Tile currentPosition;
    struct ListNode* next;
}* List;

List newEmptyList(){
    return NULL;
}

// Adds a Tile to the list in a sorted manner from least to greatest
List addSorted(Tile position, List li){
    if (li == NULL){
        List newList = malloc(sizeof(struct ListNode));
        assert(newList != NULL);
        newList->currentPosition=position;
        newList->next=newEmptyList();
        return newList;
    }

    List startOfOldList = li;

    List newList = malloc(sizeof(struct ListNode));
    assert(newList != NULL);
    newList->currentPosition = position;

    if (position.costRemaining < li->currentPosition.costRemaining){
        newList->next = li;
        return newList;
    }else {
        if (li->next == NULL) {
            newList->next = newEmptyList();
            li->next = newList;
            return startOfOldList;
        }
        while (position.costRemaining >= (li->next)->currentPosition.costRemaining) {
            li = li->next;
            if (li->next == NULL) {
                newList->next = newEmptyList();
                li->next = newList;
                return startOfOldList;
            }
        }
    }

    newList->next = li->next;
    li->next = newList;

    return startOfOldList;
}

Tile popSmallest(List* li){
    Tile returnPosition = (*li)->currentPosition;
    *li = (*li)->next;

    return returnPosition;
}

int goalTest(Tile tile, int testX, int testY){
    return tile.x == testX && tile.y == testY ? 1 : 0;
}

int isValidLocation(int x, int y) {
    return (0<=x && x < N && 0<= y && y < N);
}

void freeBoard(Tile ** board){
    for (int i=0; i<N; i++){
        free(board[i]);
    }
    free(board);
}

void freeList(List li){
    if(li == NULL) {
        return;
    }
    freeList(li->next);
    free(li);
}

double effectiveBranchingFactor(unsigned long states, int d) {
    /* approximates such that N=\Sum+{i=1}^{d} b^i */
    double lwb = 1;
    double upb = pow(states, 1.0/d);
    while (upb - lwb > 0.000001) {
        double mid = (lwb + upb) / 2.0;
        /* the following test makes use of the geometric series */
        if (mid*(1-pow(mid, d))/(1-mid) <= states) {
            lwb = mid;
        } else {
            upb = mid;
        }
    }
    return lwb;
}

void manhattanDist(int x, int y, int goalX, int goalY){
    Tile** board = initialize(board);
    List frontier = newEmptyList();
    int statesVisited=0;

    board[0][0].pathLength=0;
    frontier = addSorted(board[y][x], frontier);
    Tile current;
    Tile* actionTile;


    while (1 == 1){
        if (frontier == NULL){
            printf("No path found.");
            freeBoard(board);
            return;
        }

        current = popSmallest(&frontier);
        statesVisited++;
        if (goalTest(current, goalY, goalX)){
            printf("Manhattan:\n");
            printf("Path Length: %d\n", current.pathLength);
            printf("States Visited: %d\n", statesVisited);
            printf("Branch Factor: %f\n \n", effectiveBranchingFactor(statesVisited, current.pathLength));
            freeBoard(board);
            freeList(frontier);
            return;
        }
        board[current.y][current.x].state = 1;


        for(int i=0; i<8; i++){
            if (isValidLocation((current.y)+(actions[i][1]),(current.x)+(actions[i][0]))) {
                actionTile = board[(current.y) + (actions[i][1])] + (current.x) + (actions[i][0]);
                if(current.pathLength + 1 < actionTile->pathLength) {
                    actionTile->pathLength = current.pathLength + 1;
                    actionTile->costRemaining = (((abs(goalX-actionTile->x) + abs(goalY-actionTile->y))/3.0) + actionTile->pathLength);
                    frontier = addSorted(*actionTile, frontier);
                }
            }
        }

    }
}



void pythag(int x, int y, int goalX, int goalY){
    Tile** board = initialize(board);
    List frontier = newEmptyList();
    unsigned long statesVisited2=0;

    board[0][0].pathLength=0;
    frontier = addSorted(board[y][x], frontier);
    Tile current;
    Tile* actionTile;


    while (1 == 1){
        if (frontier == NULL){
            printf("No path found.");
            freeBoard(board);
            return;
        }

        current = popSmallest(&frontier);
        statesVisited2++;
        if (goalTest(current, goalX, goalY)){
            printf("Pythag: \n");
            printf("Path Length: %d\n", current.pathLength);
            printf("States Visited: %d\n", statesVisited2);
            printf("Branch Factor: %f\n \n", effectiveBranchingFactor(statesVisited2, current.pathLength));
            freeBoard(board);
            freeList(frontier);
            return;
        }
        board[current.y][current.x].state = 1;


        for(int i=0; i<8; i++){
            if (isValidLocation((current.y)+(actions[i][1]),(current.x)+(actions[i][0]))) {
                actionTile = board[(current.y) + (actions[i][1])] + (current.x) + (actions[i][0]);
                if(current.pathLength + 1 < actionTile->pathLength) {
                    actionTile->pathLength = current.pathLength + 1;
                    actionTile->costRemaining = (sqrt((goalX-actionTile->x)*(goalX-actionTile->x) + (goalY-actionTile->y)*(goalY-actionTile->y))/sqrt(5) + actionTile->pathLength);
                    frontier = addSorted(*actionTile, frontier);
                }
            }
        }

    }
}

int main(int argc, char* argv[]){
    int x,y,goalX,goalY;
    printf("Enter x y goalx goaly:");
    scanf("%d %d %d %d", &x, &y, &goalX, &goalY);

    manhattanDist(x,y,goalX,goalY);
    pythag(x,y,goalX,goalY);


    return 0;
}