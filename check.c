#include <setjmp.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

#include "tetris.h"

jmp_buf buff;

void signalHandler(int signal) {
  if (signal == SIGSEGV) {
    siglongjmp(buff, 1);
  }
}

int main(int argc, char *argv[]) {
  signal(SIGSEGV, signalHandler);
  if (argc != 2) {
    printf("Invalid number of arguments.\n");
    return EXIT_FAILURE;
  }

  FILE *currentGameFile;

  currentGameFile = fopen(argv[1], "rb");
  TetrisGameState currentQuickSave;
  fread(&currentQuickSave, sizeof(TetrisGameState), 1, currentGameFile);

  extern void is_legitimate(TetrisGameState * game, int *is_legit);
  int results = 1;

  if (sigsetjmp(buff, 1) == 1) {
    printf("File is legitimate\n");
    return EXIT_SUCCESS;
  }
  is_legitimate(&currentQuickSave, &results);
  printf("File is not legitimate\n");
  return EXIT_SUCCESS;
}
