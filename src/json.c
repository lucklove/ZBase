#include "json.h"
#include "mem.h"
//#include "debug.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

static struct JsonPair soakPair(const char *);
static mem_t soakJson(const char *);
static mem_t soakPairs(const char *);
static mem_t soakPairList(const char *);
static mem_t soakString(const char *);

#define MAX_ERROR_OFFSET 20

//static int json_error;
static const char*string_start;			//for out put error msg

static const char *
find_error_line_start(const char *error_point, unsigned int *line_count)
{
	*line_count = 1;
	const char *this_line = string_start;
	const char *tmp = string_start;
	while(tmp < error_point) {
		if(*tmp == '\n') {
			++(*line_count);
			this_line = tmp + 1;
		}
		++tmp;
	}
	return this_line;
}

static const char *
find_error_line_end(const char *error_point)
{
	const char *this_line = error_point;
	while(*this_line != '\0' && *this_line != '\n')
		++this_line;
	return this_line;
}

static void
point_error(const char *error_point)
{
	unsigned int line_num = 0;
	const char *line_start = find_error_line_start(error_point, &line_num);
	const char *line_end = find_error_line_end(error_point);
	if(error_point - line_start > MAX_ERROR_OFFSET)
		line_start = error_point - MAX_ERROR_OFFSET;
	if(line_end - error_point > MAX_ERROR_OFFSET)
		line_end = error_point + MAX_ERROR_OFFSET;
//	printf("on line %d:\n", line_num);
	fwrite(line_start, line_end - line_start, 1, stdout);
	printf("\n");
	for(int i = 0; i < error_point - line_start; i++)
		printf(" ");
	printf("^\n");
	return;
}
 	
static bool
check_num(const char *string)
{
	while(*string != ' ' && *string != '}') {
		if(*string > '9' || *string < '0') {
			return false;
		}
		++string;
	}
	return true;
}

static const char *
find_pair(const char *string, char left, char right)
{
	int left_count = 1;
	assert(*string == left);
	while(*string != '\0' && left_count != 0) {
		++string;
		if(*string == left)
			++left_count;
		if(*string == right)
			--left_count;
	}
	if(left_count != 0) {
		printf("JSON ERROR: unexpect end.\n");
		point_error(string);
		return NULL;
	}
	return string;
}

static mem_t
soakString(const char *string)
{
	assert(*string == '\"');
	++string;
	const char *tmp = strchr(string, '\"');
	if(tmp == NULL) {
		printf("JSON ERROR:expect another '\"':\n");
		point_error(string);
		return makeMem(0);
	}
	mem_t retval = makeMem(tmp - string + 1);
	memCpy(&retval, string, tmp - string);
	return retval;
}
	
static struct JsonPair
soakPair(const char *string)
{
	const char *tmp = string;
	while(*(++tmp) == ' ');
	if(*tmp == '}')
		return (struct JsonPair){ makeMem(0), makeMem(0), JSON_NONE };
	struct JsonPair pair;
	while(*(++string) == ' ');
	if(*string != '\"') {
		printf("JSON ERROR:expect '\"':\n");
		point_error(string);
		return (struct JsonPair){ makeMem(0), makeMem(0), JSON_NONE };
	}
	pair.key = soakString(string);
	if(getMemIndex(pair.key) == 0) 
		return (struct JsonPair){ makeMem(0), makeMem(0), JSON_NONE };
	string = strchr(++string, '\"');
	assert(string != NULL);
	while(*(++string) == ' ');
	if(*string != ':') {
		printf("JSON ERROR:expect ':'\n");
		point_error(string);
		return (struct JsonPair){ makeMem(0), makeMem(0), JSON_NONE };
	}
	while(*(++string) == ' ');
	if(*string == '[') {
		pair.val = soakJson(string);
		pair.type = JSON_ARRAY;
	} else if(*string == '{') {
		pair.val = soakJson(string);
		pair.type = JSON_PAIR;
	} else if(*string == '\"') {
		pair.val = soakString(string);
		pair.type = JSON_STRING;
	} else if(strncmp(string, "true", 4) == 0 
		&& (string[4] == ' ' || string[4] == '}'))  {
		pair.val = makeMem(5);
		memCpy(&pair.val, string, 4);
		pair.type = JSON_BOOL;
	} else if(strncmp(string, "false", 5) == 0 
		&& (string[5] == ' ' || string[5] == '}'))  {
		pair.val = makeMem(6);
		memCpy(&pair.val, string, 5);
		pair.type = JSON_BOOL;
	} else if(strncmp(string, "null", 4) == 0
		&& (string[4] == ' ' || string[4] == '}'))  {
		pair.val = makeMem(5);
		memCpy(&pair.val, string, 4);
		pair.type = JSON_NONE;
	} else if(check_num(string)) {
		tmp = string;
		while(*tmp != ' ' && *tmp != '}') ++tmp;
		pair.val = makeMem(tmp - string + 1);
		memCpy(&pair.val, string, tmp - string);
		pair.type = JSON_NUMBER;
	} else {
		printf("JSON ERROR:unexpect '%c'\n", *string);
		point_error(string);
		return (struct JsonPair){ makeMem(0), makeMem(0), JSON_NONE };
	}
	return pair;
}


static mem_t
soakPairList(const char *string)
{
	int have_pair = true;
	mem_t retval = makeMem(sizeof(struct JsonPair));
	while(have_pair) {
		while(*string == ' ')
			++string;
		if(*string != '{') {
			printf("JSON ERROR: expect '{'\n");
			point_error(string);
			return makeMem(0);
		}
		SET_TYPE_MEM(&retval, struct JsonPair, GET_ITEM_NUM(retval, struct JsonPair), soakPair(string));
		if((string = find_pair(string, '{', '}')) == NULL)
			return makeMem(0);
		++string;
		while(*string == ' ') ++string;
		if(*string == ']') {
			have_pair = false;
		} else if(*string == ',') {
			++string;
			continue;
		} else {
			printf("JSON ERROR: unexpect '%c'\n", *string);
			point_error(string);
			return makeMem(0);
		}
	}
	return retval;
}
		
static mem_t
soakPairs(const char *string)
{
	const char *tmp = string;
	while(*(++tmp) == ' ');
	if(*tmp == ']')
		return makeMem(0);
	++string;
	return soakPairList(string);
}

static mem_t
soakJson(const char *string)
{
	while(*string == ' ' && *string != '\0')
		string++;
	if(*string == '\0') {
		printf("JSON ERROR: unexpect end.\n");
		point_error(string);
	}
	if(*string == '[') {
		return soakPairs(string);
	} else if(*string == '{') {
		mem_t retval = makeMem(sizeof(struct JsonPair));
		struct JsonPair pair = soakPair(string);
		SET_TYPE_MEM(&retval, struct JsonPair, 0, pair);
		return retval;
	} 
	printf("JSON ERROR: expect '{' or '[':\n");
	point_error(string);
	return makeMem(0);
}
	
json_t
makeJson(const char *string)
{
	string_start = string;
	while(*(string) == ' ') ++string;
	if(*string == '{') {
		return (json_t){ makeMem(0), soakJson(string), JSON_PAIR };
	} else if(*string == '[') {
		return (json_t){ makeMem(0), soakJson(string), JSON_ARRAY };
	} else {
		return (json_t){ makeMem(0), makeMem(0), JSON_NONE };
	}
}

static void
dumpVal(struct JsonPair *p)
{
	printf("{");
	switch(p->type) {
		case JSON_NONE:
			break;
		case JSON_NUMBER:
		case JSON_BOOL:
			printf("\"%s\":%s", (char *)getMemPtr(&p->key, 0, 0), (char *)getMemPtr(&p->val, 0, 0));
			break;
		case JSON_STRING:
			printf("\"%s\":\"%s\"", (char *)getMemPtr(&p->key, 0, 0), (char *)getMemPtr(&p->val, 0, 0));
			break;
		case JSON_ARRAY:
			printf("\"%s\":", (char *)getMemPtr(&p->key, 0, 0));
			printf("[");
			for(int i = 0; i < GET_ITEM_NUM(p->val, struct JsonPair); i++) {
				dumpVal(GET_TYPE_MEM(&p->val, struct JsonPair, i));
				if(i != GET_ITEM_NUM(p->val, struct JsonPair) - 1)
					printf(",");
			}
			printf("]");
			break;
		case JSON_PAIR:
			printf("\"%s\":", (char *)getMemPtr(&p->key, 0, 0));
			printf("{");
			dumpVal(GET_TYPE_MEM(&p->val, struct JsonPair, 0));
			printf("}");
			break;
		default:
			printf("error");
			break;
	}
	printf("}");
}

void
dumpJson(json_t json)
{
	if(json.type == JSON_ARRAY)
		printf("[");
	for(int i = 0; i < GET_ITEM_NUM(json.val, struct JsonPair); i++) {
		dumpVal(GET_TYPE_MEM(&json.val, struct JsonPair, i));
		if(i != GET_ITEM_NUM(json.val, struct JsonPair) - 1)
			printf(",");
	}
	if(json.type == JSON_ARRAY)
		printf("]");
	printf("\n");
}
