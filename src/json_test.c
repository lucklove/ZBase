#include "json.h"
#include "debug.h"
#include <stdio.h>

int
main(int argc, char *argv[])
{
	memDebugInit();
	FILE *fp = fopen("json.txt", "r");
	json_t json = jsonLoad(fp);
	printf("-----------------------------------\n");
	jsonDump(json, stdout);
	printf("-----------------------------------\n");
	printf("json type = %d\n", jsonGetType(json));
	if(jsonGetType(json) == JSON_ARRAY) {
		for(int i = 0; i < jsonGetArrayLen(json); ++i) {
			json_t j = jsonGetVal(json, i);
			printf("type = %d\n", jsonGetType(j));
			printf("key = %s\n", jsonGetKey(j));
			printf("content:%s\n", jsonGetContent(j));
		}
	}
	memCheckLeak(stdout);
	memDebugRelease();
	return 0;
}
