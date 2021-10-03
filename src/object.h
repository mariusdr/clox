#ifndef CLOX_OBJECT_H
#define CLOX_OBJECT_H

#include "common.h"
#include "value.h"

typedef enum {
  OBJ_STRING,
} ObjType;

#define OBJ_TYPE(object) (AS_OBJ(object)->type)

struct Obj {
  ObjType type;
  Obj *next;
};

struct ObjString {
  Obj obj;
  int length;
  char *chars;
};

ObjString *takeString(char* chars, int length);
ObjString *copyString(const char *chars, int length);

static inline bool isObjType(Value value, ObjType type) {
  return IS_OBJ(value) && AS_OBJ(value)->type == type;
}

#define IS_STRING(value) isObjType((value), OBJ_STRING)

#define AS_STRING(value) ((ObjString *)AS_OBJ((value)))
#define AS_CSTRING(value) (((ObjString *)AS_OBJ((value)))->chars)

#endif