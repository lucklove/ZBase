#include "json.h"
#include "mem.h"
//#include "debug.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

static struct JsonPair soakPair(const char **);
static mem_t soakJson(const char **);
static mem_t soakPairs(const char **);
static mem_t soakPairList(const char **);
static mem_t soakString(const char **);

#define MAX_ERROR_OFFSET 20

//static int json_error;
static const char *string_start;			//for out put error msg

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
	printf("on line %d:\n", line_num);
	fwrite(line_start, line_end - line_start, 1, stdout);
	printf("\n");
	for(int i = 0; i < error_point - line_start; i++)
		printf(" ");
	printf("^\n");
	return;
}

static bool check_num(const char *str)
{
    int checkE = 0;
    int i = 0;
    int Have_number = 0;
    int checkPoint = 0;
    while(str[i]!='}'&&str[i]!=' ')
    {
        if(str[i]>='0'&&str[i]<='9')
        {
            Have_number = 1;
            i++;
        } else if(str[i]=='E'||str[i]=='e') {
            if(str[i+1]!='}'&&str[i+1]!=' ')  //E和e只能出现在数字后，并且只能出现一次
            {
                if(Have_number==1&&checkE==0)
                {
                    if(str[i+1]=='.')
                    return false;
                    i++;
                    checkE++;
                }
                else
                    return false ;
            }
            else
                return false;
        }
        else if(str[i]=='.')
        {
            if(checkPoint==1&&Have_number==1)
            {
                i++;
            }
            else if(checkPoint==0)
            {
                checkPoint++;
                i++;
            }
            else
                return false;
        }
else return false;
    }
    if(Have_number==0||str[i-1]=='.')
        return false;
    else
        return true;
}

static mem_t
soakString(const char **string)
{
	assert(**string == '\"');
	++(*string);
	const char *tmp = strchr(*string, '\"');
	if(tmp == NULL) {
		printf("JSON ERROR:expect another '\"':\n");
		point_error(*string);
		exit(-1);
		return makeMem(0);
	}
	mem_t retval = makeMem(tmp - *string + 1);
	memCpy(&retval, *string, tmp - *string);
	*string = ++tmp;
	return retval;
}
	
static struct JsonPair
soakPair(const char **string)
{
	const char *tmp = *string;
	while(*(++tmp) == ' ' || *tmp == '\n' || *tmp == '\t');
	if(*tmp == '}')
		return (struct JsonPair){ makeMem(0), makeMem(0), JSON_NONE };
	struct JsonPair pair;
	while(*(++(*string)) == ' ' || **string == '\n' || **string == '\t');
	if(**string != '\"') {
		printf("JSON ERROR:expect '\"':\n");
		point_error(*string);
		exit(-1);
		return (struct JsonPair){ makeMem(0), makeMem(0), JSON_NONE };
	}
	pair.key = soakString(string);
	if(getMemIndex(pair.key) == 0) 
		return (struct JsonPair){ makeMem(0), makeMem(0), JSON_NONE };
	while((**string) == ' ' || **string == '\n' || **string == '\t') ++(*string);
	if(**string != ':') {
		printf("JSON ERROR:expect ':'\n");
		point_error(*string);
		exit(-1);
		return (struct JsonPair){ makeMem(0), makeMem(0), JSON_NONE };
	}
	while(*(++(*string)) == ' ' || **string == '\n' || **string == '\t');
	if(**string == '[') {
		pair.val = soakJson(string);
		pair.type = JSON_ARRAY;
	} else if(**string == '{') {
		pair.val = soakJson(string);
		pair.type = JSON_PAIR;
	} else if(**string == '\"') {
		pair.val = soakString(string);
		pair.type = JSON_STRING;
	} else if(strncmp(*string, "true", 4) == 0 
		&& ((*string)[4] == ' ' || (*string)[4] == '}'))  {
		pair.val = makeMem(5);
		memCpy(&pair.val, *string, 4);
		*string += 4;
		pair.type = JSON_BOOL;
	} else if(strncmp(*string, "false", 5) == 0 
		&& ((*string)[5] == ' ' || (*string)[5] == '}'))  {
		pair.val = makeMem(6);
		memCpy(&pair.val, *string, 5);
		*string += 5;
		pair.type = JSON_BOOL;
	} else if(strncmp(*string, "null", 4) == 0
		&& ((*string)[4] == ' ' || (*string)[4] == '}'))  {
		pair.val = makeMem(5);
		memCpy(&pair.val, *string, 4);
		*string += 4;
		pair.type = JSON_NONE;
	} else if(check_num(*string)) {
		tmp = *string;
		while(*tmp != ' ' && *tmp != '}' && *tmp != '\n' && *tmp != '\t') ++tmp;
		pair.val = makeMem(tmp - *string + 1);
		memCpy(&pair.val, *string, tmp - *string);
		*string = tmp;
		pair.type = JSON_NUMBER;
	} else {
		printf("JSON ERROR: expect '[' or '{' or '\"' or true or false or null or NUMBER\n");
		point_error(*string);
		exit(-1);
		return (struct JsonPair){ makeMem(0), makeMem(0), JSON_NONE };
	}
	while(**string == ' ' || **string == '\n' || **string == '\t') ++(*string);
	if(**string != '}') {
		printf("JSON ERROR: expect '}'\n");
		point_error(*string);
		exit(-1);
	}
	return pair;
}


static mem_t
soakPairList(const char **string)
{
	int have_pair = true;
	mem_t retval = makeMem(sizeof(struct JsonPair));
	while(have_pair) {
		while(**string == ' ' || **string == '\n' || **string == '\t')
			++(*string);
		if(**string != '{') {
			printf("JSON ERROR: expect '{'\n");
			point_error(*string);
			exit(-1);
			return makeMem(0);
		}
		SET_TYPE_MEM(&retval, struct JsonPair, GET_ITEM_NUM(retval, struct JsonPair), soakPair(string));
		++(*string);
		while(**string == ' ' || **string == '\n' || **string == '\t') ++(*string);
		if(**string == ']') {
			have_pair = false;
		} else if(**string == ',') {
			++(*string);
			continue;
		} else {
			printf("JSON ERROR: expect '] or ','");
			point_error(*string);
			exit(-1);
			return makeMem(0);
		}
	}
	++(*string);
	return retval;
}
		
static mem_t
soakPairs(const char **string)
{
	const char *tmp = *string;
	while(*(++tmp) == ' ' || *tmp == '\n' || *tmp == '\t');
	if(*tmp == ']')
		return makeMem(0);
	++(*string);
	return soakPairList(string);
}

static mem_t
soakJson(const char **string)
{
	while(**string == ' ' || **string == '\n' || **string == '\t')
		++(*string);
	if(**string == '\0') {
		printf("JSON ERROR: unexpect end.\n");
		point_error(*string);
		exit(-1);
	}
	if(**string == '[') {
		return soakPairs(string);
	} else if(**string == '{') {
		mem_t retval = makeMem(sizeof(struct JsonPair));
		struct JsonPair pair = soakPair(string);
		SET_TYPE_MEM(&retval, struct JsonPair, 0, pair);
		return retval;
	} 
	printf("JSON ERROR: expect '{' or '['\n");
	exit(-1);
	point_error(*string);
	return makeMem(0);
}
	
json_t
makeJson(const char *key, const char *val)
{
	string_start = val;
	mem_t key_mem;
	if(key != NULL) {
		key_mem = makeMem(strlen(key));
		memCpy(&key_mem, key, strlen(key));
	} else {
		key_mem = makeMem(0);
	}
	while(*(val) == ' ' || *val == '\n' || *val == '\t') ++val;
	if(*val == '{') {
		return (json_t){ key_mem, soakJson(&val), JSON_PAIR };
	} else if(*val == '[') {
		return (json_t){ key_mem, soakJson(&val), JSON_ARRAY };
	} else {
		return (json_t){ key_mem, makeMem(0), JSON_NONE };
	}
}

static void
dumpVal(struct JsonPair *p, FILE *file)
{
	printf("{");
	switch(p->type) {
		case JSON_NONE:
			break;
		case JSON_NUMBER:
		case JSON_BOOL:
			fprintf(file, "\"%s\":%s", (char *)getMemPtr(&p->key, 0, 0), (char *)getMemPtr(&p->val, 0, 0));
			break;
		case JSON_STRING:
			fprintf(file, "\"%s\":\"%s\"", (char *)getMemPtr(&p->key, 0, 0), (char *)getMemPtr(&p->val, 0, 0));
			break;
		case JSON_ARRAY:
			fprintf(file, "\"%s\":", (char *)getMemPtr(&p->key, 0, 0));
			fprintf(file, "[");
			for(int i = 0; i < GET_ITEM_NUM(p->val, struct JsonPair); i++) {
				dumpVal(GET_TYPE_MEM(&p->val, struct JsonPair, i), file);
				if(i != GET_ITEM_NUM(p->val, struct JsonPair) - 1)
					fprintf(file, ",");
			}
			fprintf(file, "]");
			break;
		case JSON_PAIR:
			fprintf(file, "\"%s\":", (char *)getMemPtr(&p->key, 0, 0));
			fprintf(file, "{");
			dumpVal(GET_TYPE_MEM(&p->val, struct JsonPair, 0), file);
			fprintf(file, "}");
			break;
		default:
			fprintf(file, "error");
			break;
	}
	fprintf(file, "}");
}

void
jsonDump(json_t json, FILE *file)
{
	if(json.type == JSON_ARRAY)
		fprintf(file, "[");
	for(int i = 0; i < GET_ITEM_NUM(json.val, struct JsonPair); i++) {
		dumpVal(GET_TYPE_MEM(&json.val, struct JsonPair, i), file);
		if(i != GET_ITEM_NUM(json.val, struct JsonPair) - 1)
			fprintf(file, ",");
	}
	if(json.type == JSON_ARRAY)
		fprintf(file, "]");
	fprintf(file, "\n");
}

json_t
jsonLoad(FILE *file)
{
	char ch;
	mem_t mem = makeMem(256);
	while((ch = getc(file)) != EOF)
		SET_TYPE_MEM(&mem, char, GET_ITEM_NUM(mem, char), ch);
	SET_TYPE_MEM(&mem, char, GET_ITEM_NUM(mem, char), '\0');
	json_t ret = makeJson(NULL, getMemPtr(&mem, 0, 0));
	destroyMem(mem);
	return ret;
}

enum JsonType
jsonGetType(json_t json)
{
	return json.type;
}

const char *
jsonGetContent(json_t json)
{
	return getMemPtr(&json.val, 0, 0);
}

json_t
jsonGetVal(json_t json, unsigned int index)
{
	return *(GET_TYPE_MEM(&json.val, json_t, index));
}

unsigned int
jsonGetArrayLen(json_t json)
{
	if(jsonGetType(json) != JSON_ARRAY)
		return 0;
	return GET_ITEM_NUM(json.val, json_t);
}

const char *
jsonGetKey(json_t json)
{
	if(getMemIndex(json.key) == 0)
		return NULL;
	return getMemPtr(&json.key, 0, 0);
}
