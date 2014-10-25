#pragma once

#include "mem.h"
#include <stdio.h>

enum JsonType { JSON_NONE, JSON_STRING, JSON_BOOL, JSON_NUMBER, JSON_ARRAY, JSON_PAIR };

typedef struct JsonPair {
	mem_t key;
	mem_t val;
	enum JsonType type;
} json_t;

json_t makeJson(const char *, const char *);
json_t jsonLoad(FILE *);
void jsonDump(json_t, FILE *);
enum JsonType jsonGetType(json_t);
const char *jsonGetKey(json_t);
const char *jsonGetContent(json_t);
json_t jsonGetVal(json_t, unsigned int);
unsigned int jsonGetArrayLen(json_t);
