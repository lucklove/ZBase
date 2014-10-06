#include "object.h"
#include <string.h>
#include <stdlib.h>

inline Object
makeInt(int val)
{ 
	Object obj = { .type = OBJ_INT, .intval = val };
	return obj;
}

inline Object
makeChar(char val)
{
	Object obj = { .type = OBJ_CHAR, .charval = val };
	return obj;
}

inline Object
makeStr(char *str)
{
	Object obj;
	int size = strlen(str) + 1;
	obj.strData = malloc(size);
	if(obj.strData == NULL) {
		obj.type = OBJ_BADVAL;
		return obj;
	}
	obj.type = OBJ_STR;
	memcpy(obj.strData, str, size);
	return obj;
}

inline Object
makePair(Object first, Object second)
{
	Object obj;
	ObjectPtr first_ptr = malloc(sizeof(Object));
	if(first_ptr == NULL) {
		obj.type = OBJ_BADVAL;
		return obj;
	}
	ObjectPtr second_ptr = malloc(sizeof(Object));
	if(second_ptr == NULL) {
		obj.type = OBJ_BADVAL;
		return obj;
	}
	obj.type = OBJ_PAIR;
	obj.first = first_ptr;
	obj.second = second_ptr;
	return obj;
}

inline Object *
getFirst(Object obj)
{
	return obj.first;
}

inline Object *
getSecond(Object obj)
{
	return obj.second;
}

inline int
getInt(Object obj)
{
	return obj.intval;
}

inline char
getChar(Object obj)
{
	return obj.charval;
}

inline char *
getStr(Object obj)
{
	return obj.strData;
}

inline int
getStrLen(Object str)
{
	return strlen(str.strData);
}

inline int
objectIsInt(Object obj)
{
	return obj.type == OBJ_INT;
}

inline int
objectIsStr(Object obj)
{
	return obj.type == OBJ_STR;
}
	
inline int
objectIsChar(Object obj)
{
	return obj.type == OBJ_CHAR;
}

inline int
objectDump(Object obj, void *user_buf)
{
	switch(obj.type) {
		case OBJ_BADVAL:
			return -1;
		case OBJ_INT:
			*(int *)user_buf = obj.intval;
			return 0;
		case OBJ_CHAR:
			*(char *)user_buf = obj.charval;
			return 0;
		case OBJ_STR:
			*(void **)user_buf = obj.strData;
			return 0;
		default:
			return -1;
	}
}

void
destroyObject(ObjectPtr p_obj)
{
	switch(p_obj->type) {
		case OBJ_BADVAL: break;
		case OBJ_STR:
			free(p_obj->strData);
			break;
		case OBJ_PAIR:
			destroyObject(p_obj->first);
			destroyObject(p_obj->second);
			break;
		default:
			break;
	}
	p_obj->type = OBJ_BADVAL;
}

void
setInt(ObjectPtr p_obj, int val)
{
	destroyObject(p_obj);
	*p_obj = makeInt(val);
}

void
setChar(ObjectPtr p_obj, char val)
{
	destroyObject(p_obj);
	*p_obj = makeChar(val);
}

void
setStr(ObjectPtr p_obj, char *val)
{
	destroyObject(p_obj);
	*p_obj = makeStr(val);
}
