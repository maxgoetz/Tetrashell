// I, Max Goetz (730465187), pledge that I have neither given nor received unauthorized aid on this assignment.
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int main() 
{
	if (strcmp(getenv("TERM"), "xterm-256color") == 0) {
		printf("Hello! %s thinks it's a \033[38;2;0;255;255mGDTBATH!\033[0m\n", getlogin());
	} else {
		printf("Hello! %s thinks it's a GDTBATH!\n", getlogin());
	} return 0;
}
