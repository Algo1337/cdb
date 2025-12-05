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

db_t create_db(char *name, char **tables)
{
    if(!name)
        return NULL;

    db_t db = new_table(name);
    if(!db)
    {
        _err("unable to create table instance....!");
        return NULL;
    }

    char path[strlen(name) + 9];
    strcpy(path, "/dbs/");
    strcat(path, name);
    strcat(path, ".db");

    if(!create_db_file(name, path, tables, db->created_at)) {
        _err("[ x ] unable to create db file...!\n");
        return NULL;
    }

    __CURRENT_DB = to_heap(&db, sizeof(_db));
    return __CURRENT_DB;
}

db_t open_db(char *name)
{
    if(!name)
        return NULL;

    _db db = {0};

    char path[strlen(name) + 9];
    strcpy(path, "/dbs/");
    strcat(path, name);
    strcat(path, ".db");

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

int append_table(db_t db, table_t table)
{
    if(!db || !table)
        return 0;

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

    table->rows[table->len++] = (char *)args;
	if(!(table->rows = (char **)realloc(table->rows, sizeof(char *) * (table->len + 1)))) {
	    _rememerr;
	    return 0;
	}

    return 1;
}


table_pipe_t get_table(db_t db, char *name)
{
    if(!db || !name)
        return NULL;

    for(int i = 0; i < db->len || db->tables[i] != NULL; i++)
    {
        if(!strcmp(db->tables[i]->name, name))
            return db->tables[i];
    }

    return NULL;
}