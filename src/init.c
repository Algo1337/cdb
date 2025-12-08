#include "init.h"

int ___DB_DEBUG = 0;
db_t __CURRENT_DB = NULL;

__attribute__((constructor)) void db_init()
{
    system("mkdir -p /dbs/");
}

__attribute__((destructor)) void db_uninit()
{
    /* Free __CURRENT_DB struct */
}

db_t create_db(char *name)
{
    if(!name)
        return NULL;

    _db db = {
        .name = strdup(name),
		.created_at = get_current_time(),
        .all_tables = (char **)malloc(sizeof(char *) * 1),
        .table_count = 0,
        .tables = (tables_t)malloc(sizeof(_table *) * 1),
        .len = 0
    };

    char path[strlen(name) + 9];
    strcpy(path, DEFAULT_DB_PATH);
    strcat(path, name);
    strcat(path, DEFAULT_DB_EXTENSION);

    __CURRENT_DB = to_heap(&db, sizeof(_db));
    return __CURRENT_DB;
}

db_t open_db(char *name)
{
    if(!name)
        return NULL;

    _db db = {
        .len = 0
    };

    char path[strlen(name) + 9];
    strcpy(path, DEFAULT_DB_PATH);
    strcat(path, name);
    strcat(path, DEFAULT_DB_EXTENSION);

    int buffer_len;
    char *content = _get_file_content(path, &buffer_len);

    // Ensure there's content to read
    if(buffer_len == 0)
    {
        _err("the database is not initialized or setup...!");
        return NULL;
    }

    int line_count;
    char **lines = __split(content, "\n", &line_count);

    db.tables = (tables_t)malloc(sizeof(_table) * 1);
    db.len = 0;
    int chk = parse_database(&db, lines, line_count);
    if(!chk)
    {
        _err("the database is not setup...!");
        return NULL;
    }

    /* TODO: Free content and lines */
    free_arr((void *)lines);
    free(content);

    __CURRENT_DB = to_heap(&db, sizeof(_db));
    return __CURRENT_DB;
}

table_pipe_t create_table(db_t db, char *name, char **tables)
{
	if(!name || !tables)
		return NULL;

    /* Ensure there isnt a table with the name */
	table_t chk = get_table(db, name);
	if(chk)
	{
		_err("table already exist...!");
		return NULL;
	}

    /* New Table */
    table_t table = new_table(name, tables);
    if(!table)
	{
		_err("unable to create table instance...!");
		return NULL;
	}

    
    append_table(db, table);
    return table;
}

int append_table(db_t db, table_t table)
{
    if(!db || !table)
        return 0;

    db->all_tables[db->table_count++] = strdup(table->name);
    db->all_tables = realloc(db->all_tables, sizeof(char *) * (db->table_count + 1));
    if(!db->all_tables)
    {
        _rememerr;
    }

    db->tables[db->len++] = table;
    db->tables = (tables_t)realloc(db->tables, sizeof(_table *) * (db->len + 1));
    if(!db->tables)
    {
        _rememerr;
    }

    db->tables[db->len] = NULL;
    return 1;
}

int append_row(db_t db, table_t table, char **args)
{
    if(!db || !table || !args)
        return 0;

    /* TODO; Ensure args has the same amount of columns as the table */

    char **arr = (char **)malloc(sizeof(char *));
    int i;
    for(i = 0; args[i] != NULL; i++) {
        arr[i] = strdup(args[i]);
        arr = (char **)realloc(arr, sizeof(char *) * (i + 2));
    }
    arr[i] = NULL;

    table->rows[table->len++] = (char *)arr;
    if(!(table->rows = (char **)realloc(table->rows, sizeof(char *) * (table->len + 1)))) {
        _rememerr;
        return 0;
    }

    table->rows[table->len] = NULL;
    return 1;
}


table_pipe_t get_table(db_t db, char *name)
{
    if(!db || !name)
        return NULL;

	if(db->len == 0)
		return NULL;

    for(int i = 0; i < db->len || db->tables[i] != NULL; i++)
    {
        if(!strcmp(db->tables[i]->name, name))
            return db->tables[i];
    }

    return NULL;
}

int save_database(db_t db)
{
	if(!db)
		return 0;

	/* Add Database Header */
	char data[1024 * 4] = {0};
    strcpy(data, "[@DB: ");
    strcat(data, db->name);
    strcat(data, "] ");
    strcat(data, db->created_at);
    strcat(data, "\n");

    for(int i = 0; i < db->table_count; i++) {
        strcat(data, db->all_tables[i]);
        if(db->all_tables[i + 1]) strcat(data, ", ");
    }

    strcat(data, "\n\n");
    for(int i = 0; db->tables[i] != NULL; i++)
    {
        char *table_data = table_to_db(db->tables[i]);
        if(!table_data)
        {
            _err("failed to convert table to string...!");
            break;
        }
        strcat(data, table_data);
        strcat(data, "\n");
    }

	if(___DB_DEBUG)
	    printf("[ SAVE_PREVIEW ]: %s", data);

    char path[50] = {0};
    strcpy(path, DEFAULT_DB_PATH);
    strcat(path, db->name);
    strcat(path, DEFAULT_DB_EXTENSION);

    save_file(path, data);
    return 1;
}

void db_Destruct(db_t db)
{
    if(db->name)
        free(db->name);

    if(db->created_at)
        free(db->created_at);

    if(db->all_tables)
    {
        for(int i = 0; i < db->table_count; i++)
            free(db->all_tables[i]);
    }

    if(db->tables)
    {
        for(int i = 0; i < db->len; i++)
            table_Destruct(db->tables[i]);

        free(db->tables);
    }

    free(db);
}
