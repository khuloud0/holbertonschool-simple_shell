#include "Shell.h"

int main(int argc, char **argv)
{
	char *line;
	char **args;
	int status = 0;

	(void)argc;

	while (1)
	{
		line = read_input();
		if (!line)
			break;

		args = parse_line(line);
		if (!args || !args[0])
		{
			free_tokens(args);
			free(line);
			continue;
		}

		status = handle_builtins(args);
		if (status == -1)
			status = execute_command(args, argv);

		free_tokens(args);
		free(line);
	}

	return (status);
}
