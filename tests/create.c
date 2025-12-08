#include "../src/init.h"

int main()
{
	printf("[ + ] Creating database....\n");
    db_t db = create_db("test");
    if(!db)
    {
        _err("unable to create a new db...!");
        //return 1;
    }

	printf("[ + ] Creating and initializing tables...\n");
	table_pipe_t users = create_table(
		db,
		"users",
		(char *[]){"username", "ip_addr", "password", NULL}
	);

    table_pipe_t logs = create_table(
    	db,
    	"logs",
    	(char *[]){"date", "log", NULL}
    );
    printf("[ + ] Table successfully created....!\n");

	// Default User
    append_row(db, users, (char *[]){"root", "0.0.0.0", "root", NULL});

    // Default Log
    append_row(db, logs, (char *[]){"12/17/25-6:30:02AM", "net_db_created", NULL});


	char *n = malloc(10);
	strcpy(n, "dick_db");

	free(((char **)logs->rows[0])[1]);
	((char **)logs->rows[0])[1] = n;
    save_database(db);
    db_Destruct(db);
    return 0;
}
