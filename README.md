<div align="center">
    <h1>cdb</h1>
	<p>db written in C</p>
</div>

# How To Install

Should work on any system with standard library and openssl for encryption

```bash
wget https://github.com/algo1337/cdb.git
cd cdb
make
```

# Quick Doc

```
/* Open a database */
db_t open_db(char *filename);

/* Create a new database */
db_t create_db(char *name);

/* Create a new table */
table_pipe_t create_table(db_t db, char *name, char **tables);

/* Append New Row */
int append_row(db_t db, table_t table, char **args);

/* Search Table */
table_pipe_t get_table(db_t db, char *name);

/* Destruct db */
void db_Destruct(db_t db);

/*
   	Get a row cell by passing a cell index to
    'where_col' and a query to 'search' for look up

    Get the full row by passing -1 to 'where_col'
*/
table_row get_row(table_t table, int from_col, int where_col, char *query);

/*
    Change a row cell by passing a cell index to
    'where_col' and a query to 'search' for look up

    Get the full row by passing -1 to 'where_col'
*/
int change_cell(table_t table, int from, int where, char *search, char *new_data);

```

# Contribution

Request a PR for contribution

# Contact

discord; heineken_t
