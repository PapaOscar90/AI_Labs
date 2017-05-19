/* nqueens.c: (c) Arnold Meijster (a.meijster@rug.nl) */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define MAXQ 100
#define MAX_STATE_COUNT 10000
#define RUNLIMIT 100000

#define e 2.718281828

#define FALSE 0
#define TRUE  1

#define ABS(a) ((a) < 0 ? (-(a)) : (a))

int nqueens;      /* number of queens: global variable */
int queens[MAXQ]; /* queen at (r,c) is represented by queens[r] == c */


int queenPopulation[250][MAXQ];
int newQueenPopulation[250][MAXQ];
int fitnessOfPop[MAXQ];


void initializeRandomGenerator() {
    /* this routine initializes the random generator. You are not
     * supposed to understand this code. You can simply use it.
     */
    time_t t;
    srand((unsigned) time(&t));
}

/* Generate an initial position.
 * If flag == 0, then for each row, a queen is placed in the first column.
 * If flag == 1, then for each row, a queen is placed in a random column.
 */
void initiateQueens(int flag) {
    int q;
    for (q = 0; q < nqueens; q++) {
        queens[q] = (flag == 0 ? 0 : rand() % nqueens);
    }
}

void initiatePopulation(int pop) {
    for (int i = 0; i < pop; i++) {
        for (int j = 0; j < nqueens; j++) {
            queenPopulation[i][j] = rand() % nqueens;
            //printf("%d, ", queenPopulation[i][j]);
        }
        //printf("\n");
    }

}

/* returns TRUE if position (row0,column0) is in
 * conflict with (row1,column1), otherwise FALSE.
 */
int inConflict(int row0, int column0, int row1, int column1) {
    if (row0 == row1) return TRUE; /* on same row, */
    if (column0 == column1) return TRUE; /* column, */
    if (ABS(row0 - row1) == ABS(column0 - column1)) return TRUE;/* diagonal */
    return FALSE; /* no conflict */
}

/* returns TRUE if position (row,col) is in
 * conflict with any other queen on the board, otherwise FALSE.
 */
int inConflictWithAnotherQueen(int row, int col) {
    int queen;
    for (queen = 0; queen < nqueens; queen++) {
        if (inConflict(row, col, queen, queens[queen])) {
            if ((row != queen) || (col != queens[queen])) return TRUE;
        }
    }
    return FALSE;
}

/* print configuration on screen */
void printState() {
    int row, column;
    printf("\n");
    for (row = 0; row < nqueens; row++) {
        for (column = 0; column < nqueens; column++) {
            if (queens[row] != column) {
                printf(".");
            } else {
                if (inConflictWithAnotherQueen(row, column)) {
                    printf("Q");
                } else {
                    printf("q");
                }
            }
        }
        printf("\n");
    }
}

/* move queen on row q to specified column, i.e. to (q,column) */
void moveQueen(int queen, int column) {
    if ((queen < 0) || (queen >= nqueens)) {
        fprintf(stderr, "Error in moveQueen: queen=%d "
                "(should be 0<=queen<%d)...Abort.\n", queen, nqueens);
        exit(-1);
    }
    if ((column < 0) || (column >= nqueens)) {
        fprintf(stderr, "Error in moveQueen: column=%d "
                "(should be 0<=column<%d)...Abort.\n", column, nqueens);
        exit(-1);
    }
    queens[queen] = column;
}

/* returns TRUE if queen can be moved to position
 * (queen,column). Note that this routine checks only that
 * the values of queen and column are valid! It does not test
 * conflicts!
 */
int canMoveTo(int queen, int column) {
    if ((queen < 0) || (queen >= nqueens)) {
        fprintf(stderr, "Error in canMoveTo: queen=%d "
                "(should be 0<=queen<%d)...Abort.\n", queen, nqueens);
        exit(-1);
    }
    if (column < 0 || column >= nqueens) return FALSE;
    if (queens[queen] == column) return FALSE; /* queen already there */
    return TRUE;
}

/* returns the column number of the specified queen */
int columnOfQueen(int queen) {
    if ((queen < 0) || (queen >= nqueens)) {
        fprintf(stderr, "Error in columnOfQueen: queen=%d "
                "(should be 0<=queen<%d)...Abort.\n", queen, nqueens);
        exit(-1);
    }
    return queens[queen];
}

/* returns the number of pairs of queens that are in conflict */
int countConflicts() {
    int cnt = 0;
    int queen, other;
    for (queen = 0; queen < nqueens; queen++) {
        for (other = queen + 1; other < nqueens; other++) {
            if (inConflict(queen, queens[queen], other, queens[other])) {
                cnt++;
            }
        }
    }
    return cnt;
}

int countConflictsInChrome(int chrome) {
    int cnt = 0;
    int queen, other;
    for (queen = 0; queen < nqueens; queen++) {
        for (other = queen + 1; other < nqueens; other++) {
            if (inConflict(queen, queenPopulation[chrome][queen], other, queenPopulation[chrome][other])) {
                cnt++;
            }
        }
    }
    return cnt;
}

/* evaluation function. The maximal number of queens in conflict
 * can be 1 + 2 + 3 + 4 + .. + (nquees-1)=(nqueens-1)*nqueens/2.
 * Since we want to do ascending local searches, the evaluation
 * function returns (nqueens-1)*nqueens/2 - countConflicts().
 */
int evaluateState() {
    return (nqueens - 1) * nqueens / 2 - countConflicts();
}

/******************RANDOM SEARCH*******************************************/

/* A very silly random search 'algorithm' */
#define MAXITER 10000

void randomSearch() {
    int queen, iter = 0;
    int optimum = (nqueens - 1) * nqueens / 2;

    while (evaluateState() != optimum) {
        printf("iteration %d: evaluation=%d\n", iter++, evaluateState());
        if (iter == MAXITER) break;  /* give up */
        /* generate a (new) random state: for each queen do ...*/
        for (queen = 0; queen < nqueens; queen++) {
            int pos, newpos;
            /* position (=column) of queen */
            pos = columnOfQueen(queen);
            /* change in random new location */
            newpos = pos;
            while (newpos == pos) {
                newpos = rand() % nqueens;
            }
            moveQueen(queen, newpos);
        }
    }
    if (iter < MAXITER) {
        printf("Solved puzzle. ");
    }
    printf("Final state is");
    printState();
}

/*********************HILLCLIMBING SEARCH****************************************/

 //This is a list that stores the best local states currently found (equal score)
int nextBestStates[MAX_STATE_COUNT][MAXQ];

void copyToStates(int stateIndex){
	for (int i = 0; i < nqueens; i++){
		nextBestStates[stateIndex][i] = queens[i];
	}
}

void copyFromStates(int stateIndex){
	for (int i = 0; i < nqueens; i++){
		queens[i] = nextBestStates[stateIndex][i];
	}
}

int successes = 0; //Used for testing purposes

void hillClimbing() {
	int optimum = (nqueens - 1) * nqueens / 2;
	int bestH = evaluateState();  
	int stateIndex = 0;
	int successorFound = 0;
	int iter = 0;
	int maxSteps = 1000;
	while (evaluateState() != optimum && iter != maxSteps){
		//printf("iteration %d: evaluation=%d\n", iter++, evaluateState());
		iter++;
		stateIndex = 0;
		successorFound = 0;
		for (int queen = 0; queen < nqueens; queen++){
			//Save the original position of the queen, so it can be replaced
			//in the next queen iteration
			int originalColumn = queens[queen];
			for (int column = 0; column < nqueens; column++){
				if (canMoveTo(queen, column)){
					moveQueen(queen, column);
					if (evaluateState() == bestH){
						successorFound = 1;
						copyToStates(stateIndex);
						stateIndex++;
					} else if (evaluateState() > bestH){
						successorFound = 1;
						bestH = evaluateState();
						//Reset the state list to accomodate new best choices
						stateIndex = 0;
						copyToStates(stateIndex);
						stateIndex++;
					}
				}
			}
			queens[queen] = originalColumn;
		}
		copyFromStates(rand() % stateIndex);
		if (!successorFound){
			break;
		}
	}
    if (successorFound && iter != maxSteps) {
        printf("Solved puzzle. ");
       // successes++;
    }
    printf("Final state is");
    printState();
}

/*************************************************************/

float randomFloat(){
      float r = (float)rand()/(float)RAND_MAX;
      return r;
}

double timeToTemperature(int t, double startingTemp){
	if (t > 10000000){
		return 0;
	}
	return startingTemp/pow(t, 0.5);
}

void simulatedAnnealing(double startingTemp) {
    int optimum = (nqueens - 1) * nqueens / 2;
    int previousH = evaluateState();
    int currentH;
    int t = 1;
    float temp;
    while(evaluateState() != optimum){
		temp = timeToTemperature(t, startingTemp);
		if (temp == 0){
			break;
		}
		int queen = rand() % nqueens;
		int column = rand() % nqueens;
		while (!canMoveTo(queen, column)){
			//Make sure there's no queen in this spot
			queen = rand() % nqueens;
			column = rand() % nqueens;
		}
		moveQueen(queen, column);
		currentH = evaluateState();
		int E = currentH - previousH;
		double rngFloat = randomFloat();
		if (E > 0){
			previousH = currentH;
			//printf("Accepted\n");
		} else if (rngFloat < pow(e, E/temp)){
			//printf("%d, %f, %d, %f\n", currentH, temp, E, pow(e, E/temp));
			previousH = currentH;
			//printf("Accepted\n");
		} else {
			queens[queen] = column;
		}
		t++;
	}
	if (evaluateState() == optimum){
		printf("\nSolved puzzle. \n");
	}
	printf("Final state is\n");
	printf("%d", evaluateState());
    printState();
}
/*************************************************************/

void updateFitness(int pop) {
    for (int i = 0; i < pop; i++) {
        fitnessOfPop[i] = countConflictsInChrome(i);
    }
}

void printAnswer(int index, int run){
    printf("\nFOUND ON RUN %d!\n", run);
    for (int i = 0; i < nqueens; i++) {
        printf("%d, ", queenPopulation[index][i]);
    }
}

// true if person fits goal
int isFound(int run) {
    int isFound = 0, index = 0;
    for (int i = 0; i < nqueens; i++) {
        if (fitnessOfPop[i] == 0) {
            isFound = 1;
            index = i;
        }
    }
    if (isFound == 1) {
        printAnswer(index, run);
        return 1;
    }
    return 0;
}

// Finds a possible chromosome for mating by chance porportional to its fintness. Smaller = better chance
int getNextMate(int sizeofpop, int lastSelected) {
    int index = nqueens-1;
    int max = 0;
    int i=0;

    // Continue until it finds one to return
    while(TRUE){
        int chance = rand() % 100;
        if (((nqueens*(nqueens-1))/fitnessOfPop[i]) >= chance && i != lastSelected) {
            max = fitnessOfPop[i];
            index = i;
            return index;
        }
        i = (i+1)%nqueens;
    }
}

// Reproduces two chromosomes at a random splice point
void reproduce(int *X, int *Y, int popNumber) {
    int position = rand() % nqueens;
    int child[nqueens];
    for (int i = 0; i < position; i++) {
        child[i] = X[i];
    }
    for (int i = position; i < nqueens; i++) {
        child[i] = Y[i];
    }
    for (int i = 0; i < nqueens; i++) {
        newQueenPopulation[popNumber][i] = child[i];
    }
}

// Updates the population with the new population. Directly copies and resets new to 0's
void updatePopulation(int sizeofpop) {
    for (int i = 0; i < sizeofpop; i++) {
        for (int j = 0; j < nqueens; j++) {
            queenPopulation[i][j] = newQueenPopulation[i][j];
            newQueenPopulation[i][j] = 0;
        }
    }
}


// Creates random mutations
void mutate(int sizeofpop){
    int probability = 1;
    int chance;

    for (int i=0; i<sizeofpop; i++){
        for (int j=0; j<nqueens; j++){
            chance = rand()%100;
            if(chance<=probability){
                queenPopulation[i][j]=rand()%nqueens;
            }
        }
    }
}

// The main genetic algorithm
void geneticAlgorithm(int sizeOfPop) {
    int X[nqueens];
    int Y[nqueens];

    // For each person, find two chromosome with fitness function, then reproduce into new population
    for (int i = 0; i < sizeOfPop; i++) {
        int Xrand = getNextMate(sizeOfPop,-1);
        int Yrand = getNextMate(sizeOfPop,Xrand);

        for (int j = 0; j < nqueens; j++) {
            X[j] = queenPopulation[Xrand][j];
            Y[j] = queenPopulation[Yrand][j];
        }
        reproduce(X, Y, i);
    }

    // Update the population with new population and reset new back to 0's
    updatePopulation(sizeOfPop);
    mutate(sizeOfPop); // Cause random mutation(s) in each person
    updateFitness(nqueens); // Update fitness for next round
}

void geneticHelper() {
    // make an even population so everybody can mate
    int numPopulation = 25+(0.1*nqueens);
    initiatePopulation(numPopulation); //Makes chromosomes random
    updateFitness(nqueens); // Update fitness for initial check
    int run = 0;

    // The main algorithm loop. Continues until found, our max run limit
    while (isFound(run) == 0 && run < RUNLIMIT) {
        geneticAlgorithm(numPopulation);
        run++;
    };
    if(run == RUNLIMIT)
        printf("Did Not Find - Run Limit!");
}

//Used to test frequency of successful solves
void hillClimbingSuccessRateTester(int tests){
	for (int i = 0; i < tests; i++){
		hillClimbing();
		initiateQueens(1);
	}
	printf("%d\n", successes);
}
	

int main(int argc, char *argv[]) {
    int algorithm;
    srand(time(NULL));
    do {
        printf("Number of queens (1<=nqueens<%d): ", MAXQ);
        scanf("%d", &nqueens);
    } while ((nqueens < 1) || (nqueens > MAXQ));

    do {
        printf("Algorithm: (1) Random search  (2) Hill climbing  ");
        printf("(3) Simulated Annealing  (4) Genetic Algorithm: ");
        scanf("%d", &algorithm);
    } while ((algorithm < 1) || (algorithm > 4));

    if (algorithm == 1 || algorithm == 2 || algorithm == 3) {
        initializeRandomGenerator();
        initiateQueens(1);
        printf("\nInitial state:");
        printState();
    }

    switch (algorithm) {
        case 1:
            randomSearch();
            break;
        case 2:
			hillClimbing();
            break;
        case 3:
			printf("Pick the starting temperature: ");
			double startingTemp;
			scanf("%lf", &startingTemp);
            simulatedAnnealing(startingTemp);
            break;
        case 4:
            geneticHelper();
            break;
    }

    return 0;
}
