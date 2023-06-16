#include <stdio.h>
#include "tetris.h"
#include <stdlib.h>
#include <string.h>
#include <limits.h>


int main(int argc, char *argv[]) {

	FILE * tetrisSaveRead;
	tetrisSaveRead = fopen("tetris_quicksave.bin", "rb");
	
	TetrisGameState customGameState;
	
	fread(&customGameState, sizeof(customGameState), 1, tetrisSaveRead);

	if (argc != 3) {
		printf("Invalid number of arguments.\n");
		return 1;
	}
	if (atol(argv[2]) > UINT_MAX || atol(argv[2]) < 0) {
		printf("Invalid 3rd argument, argument must be an unsigned int.\n");
		return 1;
	}

	if (strcmp(argv[1], "score") == 0) {
		customGameState.score = atol(argv[2]);
	} else if (strcmp(argv[1], "lines") == 0) {
		customGameState.lines = atol(argv[2]);	
	} else if (strcmp(argv[1], "next_piece") == 0) {
		if ((atol(argv[2])) <= 18) {
			customGameState.next_piece = atol(argv[2]);
		} else {
			printf("Invalid 3rd argument, next_piece input must be less than 19.\n");
			return 1;
		}
	} else {
		printf("Invalid 2nd argument, options are lines, score, and next_piece\n");
	}

	fclose(tetrisSaveRead);

	FILE * tetrisSaveWrite;
	tetrisSaveWrite = fopen("tetris_quicksave.bin", "wb");

	fwrite(&customGameState, sizeof(customGameState), 1, tetrisSaveWrite);

	fclose(tetrisSaveWrite);

	return 0;
}
