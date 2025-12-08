#include "init.h"

int parse_database(db_t db, char **lines, int count)
{
    if(!lines || !count)
        return 0;

    char **args; int arg_c;

    /* Parse Header Line */
    if(!parse_db_header(db, lines))
        printf("[ x ] Error, Unable to parse database header...!\n");

    /* Parse Tables */
    _table table = {0};
    for(int i = 2; i < count; i++)
    {
        // Skip empty lines
        if(lines[i][0] == ' ') continue;

        /* Strip Table Header Syntax */
        if(lines[i][0] != '@') {
	        trim_idx(lines[i], 0);
	        trim_idx(lines[i], 0);
	        int len = strlen(lines[i]);
	        lines[i][len - 4] = '\0';
		}

        /* Parse Table Header and Table Columns and Rows */
        if(lines[i][0] == '@')
        {
            if(___DB_DEBUG)
                printf("Table Found: '%s'\n", lines[i]);

            // Check to see if the current table contains any rows to save
            if(table.len > 0)
            {
                if(___DB_DEBUG)
                    printf("Adding table: %s to db....!\n", table.name);
                    
                if(!append_table(db, to_heap(&table, sizeof(_table))))
                    printf("[ x ] Error, Unable to add table to db table list...\n");
            }

            /* Table Header Info */
            if(!parse_table_header(db, &table, lines, i))
                printf("[ x ] Error, Unable to parse table header...!\n");
                
             i++;
             if(!parse_table(db, &table, lines, count, &i))
                printf("[ x ] Error, Unable to parse table...!\n");

             i--;
        }

        if(___DB_DEBUG)
		    printf("[ LINE:%d ]: %s\n", i, lines[i]);
    }
    
    // db->tables[db->len++] = to_heap(&table, sizeof(_table));
    append_table(db, to_heap(&table, sizeof(_table)));
    db->tables[db->len] = NULL;

    return 1;
}

int parse_db_header(db_t db, char **lines)
{
    char **args; int arg_c;

    /* Parse Header Line */
    char *header = lines[0];
    if(strstr(header, "[@DB"))
    {
        args = __split(header, " ", &arg_c);
        //args[1][strlen(args[1]) - 1] = '\0';

        db->name = strdup(args[1]);
        db->created_at = strdup(args[2]);
		db->name[strlen(db->name) - 1] = '\0';

        /* Debug */
        if(___DB_DEBUG)
        	printf("db: %s | Created At: %s\n", db->name, db->created_at);
        free_arr((void *)args);
    }

    /* Ensure there is tables in the database */
    db->all_tables = __split(lines[1], ", ", &db->table_count);
    if(!db->table_count)
    {
        _err("No tables found in database....!\n");
        return 0;
    }
}

int parse_table_header(db_t db, table_t table, char **lines, int line_c)
{
    if(!db || !table || !lines)
        return 0;

    char **args; int arg_c;

    /* Table Header Info */
    args = __split(lines[line_c], " ", &arg_c);
    *table = (_table){
       .name = strdup(args[0]),
       .created_at = strdup(args[1]),
       .rows = (char **)malloc(sizeof(char *) * 1),
       .columns = __split(lines[line_c + 1], ",", &arg_c),
       .len = 0
    };
    table->name[strlen(table->name) - 1] = '\0';

    table->idx = arg_c;
    free_arr((void *)args);

    return 1;
}

int parse_table(db_t db, table_t table, char **lines, int line_c, int *start)
{
    if(!db || !lines)
        return 0;

    char **args;
    int arg_c;
    (*start)++;
    for(;*start < line_c; (*start)++)
    {
        if(lines[*start][0] != '(' && lines[*start][0] == '@')
            break;

        if(lines[*start][0] != '@') {
	        trim_idx(lines[*start], 0);
	        trim_idx(lines[*start], 0);
	        int len = strlen(lines[*start]);
	        lines[*start][len - 4] = '\0';
		}

        if(___DB_DEBUG)
            printf("[ PARSE_TABLE - LINE:%d ] %s\n", *start, lines[*start]);

        args = __split(lines[*start], "','", &arg_c);
        if(!args) if(___DB_DEBUG) printf("Failed\n");
        if(arg_c > 0) {
	        table->rows[table->len++] = (char *)args;
	        if(!(table->rows = (char **)realloc(table->rows, sizeof(char *) * (table->len + 1)))) {
	            _rememerr;
	            break;
	        }
        }

        table->rows[table->len] = NULL;
    }

    return 1;
}
