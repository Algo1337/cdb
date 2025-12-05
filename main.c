#include "src/init.h"

typedef struct
{
    db_t            db;
    table_pipe_t    users;
    table_pipe_t    logs;
} my_app;


int main(int argc, char *argv[])
{
    ___DB_DEBUG = 0;
    if(argc < 2)
    {
        printf("[ x ] Error, Invalid arguments provided\nUsage: %s <database>\n", argv[0]);
        return 1;
    }

    my_app app = { .db = open_db(argv[1]) };

    if(!app.db)
    {
        _err("unable to open database....!\n");
        return 1;
    }

    app.users = get_table(app.db, "@USERS"); // pipe USER table
    app.logs = get_table(app.db, "@LOGS"); // pipe LOGS table

    /* Ensure we retrieved them */
    if(!app.users || !app.logs)
    {
        printf("Unable to get user table from db\n");
        return 1;
    }

    // Iterate username in user table */
    for(int i = 0; i < app.users->len; i++)
    {
        printf("Username: %s\n", ((char **)app.users->rows[i])[0]);
    }


    /* Iterate Tables */

    // printf("Tables Found: %d\n", db->len);
    // for(int i = 0; i < db->len; i++)
	// {
	// 	printf("Name: %s\n", db->tables[i]->name);
    //     /* Iterate Table Rows */
	// 	for(int c = 0; c < db->tables[i]->len; c++) {
    //         /* Iterate Table Row Columns-Values */
	// 		for(int n = 0; n < db->tables[i]->idx; n++) {
    //             printf("[ TABLE: %s:%d;%d ] %s\n", db->tables[i]->name, c, n, (char *)((char **)db->tables[i]->rows[c])[n]);
    //         }
	// 		printf("\n");
	// 	}
	// }

    return 0;
}
