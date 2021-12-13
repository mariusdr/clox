#include <stdio.h>
#include <string.h>

#include "common.h"
#include "memory.h"
#include "value.h"
#include "object.h"

void initValueArray(ValueArray *array) {
  array->count = 0;
  array->capacity = 0;
  array->values = NULL;
}

void freeValueArray(ValueArray *array) {
  FREE_ARRAY(Value, array->values, 0);
  initValueArray(array);
}

void writeValueArray(ValueArray *array, Value value) {
  if (array->capacity < array->count + 1) {
    int oldCapacity = array->capacity;
    array->capacity = GROW_CAPACITY(oldCapacity);
    array->values =
        GROW_ARRAY(Value, array->values, oldCapacity, array->capacity);
  }
  array->values[array->count] = value;
  array->count++;
}

static void printFunction(ObjFunction *function) {
  if (!function->name) {
    printf("<script>");
  } else {
    printf("<fn %s>", function->name->chars);
  }
}

void printObject(Value value) {
  switch (OBJ_TYPE(value)) {
    case OBJ_STRING:
      printf("%s", AS_CSTRING(value));
      break;
    case OBJ_FUNCTION:
      printFunction(AS_FUNCTION(value));
      break;
    case OBJ_NATIVE:
      printf("<native fn>");
      break;
    case OBJ_CLOSURE:
      printFunction(AS_CLOSURE(value)->function);
      break;
    case OBJ_UPVALUE: 
      printf("upvalue");
      break;
  }
}

void printValue(Value value) { 
  switch (value.type) {
    case VAL_BOOL:
      printf(AS_BOOL(value) ? "true" : "false");
      break;
    case VAL_NIL:
      printf("nil");
      break;
    case VAL_NUMBER:
      printf("%g", AS_NUMBER(value)); 
      break;
    case VAL_OBJ:
      printObject(value);
      break;
    default:
      return;
  }
}

bool valuesEqual(Value a, Value b) {
  if (a.type != b.type) {
    return false;
  }

  switch (a.type) {
    case VAL_BOOL:
      return AS_BOOL(a) == AS_BOOL(b);
    case VAL_NIL:
      return true;
    case VAL_NUMBER:
      return AS_NUMBER(a) == AS_NUMBER(b);
    case VAL_OBJ:
      return AS_OBJ(a) == AS_OBJ(b);
    default:
      return false;
  }
}