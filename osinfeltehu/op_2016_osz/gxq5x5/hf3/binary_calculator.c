#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main()
{
	int pipefd[2];
	
	if (pipe(pipefd) == -1)
	{
		perror("Hiba a cső nyitásakor\n");
		exit(EXIT_FAILURE);
	}
	
	pid_t pid = fork();
	
	if (pid == 0) // child
	{
		int operands[2];
		char operator;
		read(pipefd[0], operands, sizeof(operands));
		read(pipefd[0], &operator, sizeof(operator));
		close(pipefd[0]);
		
		int result;
		switch (operator)
		{
			case '+':
				result = operands[0] + operands[1];
				break;
			case '-':
				result = operands[0] - operands[1];
				break;
			case '*':
				result = operands[0] * operands[1];
				break;
			case '/':
				result = operands[0] / operands[1];
				break;
		}
		
		write(pipefd[1], &result, sizeof(result));
		printf("[Child] I sent the result: %i\n", result);
		close(pipefd[1]);
	}
	else // parent
	{
		int operands[2] = {19, 7};
		char operator = '-';
		write(pipefd[1], operands, sizeof(operands));
		write(pipefd[1], &operator, sizeof(operator));
		printf("[Parent] I sent: %i, %i, %c\n", operands[0], operands[1], operator);
		close(pipefd[1]);
		wait();
		
		int result;
		read(pipefd[0], &result, sizeof(result));
		printf("[Parent] I received the result: %i\n", result);
		
		close(pipefd[0]);
	}
}