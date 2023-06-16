#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tetris.h"

// struct for holding score, lines, and filename
typedef struct {
  unsigned score;
  unsigned lines;
  char *fileName;
} tetrisGameInfo;

// compare function for score
int compareScore(const void *g1, const void *g2) {
  tetrisGameInfo *game1 = (tetrisGameInfo *)g1;
  tetrisGameInfo *game2 = (tetrisGameInfo *)g2;
  if ((game1->score) < (game2->score)) {
    return 1;
  } else if ((game1->score) > (game2->score)) {
    return -1;
  } else {
    if ((game1->lines) < (game2->lines)) {
      return 1;
    } else if ((game1->lines) > (game2->lines)) {
      return -1;
    }
  }
  return 0;
}

// compare function for lines
int compareLines(const void *g1, const void *g2) {
  tetrisGameInfo *game1 = (tetrisGameInfo *)g1;
  tetrisGameInfo *game2 = (tetrisGameInfo *)g2;
  if ((game1->lines) < (game2->lines)) {
    return 1;
  } else if ((game1->lines) > (game2->lines)) {
    return -1;

  } else {
    if ((game1->score) < (game2->score)) {
      return 1;
    } else if ((game1->score) > (game2->score)) {
      return -1;
    }
  }
  return 0;
}

// helper function for what to print
int printType(char *argv) {
  if (strcmp(argv, "score") == 0) {
    return 1;
  } else if (strcmp(argv, "lines") == 0) {
    return 2;
  } else {
    return 0;
  }
}

int main(int argc, char *argv[]) {
  if (argc != 3) {
    printf("Invalid number of command line arguments.\n");
    return EXIT_FAILURE;
  }
  char *line = (char *)malloc(sizeof(char) * 256);
  unsigned int size = 0;
  if (line == NULL) {
    printf("Malloc error.");
    return EXIT_FAILURE;
  }
  while ((fgets(line, 256, stdin)) != NULL) {
    size++;
  }
  if (size == 0) {
    printf("No files to sort.\n");
    return EXIT_FAILURE;
  }
  char **tetrisFileNamesPointer = (char **)malloc(sizeof(char *) * size);
  if (tetrisFileNamesPointer == NULL) {
    printf("Malloc error.");
    return EXIT_FAILURE;
  }
  for (int i = 0; i < size; i++) {
    tetrisFileNamesPointer[i] = (char *)malloc(sizeof(char) * 256);
  }
  rewind(stdin);
  free(line);
  for (int i = 0; i < size; i++) {
    fgets(tetrisFileNamesPointer[i], 256, stdin);
    char *ptr = strchr(tetrisFileNamesPointer[i], '\n');
    if (ptr) {
      *ptr = '\0';
    }
  }
  tetrisGameInfo *tetrisGameStructs;
  tetrisGameStructs = (tetrisGameInfo *)malloc((size) * sizeof(tetrisGameInfo));
  if (tetrisGameStructs == NULL) {
    printf("Malloc error.");
    return EXIT_FAILURE;
  }
  TetrisGameState currentGameState;
  FILE *currentFile;
  for (int i = 0; i < size; i++) { 
    currentFile = fopen(tetrisFileNamesPointer[i], "rb");
    fread(&currentGameState, sizeof(TetrisGameState), 1, currentFile);
    fclose(currentFile);
    tetrisGameInfo currentGame = {currentGameState.score,
                                  currentGameState.lines,
                                  tetrisFileNamesPointer[i]};
    tetrisGameStructs[i] = currentGame;
  }
  free(tetrisFileNamesPointer);
  if (printType(argv[1]) == 1) {
    qsort(tetrisGameStructs, size, sizeof(tetrisGameInfo), (compareScore));
    for (int i = 0; i < atoi(argv[2]); i++) {
      printf("%s\n", tetrisGameStructs[i].fileName);
    }
  } else if (printType(argv[1]) == 2) {
    qsort(tetrisGameStructs, size, sizeof(tetrisGameInfo), (compareLines));
    for (int i = 0; i < atoi(argv[2]); i++) {
      printf("%s\n", tetrisGameStructs[i].fileName);
    }
  } else {
    printf("Invalid rank argument.\n");
    return EXIT_FAILURE;
  }
  for (int i = 0; i < size; i++) {
    free(tetrisGameStructs[i].fileName);
  }
  free(tetrisGameStructs);
  return EXIT_SUCCESS;
}

