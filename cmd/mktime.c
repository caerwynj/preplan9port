#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char * argv[])
{
	struct tm t;

	if(argc != 4){
		fprintf(stderr, "mktime yyyy mm dd\n");
		exit(1);
	}
	t.tm_year = atoi(argv[1]) - 1900;
	t.tm_mon = atoi(argv[2]) - 1;
	t.tm_mday = atoi(argv[3]);
	t.tm_hour = 0;
	t.tm_min = 0;
	t.tm_sec = 0;
	t.tm_isdst = 1;
	printf("%d\n", mktime(&t));
	return 0;
}
