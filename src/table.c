#include "init.h"

table_t new_table(char *name, char **columns)
{
    if(!name)
        return NULL;

    table_t table = (table_t)malloc(sizeof(_table));
	table->name = strdup(name);
	table->created_at = strdup(get_current_time());
	table->rows = (char **)malloc(sizeof(char *) * 1);
	table->len = 0;
	table->columns = (char **)malloc(sizeof(char *) * 1);
	table->idx = 0;

    if(!table->columns)
    {
        _memerr;
        return NULL;
    }

    for(int i = 0; columns[i] != NULL; i++)
    {
    	table->columns[table->idx++] = strdup(columns[i]);
    	if(!(table->columns = (char **)realloc(table->columns, sizeof(char *) * (table->idx + 1))))
    	{
    		_memerr;
    		return NULL;
    	}

		table->columns[table->idx] = NULL;
    }

    return table;
}

/*
	Get a row cell by passing a cell index to
	'where_col' and a query to 'search' for look up

	Get the full row by passing -1 to 'where_col'
*/
table_row get_row(table_t table, int from_col, int where_col, char *search)
{
	if(!table || !search)
		return NULL;

	if(where_col != -1 && where_col > table->idx)
		return NULL;

	for(int i = 0; i < table->len; i++)
	{
		char **args = (char **)table->rows[i];
		if(from_col == -1)
		{
			if(!strcmp(args[where_col], search))
				return (table_row)table->rows[i];
		} else if(where_col > -1 && from_col > -1)
		{
			if(!strcmp(args[where_col], search))
				return (char **)args[from_col];
		}
	}

	return NULL;
}

int change_cell(table_t table, int from, int where, char *search, char *new_data)
{
	if(!table || !new_data)
		return 0;

	if(where > table->idx)
		return 0;

	for(int i = 0; i < table->len; i++)
	{
		char **args = (char **)table->rows[i];
		if(where == -1)
		{
			if(!strcmp(args[where], search)) {
				table->rows[i] = new_data;
				return 1;
			}
		} else if(where > -1 && from > -1)
		{
			if(!strcmp(args[where], search)) {
				((char **)table->rows[i])[from] = strdup(new_data);
				return 1;
			}
		}
	}

	return 0;
}

int change_row(table_t table, int where_col, char *search, char **args)
{
	if(!table || !search)
		return 0;

	if(where_col > table->idx)
		return 0;

	for(int i = 0; i < table->len; i++)
	{
		char **args = (char **)table->rows[i];
		if(where_col == -1)
		{
			// copy args
			char **array = malloc(sizeof(char *) * 1);
			int c;
			for(c = 0; args[c] != NULL; c++) {
				array[c] = strdup(args[c]);
				if(!(array = (char **)realloc(array, sizeof(char *) * (c + 1))))
					_rememerr;
			}
		}
	}
}

char *row_to_db(char **args)
{
	if(!args)
		return NULL;

	char data[1024 * 2] = {0};

	strcat(data, "('");

	for(int i = 0; args[i] != NULL; i++) {
		strcat(data, args[i]);
		if(args[i + 1]) strcat(data, "','");
	}
	strcat(data, "')\n");

	int len = strlen(data);
	if(len == 0)
		return NULL;

	data[len - 1] = '\0';
	return str_to_heap(data);
}

char *table_to_db(table_t table)
{
	if(!table)
		return NULL;

	char data[1024 * 2] = {0};

	char *meta[] = {"@", table->name, ": ", table->created_at, NULL};
	for(int i = 0; i < 4; i++)
		strcat(data, meta[i]);

	strcat(data, "\n('");
	for(int i = 0; i < table->idx; i++) {
		strcat(data, table->columns[i]);
		if(table->columns[i + 1]) strcat(data, "','");
	}

	strcat(data, "')\n");
	if(table->len > 0) {
		if(___DB_DEBUG)
			printf("Saving Rows: %d\n", table->len);

		for(int i = 0; i < table->len; i++)
		{
			char **args = (char **)table->rows[i];
			char *row = row_to_db(args);

			if(___DB_DEBUG)
				printf("[ ROW_PREVIEW ]: \"%s\"\n", row);

			if(!row)
				return NULL;

			strcat(data, row);
			strcat(data, "\n");

			free(row);
		}
	}

	strcat(data, "\n");
	int len = strlen(data);
	if(len == 0)
		return NULL;

	data[len - 1] = '\0';
	return str_to_heap(data);
}

void table_Destruct(table_t table)
{
	if(table->name)
		free(table->name);

	if(table->created_at)
		free(table->created_at);

	if(table->rows)
	{
		for(int i = 0; i < table->len; i++)
		{
			free_arr((void *)table->rows[i]);
		}

		free(table->rows);
	}

	if(table->columns)
	{
		for(int i = 0; i < table->idx; i++)
		{
			free(table->columns[i]);
		}
		free(table->columns);
	}

	free(table);
}
