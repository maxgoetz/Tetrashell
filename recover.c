// I, Max Goetz (730465187), pledge that I have neither given nor received unauthorized aid on this assignment.

#include <setjmp.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#include "tetris.h"

// Returns 1 if everything in the given game state looks OK, and 0 if not.
static int SanityCheckState(TetrisGameState *s) {
  int row, col, tmp;
  char c;
  tmp = s->location_x;
  if ((tmp < 0) || (tmp >= BLOCKS_WIDE)) return 0;
  tmp = s->location_y;
  if ((tmp < PIECE_START_Y) || (tmp >= BLOCKS_TALL)) return 0;

  // Important check: make sure the current piece is a valid piece ID.
  tmp = sizeof(tetris_pieces) / sizeof(const char *);
  if ((s->current_piece < 0) || (s->current_piece >= tmp)) return 0;
  if ((s->next_piece < 0) || (s->next_piece >= tmp)) return 0;

  // Make sure the board contains no invalid characters.
  for (row = 0; row < BLOCKS_TALL; row++) {
    for (col = 0; col < BLOCKS_WIDE; col++) {
      c = s->board[row * BLOCKS_WIDE + col];
      if ((c < ' ') || (c > '~')) return 0;
    }
  }
  return 1;
}

jmp_buf buff;

void signalHandler(int signal) {
  if (signal == SIGSEGV) {
    siglongjmp(buff, 1);
  }
}

int main(int argc, char *argv[]) {
  signal(SIGSEGV, signalHandler);
  if (argc != 2) {
    printf("Invalid number of command line arguments\n");
    return EXIT_FAILURE;
  }
  FILE *saveFiles;
  saveFiles = fopen(argv[1], "rb");
  if (saveFiles == NULL) {
    printf("Unable to open file");
    return EXIT_FAILURE;
  }

  const int fileSize = 4096;
  const int tetrisSaveSize = sizeof(TetrisGameState);
  int blocksSearched = 0;

  TetrisGameState currentQuickSave;
  extern void is_legitimate(TetrisGameState * game, int *is_legit);
  int results;

  char *directory = "recoveredTetrisSaves";
  mkdir(directory, S_IRWXU);

  int totalRecoveredValidGames = 1;
  char currentFileName[45];

  while (!feof(saveFiles)) {
    fseek(saveFiles, fileSize * blocksSearched++, SEEK_SET);
    fread(&currentQuickSave, tetrisSaveSize, 1, saveFiles);
    if (SanityCheckState(&currentQuickSave) != 1) {
    } else {
      if (sigsetjmp(buff, 1 == 1)) {
        sprintf(currentFileName, "recoveredTetrisSaves/recoveredFile%d.bin",
                totalRecoveredValidGames++);
        printf("%s\n", currentFileName);
        FILE *temp = fopen(currentFileName, "wb");
        fwrite(&currentQuickSave, sizeof(TetrisGameState), 1, temp);
        fclose(temp);
      } else {
        is_legitimate(&currentQuickSave, &results);
      }
    }
  }
  fclose(saveFiles);
  return EXIT_SUCCESS;
}
