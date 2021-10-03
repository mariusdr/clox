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

void printObject(Value value) {
  switch (OBJ_TYPE(value)) {
    case OBJ_STRING:
      printf("%s", AS_CSTRING(value));
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
    case VAL_OBJ: {
      ObjString *aStr = AS_STRING(a);
      ObjString *bStr = AS_STRING(b);
      return aStr->length == bStr->length &&
             memcmp(aStr->chars, bStr->chars, aStr->length) == 0;
    }
    default:
      return false;
  }
}