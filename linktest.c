#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
int main()
{
	char a[100]="";
	strcat(a,"wget -r -v -O ");
	strcat(a,"https://www.chitkara.edu.in/arts/tectureatives/ation/cture/");
	printf("%d",system(a));
}
