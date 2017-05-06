//
// Created by PhilO on 05-May-17.
//

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

// The possible actions, TODO: This can be reduced to only moving in directions towards the goal
int actions[8][2] = {  /* knight moves */
        {-2, -1}, {-2, 1}, {-1, -2}, {-1, 2}, {1,-2}, {1,2}, {2, -1}, {2, 1}
};
unsigned long statesVisited = 0;

// The tiles of the board
typedef struct Tile{
    int x,y;
    int state;
    struct Tile camefrom;
    int costRemaining;
}Tile;

// Creates a new node with the passed in arguments and returns it
Tile createNewTile(int x, int y, int state, Tile cameFrom, int remainingDistance, int startCost){
    Tile n;
    n.x=x;
    n.y=y;
    n.state=state;
    n.camefrom=cameFrom;
    n.costRemaining= remainingDistance;
    return n;
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
    int flag=0;
    List startOfOldList = li;

    List newList = malloc(sizeof(struct ListNode));
    assert(newList != NULL);
    newList->currentPosition = position;

    if (position.costRemaining < li->currentPosition.costRemaining){
        newList->next = li;
        return newList;
    }else {
        while (position.costRemaining >= (li->next)->currentPosition.costRemaining) {
            li = li->next;
        }
    }

    newList->next = li->next;
    li->next = newList;

    return startOfOldList;
}

Tile popSmallest(List li){
    Tile returnPosition = li->currentPosition;
    li = li->next;

    return returnPosition;
}

int main(int argc, char* argv[]){
    printf("Hello world.");

    return 0;
}