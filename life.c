#include <stdio.h>
#include <stdlib.h>

// prints out a board
void printBoard(int** board, int rowSize, int columnSize) {
	for(int i = 0; i < rowSize; i++) {
		for(int j = 0; j < columnSize; j++) {
			printf("%d ", board[i+1][j+1]);
		}
		printf("\n");
	}
}

// generates a board and initializes all elements to 0.
// Board is a 2D array with dimensions rowSize+2, columnSize+2
// (so there is a border, allowing simpler calculations);
void makeBoard(int*** board, int rowSize, int columnSize) {
	int rowSizeBordered = rowSize + 2;
	int columnSizeBordered = rowSize + 2;
	*board = malloc(sizeof(int*) * rowSizeBordered);
	for(int i = 0; i < rowSizeBordered; i++) {
		(*board)[i] = malloc(sizeof(int) * columnSizeBordered);
		for(int j = 0; j < columnSizeBordered; j++) {
			(*board)[i][j] = 0;
		}
	}
}

// Reads board of size rowSize, columnSize from file at filePath.
// Assumes a 2D-array of at least rowSize+2, columnSize+2,
// and reads the interior array of rowSize, columnSize
void readBoardFromFile(int*** boardIn, int rowSize, int columnSize, const char* filePath) {
	FILE* initialStateFile;
	char line[255];

	int** board = *boardIn;
	
	initialStateFile = fopen(filePath, "r");
	for(int i = 0; i < rowSize; i++) {
		fgets(line, 255, initialStateFile);
		for(int j = 0; j < columnSize; j++) {
			if(line[j] == '*')
				board[i+1][j+1] = 1;
		} 
	}
} 

void nextGeneration(int*** sourceBoardIn, int*** destBoardIn, int rowSize, int columnSize) {

	int** sourceBoard = *sourceBoardIn;
	int** destBoard = *destBoardIn;
	
	// check each area, minus the border which will always be 0
	for(int i = 1; i < rowSize+1; i++) {
		for(int j = 1; j < columnSize+1; j++) {
			// now count the surrounding 8 tiles
			int count = 0;
			for(int k = -1; k < 2; k++) {
				for(int l = -1; l < 2; l++) {
					if(sourceBoard[i+k][j+l] == 1 && !(k == 0 && l == 0))
						count++;
				}
			}
			// do something depending on count and living/dead:
			if(sourceBoard[i][j] == 0) {	
				// when dead, become alive if having exactly 3 neighbors
				if(count == 3) {
					destBoard[i][j] = 1;
				} else {
					destBoard[i][j] = 0;
				}
			} else {
				// when alive, only survive if not 2 or 3 neighbors
				if(count == 2 || count == 3) {
					destBoard[i][j] = 1;
				} else {
					destBoard[i][j] = 0;
				}
			}

		}
	}

}

// runs game, starting with sourceBoard, for a chosen amount of generations
void goForGenerations(int*** sourceBoard, int*** helperBoard, int rowSize, int columnSize, int generations) {
	int fromSource = 1;
	for(int i = 0; i < generations; i++) {
		if(fromSource) {
			nextGeneration(sourceBoard, helperBoard, rowSize, columnSize);
		} else {
			nextGeneration(helperBoard, sourceBoard, rowSize, columnSize);
		}
		fromSource = !fromSource;
	}
	// TODO: make goForGenerations take in only sourceBoard, and create the helper board itself.
	//       ensure that, if fromSource = 1 at the end, we copy from helperBoard to sourceBoard, because
	//	 sourceBoard is the only one that will be given by the user.
}

int main() {
	int rowSize = 10;
	int columnSize = 10;
	int** board1, **board2;

	makeBoard(&board1, rowSize, columnSize);
	makeBoard(&board2, rowSize, columnSize);

	// read board from file
	readBoardFromFile(&board1, rowSize, columnSize, "./board.txt");

	printf("initial:\n");	
	printBoard(board1, rowSize, columnSize);

	// start game for 1 generations
	int generations = 100;
	goForGenerations(&board1, &board2, rowSize, columnSize, 2);

	printf("after one generation:\n");
	printBoard(board1, rowSize, columnSize);

	return 0;
}

