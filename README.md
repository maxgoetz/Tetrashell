Tetrashell is a command line interface coded in vim that allows you to perform many different functions on a tetris quicksave.

Available Commands:
1. **Rank**
2. **Modify**
3. **Recover**
4. **Check**
5. **Exit**
6. **Switch**
7. **Help**
8. **Info**
9. **Commands**
10. **Undo**


**Rank**:
	The command 'rank' ranks the current quicksave against all quicksaves uploaded to the course server based on lines or score. The default ranking system will print the top 5 quicksaves based on score. Example usage: 'rank'.
	You can also specify what metric you want to use for ranking. Example usage: 'rank score'.
	The last option is to specify both the metric to rank by and how many quicksaves will be printed. Example usage: 'rank score 10'.

**Modify**:
	The command 'modify' changes score or lines value in the current quicksave. Example usage: 'modify lines 10'.

**Recover**:
        The command 'recover' recovers quicksaves from the given file path. Example usage: 'recover /playpen/a4/a4_disk.img'.

**Check**:
	The command 'check' verifies that the current quicksave passes legitimacy checks. Example usage: 'check'.

**Exit**:
	The command 'exit' closes the tetrashell program. Example usage: 'exit'.

**Switch**:
	The command 'switch' changes the current quicksave to the specified quicksave. Example usage: 'switch ./tetris_quicksave.bin’.

**Help**:
	The command 'help' tells you about and how to use the specified command. Example usage: 'help *command*'.

**Info**:
	The command 'info' prints the current quicksave path along with its score and lines. Example usage: 'info'.

**Commands**:
	The command 'commands' prints out all available commands. Example usage: 'commands'.

**Undo**:
	The command 'undo' reverses the last modification. Example usage: 'undo'.

**Other Features**
	Improved Prompt featuring username, score, and rank, all in Tarheel blue.
