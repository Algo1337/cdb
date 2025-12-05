#pragma once

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifndef MINIMAL_DB_H
#define MINIMAL_DB_H

#define _alloc(type, name, sz) \
    type name = (type)(sz == -1 ? malloc(sizeof(type)) : malloc(sz));

#define _memerr printf("[ x ] Error %s;%d, unable to allocate memory....!\n", __FILE__, __LINE__);
#define _rememerr printf("[ x ] Error %s;%d, unable to reallocate memory....!\n", __FILE__, __LINE__);

#define loop(arr, op, callback) \
    for(int i = 0; arr[i] != NULL; i++) { \
        char *callback = arr[i]; \
        op; \
    } 

#define loop_fn(arr, count, fnc) \
    for(int i = 0; i < count; i++) { \
        #fnc(arr[i]); \
    }

#define _err(msg) \
    printf("[ x ] Error %s;%d, %s\n", __FILE__, __LINE__, msg);

typedef struct
{
    char        *name;
    char        *created_at;
    char        **rows;
    int         len;

    char        **columns;
    int         idx;
} _table;

typedef _table *table_t;
typedef _table **tables_t;

typedef struct
{
    char        *name;
    char        *created_at;
    char        **all_tables;
    int         table_count;
    tables_t    tables;
    int         len;
} _db;

typedef _db *db_t;
typedef _table *table_pipe_t;

typedef char **table_row;
typedef char *table_row_value;

/*
    Usually one DB is open per app so this is used
    to free the DB upon closure 
*/
extern db_t __CURRENT_DB;

/* Debug Console Output Status */
extern int ___DB_DEBUG;

// init.c
db_t open_db(char *filename);
db_t create_db(char *name, char **tables);
int append_table(db_t db, table_t table);
table_pipe_t get_table(db_t db, char *name);

// table.c
table_t new_table(char *name);
table_row get_row(int where_col, char *query);
table_row_value get_row_value(int column, int where_col, char *query);

// parser.c
int parse_database(db_t db, char **lines, int count);
int parse_db_header(db_t db, char **lines);
int parse_table_header(db_t db, table_t table, char **lines, int line_c);
int parse_table(db_t db, table_t table, char **lines, int line_c, int *start);

// utils.c
int create_db_file(char *name, char *db_name, char **tables, char *time);
char *get_current_time();
char *_get_file_content(char *filename, int *len);
int count_char(char *buffer, char ch);
char **__split(char *buffer, char *delim, int *count);
int is_empty(char *buffer);
int trim_char(char *buffer, char ch);
int trim_idx(char *buffer, int index);
void *to_heap(void *p, int sz);
void free_arr(void **arr);

#endif
