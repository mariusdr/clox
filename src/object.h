#ifndef CLOX_OBJECT_H
#define CLOX_OBJECT_H

#include "common.h"
#include "value.h"
#include "chunk.h"
#include "table.h"

typedef enum {
  OBJ_FUNCTION,
  OBJ_NATIVE,
  OBJ_STRING,
  OBJ_CLOSURE,
  OBJ_UPVALUE,
  OBJ_CLASS,
  OBJ_INSTANCE,
  OBJ_BOUND_METHOD,
} ObjType;

#define OBJ_TYPE(object) (AS_OBJ(object)->type)

#define IS_FUNCTION(value) isObjType(value, OBJ_FUNCTION)
#define IS_NATIVE(value) isObjType(value, OBJ_NATIVE)
#define IS_STRING(value) isObjType((value), OBJ_STRING)
#define IS_CLOSURE(value) isObjType((value), OBJ_CLOSURE)
#define IS_CLASS(value) isObjType((value), OBJ_CLASS)
#define IS_INSTANCE(value) isObjType((value), OBJ_INSTANCE)
#define IS_BOUND_METHOD(value) isObjType((value), OBJ_BOUND_METHOD)

#define AS_FUNCTION(value) ((ObjFunction *)AS_OBJ(value))
#define AS_NATIVE(value) (((ObjNative*)AS_OBJ(value))->function)
#define AS_STRING(value) ((ObjString *)AS_OBJ((value)))
#define AS_CSTRING(value) (((ObjString *)AS_OBJ((value)))->chars)
#define AS_CLOSURE(value) ((ObjClosure *)AS_OBJ((value)))
#define AS_CLASS(value) ((ObjClass *)AS_OBJ(value))
#define AS_INSTANCE(value) ((ObjInstance *)AS_OBJ(value))
#define AS_BOUND_METHOD(value) ((ObjBoundMethod *)AS_OBJ(value))

struct Obj {
  ObjType type;
  bool isMarked;
  Obj *next;
};

typedef struct {
  Obj obj;
  int arity;
  int upvalueCount;
  Chunk chunk;
  ObjString *name;
} ObjFunction;

ObjFunction *newFunction();

typedef Value (*NativeFn) (int argCount, Value *args);

typedef struct {
  Obj obj;
  NativeFn function;
} ObjNative;

ObjNative *newNative(NativeFn function);

struct ObjString {
  Obj obj;
  int length;
  char *chars;
  uint32_t hash;
};

ObjString *takeString(char* chars, int length);
ObjString *copyString(const char *chars, int length);

typedef struct ObjUpvalue {
  Obj obj;
  Value *location;
  Value closed;
  struct ObjUpvalue *next;
} ObjUpvalue;

ObjUpvalue *newUpvalue(Value *slot);

typedef struct {
  Obj obj;
  ObjFunction *function;
  ObjUpvalue **upvalues;
  int upvalueCount;
} ObjClosure;

ObjClosure *newClosure(ObjFunction *function);

static inline bool isObjType(Value value, ObjType type) {
  return IS_OBJ(value) && AS_OBJ(value)->type == type;
}

typedef struct {
  Obj obj;
  ObjString *name;
  Table methods;
} ObjClass;

ObjClass *newClass(ObjString *name);

typedef struct {
  Obj obj;
  ObjClass *klass;
  Table fields;
} ObjInstance;

ObjInstance *newInstance(ObjClass *klass);

typedef struct {
  Obj obj;
  Value receiver;
  ObjClosure *method;
} ObjBoundMethod;

ObjBoundMethod *newBoundMethod(Value receiver, ObjClosure *method);

#endif