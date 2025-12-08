#include "init.h"

void strip_input(int *len, char *buffer)
{
	if(*len < 1)
		return;

	if(buffer[(*len) - 1] == '\r' || buffer[(*len) - 1] == '\n')
		buffer[(*len) - 1] = '\0';

	(*len)--;
	if(buffer[(*len) - 1] == '\r' || buffer[(*len) - 1] == '\n')
		buffer[(*len) - 1] = '\0';
}

void cdb_cli()
{
	table_t table;
	char buffer[255], **args;
	int len, arg_c = 0;

	printf("[ cdb CLI 0.13 ] Welcome to cDB!\n> ");
	while(fgets(buffer, 254, stdin) != NULL)
	{
		len = strlen(buffer);
		strip_input(&len, buffer);
		if(len == 0)
			continue;

		if(strstr(buffer, " "))
			args = __split(buffer, " ", &arg_c);

		if(strstr(buffer, "get_table") && arg_c >= 2) {
			table = get_table(__CURRENT_DB, args[1]);
			if(!table)
			{
				printf("[ x ] Error, Invalid table\n");
				continue;
			}

			for(int i = 0; i < table->len; i++) {
				char **args = (char **)table->rows[i];
				for(int c = 0; args[c] != NULL; c++) {
					printf((args[c + 1] ? "%s, " : "%s"), args[c]);
				}

				printf("\n");
			}
		} else if(strstr(buffer, "get_row"))
		{
			// get_row -1 username root
			// get_row password username root
			if(arg_c < 4) {
				_err("Invalid argument...!");
				continue;
			}

			if(!strcmp(args[1], "-1")) {
				char **row = (char **)get_row(table, -1, atoi(args[2]), args[3]);
				for(int i = 0; row[i] != NULL; i++)
					printf((row[i + 1] ? "%s, " : "%s"), row[i]);

				printf("\n");
			} else {
				table_row_value v = (table_row_value)get_row(table, atoi(args[1]), atoi(args[2]), args[3]);
				printf("%s\n", v);
			}
		} else if(strstr(buffer, "change_row_value"))
		{
			if(arg_c < 5) {
				_err("Invalid argument...!");
				continue;
			}

			if(!change_cell(table, atoi(args[1]), atoi(args[2]), args[3], args[4]))
				_err("CHANGE ERR");
		}

		printf("> ");
		if(arg_c != 0)
			free_arr((void *)args);
	}
}
