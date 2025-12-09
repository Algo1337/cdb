#include "../src/init.h"

int main(int argc, char *argv[])
{
    if(argc < 2)
    {
        printf("[ x ] Error, Invalid arguments provided\nUsage: %s <database>\n", argv[0]);
        return 1;
    }

    db_t db = open_db(argv[1]);

    if(!db)
    {
        _err("unable to open database....!\n");
        return 1;
    }

    table_pipe_t logs = get_table(db, "@logs"); // pipe LOGS table

    // Iterate username in user table */
    printf("List of rows in %s table...!\n", logs->name);
    for(int i = 0; i < logs->len; i++)
    {
    	char **args = (char **)logs->rows[i];
    	printf("[%d]: ", i);
    	for(int c = 0; args[c] != NULL; c++)
    		printf((args[c + 1] ? "%s, " : "%s"), args[c]);
    }
    printf("\n");


	table_row_value row = (table_row_value)get_row(logs, 0, 1, "dick_db");
	printf("First-Row Second-Cell Value: %s\n", row);

    return 0;
}
