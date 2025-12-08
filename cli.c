#include "src/init.h"

int main(int argc, char *argv[])
{
	if(argc < 2)
	{
		printf("[ x ] Error, no database provided\nUsage %s <database>\n", argv[0]);
		return 1;
	}

	open_db(argv[1]);
	cdb_cli();
	return 0;
}
