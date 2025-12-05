#include "init.h"

table_t new_table(char *name)
{
    if(!name)
        return NULL;
        
    _table table = {
        .name = strdup(name),
        .created_at = strdup(get_current_time()),
        .rows = (char **)malloc(sizeof(char *) * 1),
        .len = 0,
        .columns = (char **)malloc(sizeof(char *) * 1),
        .idx = 0
    };

    if(!table.rows || !table.columns)
    {
        _memerr;
        return NULL;
    }

    return to_heap(&table, sizeof(_table));
}

table_row get_row(table_t table, int where_col, char *query)
{

}

table_row_value get_row_value(int column, int where_col, char *query)
{

}