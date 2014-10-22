#pragma once

#include "mem.h"

enum JsonType { JSON_NONE, JSON_STRING, JSON_BOOL, JSON_NUMBER, JSON_ARRAY, JSON_PAIR };

typedef struct JsonPair {
	mem_t key;
	mem_t val;
	enum JsonType type;
} json_t;

json_t makeJson(const char *);
void dumpJson(json_t);
