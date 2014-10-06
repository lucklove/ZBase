#ifndef OBJECT_H
#define OBJECT_H

#include <stdio.h>
#include <stdlib.h>

typedef enum {
	OBJ_BADVAL,
	OBJ_INT,
	OBJ_CHAR,
	OBJ_STR,
	OBJ_PAIR
} ObjectType;

typedef struct ObjectType {
	ObjectType 	type;
	union {
		int	intval;
		char	charval;
		struct {
			char *strData;
			size_t strSize;
		};
		struct {
			struct ObjectType *first;
			struct ObjectType *second;
		};
	};
} Object, *ObjectPtr;

Object 	makeInt(int);
Object 	makeChar(char);
Object	makeStr(char *);
Object  makePair(Object, Object);
int	getInt(Object);
char	getChar(Object);
char*	getStr(Object);
int	getStrLen(Object);
int	objectDump(Object, void *);
int	objectIsInt(Object);
int	objectIsChar(Object);
int	objectIsStr(Object);
void	destroyObject(ObjectPtr);
void	setInt(ObjectPtr, int);
void	setChar(ObjectPtr, char);
void	setStr(ObjectPtr, char *);

#endif
