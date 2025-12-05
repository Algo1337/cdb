#include "init.h"

int create_db_file(char *name, char *db_name, char **tables, char *time)
{
    if(!db_name)
        return 0;

    FILE *stream = fopen(db_name, "w");
    if(!stream)
    {
        _err("unable to create file...!");
        return 0;
    }

    char data[1024];
    strcpy(data, "[@DB: ");
    strcat(data, name);
    strcat(data, "] ");
    strcat(data, time);
    strcat(data, "\n");

    for(int i = 0; tables[i] != NULL; i++) {
        strcat(data, tables[i]);
        if(tables[i + 1]) strcat(data, ", ");
    }

    strcat(data, "\n\n");
    for(int i = 0; tables[i] != NULL; i++)
    {
        strcat(data, "@");
        strcat(data, tables[i]);
        strcat(data, " ");
        strcat(data, time);
        strcat(data, "\n\n");
    }

    fprintf(stream, "%s", data);
    fclose(stream);

    return 1;
}

char *get_current_time()
{
    time_t t;
    struct tm *tm_info;
    char buffer[30];

    if(time(&t) == ((time_t)-1))
        _err("unable to get current time...!");

    if(!(tm_info = localtime(&t)))
        _err("unable to convert time to human text readable local time...!")

    if(!strftime(buffer, sizeof(buffer), "%m/%d/%Y-%I:%M:%S%p", tm_info))
        _err("unable to get current time...!");

    return to_heap(buffer, 30);
}

char *_get_file_content(char *filename, int *bytes)
{
    if(!filename) return NULL;

    FILE *stream = fopen(filename, "r");
    if(!stream) return NULL;

    fseek(stream, 0, SEEK_END);
    *bytes = ftell(stream);
    fseek(stream, 0, SEEK_SET);

    if(*bytes == 0)
        return NULL;

	int len = *bytes;
	char data[len + 5];
	memset(data, 0, len + 5);
    int n = fread(data, 1, (*bytes), stream);
    fclose(stream);
    if(n == 0)
    {
        _err("unable to read file....!");
        return NULL;
    }

    return to_heap(data, len + 5);
}

int count_char(char *buffer, char ch)
{
    int count = 0;
    for(int i = 0; buffer[i] != '\0'; i++)
        if(buffer[i] == ch)
            count++;

    return count;
}

char **__split(char *buffer, char *delim, int *idx)
{
    if(!buffer || !delim)
        return NULL;

    char **arr = (char **)malloc(sizeof(char *) * 1);
    if(!arr)
    {
        _memerr; return NULL;
    }
    *idx = 0;

    char *token, *copy = strdup(buffer), *rest = copy;
    while((token = strtok_r(rest, delim, &rest)))
    {
        int len = strlen(token);

        arr[*idx] = malloc(len + 1);
        memcpy(arr[*idx], token, len + 1);
        
        (*idx)++;

        if(!(arr = realloc(arr, sizeof(char *) * (*idx + 1)))) {
            free(copy);
            free_arr((void *)arr);
            _memerr; return NULL;
        }

    }

    free(copy);
    arr[*idx] = NULL;
    return arr;
}

int is_empty(char *buffer)
{
    for(int i = 0; buffer[i] != '\0'; i++)
    {
        if(buffer[i] != 32)
            return 0;
    }

    return 1;
}

int trim_char(char *buffer, char ch)
{
    if(!buffer)
        return 0;

    int idx = 0, len = strlen(buffer);
    char *data = (char *)malloc(len + 1);
    for(int i = 0; i < len; i++) {
        if(buffer[i] != ch)
            data[idx++] = buffer[i];
    }

    data[idx] = '\0';
    memcpy(buffer, data, idx);
    return 1;
}

int trim_idx(char *buffer, int index)
{
	if(!buffer)
		return 0;

	int idx = 0, len = strlen(buffer);
	char *data = (char *)malloc(len + 1);
	for(int i = 0; i < len; i++) {
		if(i != index)
			data[idx++] = buffer[i];
	}

    data[idx] = '\0';
    memcpy(buffer, data, idx);
	return 1;
}

void *to_heap(void *p, int sz)
{
    void *ptr = malloc(sz);
    memcpy(ptr, p, sz);

    return ptr;
}

void free_arr(void **arr)
{ 
    for(int i = 0; arr[i] != NULL; i++) 
        free(arr[i]);
}
