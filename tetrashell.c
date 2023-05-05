#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#include "tetris.h"

void printTetrashell(char *currentGame)
{
    char *username = getlogin();
    TetrisGameState game;
    FILE *currentFile = fopen(currentGame, "rb");
    if (currentFile == NULL) {
	printf("Invalid game file path.");
    }
    fread(&game, sizeof(TetrisGameState), 1, currentFile);
    int score = game.score;
    int lines = game.lines;
    fclose(currentFile);

    printf("\033[38;2;0;255;255m%s@TShell[score: %d][lines: %d]> \033[0m",
	   username, score, lines);
}

int validateFile(char *filePath)
{
    FILE *fp;
    fp = fopen(filePath, "r");
    while (fp == NULL) {
	printf("Invalid file path. Try again or type 'exit' to exit: ");
	fgets(filePath, PATH_MAX, stdin);
	if (strcmp(filePath, "exit\n") == 0) {
	    printf("Exited successfully.\n");
	    if (fp != NULL) {
		fclose(fp);
	    }
	    return 1;
	}
	filePath[strlen(filePath) - 1] = '\0';
	fp = fopen(filePath, "r");
    }
    fclose(fp);
    return 0;
}

void help(char *request)
{
    if (strcmp(request, "exit") == 0) {
	printf
	    ("The command 'exit' closes the tetrashell program.\nExample usage: "
	     "'exit'.\n");
    } else if (strcmp(request, "recover") == 0) {
	printf
	    ("The command 'recover' recovers quicksaves from the given file "
	     "path.\nExample usage: 'recover /playpen/a4/a4_disk.img'.\n");
    } else if (strcmp(request, "modify") == 0) {
	printf
	    ("The command 'modify' changes score or lines value in the current "
	     "quicksave.\nExample usage: 'modify lines 10'\n");
    } else if (strcmp(request, "check") == 0) {
	printf
	    ("The command 'check' verifies that the current quicksave passes "
	     "legitimacy checks.\nExample usage: 'check'.\n");
    } else if (strcmp(request, "rank") == 0) {
	printf
	    ("The command 'rank' ranks the current quicksave against all quicksaves "
	     "uploaded to the course server based on lines or score.\nThe default "
	     "ranking system will print the top 5 quicksaves based on "
	     "score.\nExample usage: 'rank'.\nYou can also specify what metric you "
	     "want to use for ranking.\nExample usage: 'rank score'.\nThe last "
	     "option is to specify both the metric to rank by and how many "
	     "quicksaves will be printed.\nExample usage: 'rank score 10'.\n");
    } else if (strcmp(request, "switch") == 0) {
	printf
	    ("The command 'switch' changes the current quicksave to the specified "
	     "quicksave.\nExample usage: 'switch ./tetris_quicksave.bin.'\n");
    } else if (strcmp(request, "info") == 0) {
	printf
	    ("The command 'info' prints the current quicksave path along with its "
	     "score and lines.\nExample usage: 'info'.\n");
    } else if (strcmp(request, "commands") == 0) {
	printf
	    ("The command 'commands' prints out all available commands.\nExample "
	     "usage: 'commands'\n");
    } else if (strcmp(request, "undo") == 0) {
	printf
	    ("The command 'undo' reverses the last modification.\nExample usage: "
	     "'undo'\n");
    } else {
	printf("Unrecognized command.\n");
    }
}

void info(char *currentGame)
{
    TetrisGameState game;
    FILE *currentFile = fopen(currentGame, "rb");
    if (currentFile == NULL) {
	printf("Invalid game file path.");
    }
    fread(&game, sizeof(TetrisGameState), 1, currentFile);
    printf("Current savefile: %s\nScore: %d\nLines: %d\n", currentGame,
	   game.score, game.lines);
    fclose(currentFile);
}

int switchSave(char *file, char *filePath)
{
    if (validateFile(file)) {
	return 1;
    }
    printf("Switched current quicksave from %s to %s.\n", filePath, file);
    strcpy(filePath, file);
    filePath[strlen(filePath)] = '\0';
    return 0;
}

int recover(char **arguments)
{
    pid_t pid;
    if ((pid = fork()) == 0) {
	// creating necessary arguments
	char *tempCommand[] = { "./recover", arguments[0], NULL };
	char *path = "/playpen/a5/recover";
	execve(path, tempCommand, NULL);
	printf("Execve failure.\n");
	return 1;
    } else if (pid > 0) {
	int status;
	wait(&status);
    } else if (pid < 0) {
	printf("Fork failure.\n");
	return 1;
    }
    return 0;
}

int modify(char **arguments, char *filePath)
{
    pid_t pid;
    if ((pid = fork()) == 0) {
	// creating arguments for modify
	char *tempCommand[] =
	    { "./modify", arguments[0], arguments[1], filePath,
	    NULL
	};
	char *path = "/playpen/a5/modify";
	execve(path, tempCommand, NULL);
	printf("Execve failure.\n");
	return 1;
    } else if (pid > 0) {
	int status;
	wait(&status);
    } else if (pid < 0) {
	printf("Fork failure.\n");
	return 1;
    }
    return 0;
}

int check(char *filePath)
{
    pid_t pid;
    if ((pid = fork()) == 0) {
	// creating arguments for check
	char *tempCommand[] = { "./check", filePath, NULL };
	char *path = "/playpen/a5/check";
	execve(path, tempCommand, NULL);
	printf("Execve failure.\n");
	return 1;
    } else if (pid > 0) {
	int status;
	wait(&status);
    } else if (pid < 0) {
	printf("Fork failure.\n");
	return 1;
    }
    return 0;
}

int rank(char **arguments, char *filePath)
{
    pid_t pid;
    int rankPipe[2];
    if (pipe(rankPipe) < 0) {
	perror("pipe error");
	return 1;
    }
    if ((pid = fork()) == 0) {
	close(rankPipe[1]);
	dup2(rankPipe[0], 0);
	char *tempCommand[5];
	if (arguments[0] == NULL) {
	    char *tempCommand[] =
		{ "./rank", "score", "5", "uplink", NULL };
	} else if (arguments[1] == NULL) {
	    char *tempCommand[] =
		{ "./rank", arguments[0], "5", "uplink", NULL };
	} else {
	    char *tempCommand[] =
		{ "./rank", arguments[0], arguments[1], "uplink",
		NULL
	    };
	}
	char *path = "/playpen/a5/rank";
	execve(path, tempCommand, NULL);
	printf("Execve failure.\n");
	return 1;
    } else if (pid > 0) {
	close(rankPipe[0]);
	write(rankPipe[1], filePath, strlen(filePath));
	close(rankPipe[1]);
	int status;
	wait(&status);
    } else if (pid < 0) {
	printf("Fork failure.\n");
	return 1;
    }
    return 0;
}

void freeSpace(char **arguments, int i)
{
    for (int j = 0; j < i; j++) {
	free(arguments[j]);
	arguments[j] = NULL;
    }
}

void commands()
{
    printf
	("Available commands: commands, recover, modify, check, rank, switch, "
	 "help, info, undo, and exit.\nType 'help *command*' to get information "
	 "about the command.\n");
}

void copyFile(char *filePath, TetrisGameState * copy)
{
    FILE *fp;
    fp = fopen(filePath, "rb");
    if (filePath == NULL) {
	perror("fopen error.");
    }
    fread(copy, sizeof(TetrisGameState), 1, fp);
    fclose(fp);
}

int undo(TetrisGameState * game, char *filePath)
{
    char *score = malloc(20 * sizeof(char));
    char *lines = malloc(20 * sizeof(char));

    sprintf(score, "%u", game->score);
    sprintf(lines, "%u", game->lines);

    pid_t pid;

    if ((pid = fork()) == 0) {
	char *tempCommand[] =
	    { "./modify", "score", score, filePath, NULL };
	char *path = "/playpen/a5/modify";
	execve(path, tempCommand, NULL);
	free(score);
	free(lines);
	printf("Execve failure.\n");
	return 1;
    } else if (pid > 0) {
	int status;
	wait(&status);
    } else if (pid < 0) {
	printf("Fork failure.\n");
	free(score);
	free(lines);
	return 1;
    }

    if ((pid = fork()) == 0) {
	char *tempCommand[] =
	    { "./modify", "lines", lines, filePath, NULL };
	char *path = "/playpen/a5/modify";
	execve(path, tempCommand, NULL);
	printf("Execve failure.\n");
	free(score);
	free(lines);
	return 1;
    } else if (pid > 0) {
	int status;
	wait(&status);
    } else if (pid < 0) {
	printf("Fork failure.\n");
	free(score);
	free(lines);
	return 1;
    }
    free(score);
    free(lines);
    return 0;
}

int main()
{
    char input[256];
    char filePath[PATH_MAX];

    printf("\033[38;2;0;255;255mWelcome to...\n");
    printf
	("████████╗███████╗████████╗██████╗  █████╗ ███████╗██╗  ██╗███████╗██╗   "
	 "  ██╗\n");
    printf
	("╚══██╔══╝██╔════╝╚══██╔══╝██╔══██╗██╔══██╗██╔════╝██║  ██║██╔════╝██║   "
	 "  ██║\n");
    printf
	("   ██║   █████╗     ██║   ██████╔╝███████║███████╗███████║█████╗  ██║   "
	 "  ██║\n");
    printf
	("   ██║   ██╔══╝     ██║   ██╔══██╗██╔══██║╚════██║██╔══██║██╔══╝  ██║   "
	 "  ██║\n");
    printf
	("   ██║   ███████╗   ██║   ██║  ██║██║  ██║███████║██║  "
	 "██║███████╗███████╗███████╗\n");
    printf
	("   ╚═╝   ╚══════╝   ╚═╝   ╚═╝  ╚═╝╚═╝  ╚═╝╚══════╝╚═╝  "
	 "╚═╝╚══════╝╚══════╝╚══════╝\n");
    printf("Enter the path of the quicksave you want to hack:\033[38;2;0;255;255m ");
    fgets(filePath, PATH_MAX, stdin);
    filePath[strlen(filePath) - 1] = '\0';
    if (validateFile(filePath)) {
	return 0;
    }
    printTetrashell(filePath);
    TetrisGameState *copy = malloc(sizeof(TetrisGameState));
    while (1) {
	char *arguments[2];
	fgets(input, 256, stdin);
	char output[256];
	strcpy(output, input);
	char *command = strtok(input, " ");
	char *argument = strtok(NULL, " ");
	int i = 0;
	// parsing input to make sure each string in the array arguments is null
	// terminated
	while (argument != NULL) {
	    arguments[i] = malloc(strlen(argument) + 1);
	    strcpy(arguments[i], argument);
	    if (arguments[i][strlen(argument) - 1] == '\n') {
		arguments[i][strlen(argument) - 1] = '\0';
	    } else {
		arguments[i][strlen(argument)] = '\0';
	    }
	    argument = strtok(NULL, " ");
	    i++;
	}
	if (strcmp(command, "exit\n") == 0) {
	    printf("Exited successfully!\n");
	    freeSpace(arguments, i);
	    if (copy->score != '\0') {
		free(copy);
	    }
	    return 0;
	} else if (strcmp(command, "recover") == 0) {
	    if (recover(arguments)) {
		freeSpace(arguments, i);
		return EXIT_FAILURE;
	    }
	    printTetrashell(filePath);
	    freeSpace(arguments, i);
	} else if (strcmp(command, "modify") == 0) {
	    if (copy->score != '\0') {
		free(copy);
	    }
	    copyFile(filePath, copy);
	    if (modify(arguments, filePath)) {
		freeSpace(arguments, i);
		return EXIT_FAILURE;
	    }
	    printTetrashell(filePath);
	    freeSpace(arguments, i);
	} else if (strcmp(command, "check\n") == 0) {
	    if (check(filePath)) {
		freeSpace(arguments, i);
		return EXIT_FAILURE;
	    }
	    printTetrashell(filePath);
	    freeSpace(arguments, i);
	} else if (strcmp(command, "rank") == 0
		   || strcmp(command, "rank\n") == 0) {
	    if (rank(arguments, filePath)) {
		freeSpace(arguments, i);
		return EXIT_FAILURE;
	    }
	    printTetrashell(filePath);
	    freeSpace(arguments, i);
	} else if (strcmp(command, "switch") == 0) {
	    if (switchSave(arguments[0], filePath) == 1) {
		freeSpace(arguments, i);
		return EXIT_SUCCESS;
	    }
	    printTetrashell(filePath);
	    freeSpace(arguments, i);
	} else if (strcmp(command, "help") == 0) {
	    help(arguments[0]);
	    printTetrashell(filePath);
	    freeSpace(arguments, i);
	} else if (strcmp(command, "info\n") == 0) {
	    info(filePath);
	    printTetrashell(filePath);
	    freeSpace(arguments, i);
	} else if (strcmp(command, "commands\n") == 0) {
	    commands();
	    printTetrashell(filePath);
	    freeSpace(arguments, i);
	} else if (strcmp(command, "undo\n") == 0) {
	    if (copy->score != '\0') {
		undo(copy, filePath);
	    } else {
		printf("No modifies have occurred yet.\n");
	    }
	    printTetrashell(filePath);
	    freeSpace(arguments, i);
	} else {
	    printf("%s", output);
	    printTetrashell(filePath);
	    freeSpace(arguments, i);
	}
    }
    return 0;
}
