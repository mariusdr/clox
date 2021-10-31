#include <stdio.h>
#include <string.h>

#include "memory.h"
#include "object.h"
#include "value.h"
#include "vm.h"
#include "table.h"

static ObjString *allocateString(char*, int, uint32_t);
static Obj *allocateObject(size_t, ObjType);

static uint32_t hashString(const char *key, int length) {
  uint32_t hash = 2166136261u;
  for (int i = 0; i < length; ++i) {
    hash ^= (uint8_t)key[i];
    hash *= 16777619;
  }
  return hash;
}

ObjString *copyString(const char *chars, int length) {
  char *heapChars = ALLOCATE(char, length + 1);
  memcpy(heapChars, chars, length);
  heapChars[length] = '\0';
  uint32_t hash = hashString(chars, length);

  // The vm's runtime stores a string "abc" only once in its strings table,
  // every instance of "abc" receives the same pointer to that string here.
  // So when we want to check if two strings are equal we only need to check
  // for pointer equality.
  ObjString *interned = tableFindString(&vm.strings, chars, length, hash);
  if (interned) {
    FREE_ARRAY(char, heapChars, length + 1);
    return interned;
  }

  return allocateString(heapChars, length, hash);
}

ObjString *takeString(char* chars, int length) {
  uint32_t hash = hashString(chars, length);
  return allocateString(chars, length, hash);
}

static Obj *allocateObject(size_t size, ObjType type) {
  Obj *object = (Obj *)reallocate(NULL, 0, size);
  object->type = type;
  vm.objects = object;
  return object;
}

#define ALLOCATE_OBJ(type, objectType) \
  (type *)allocateObject(sizeof(type), objectType)

static ObjString *allocateString(char* chars, int length, uint32_t hash) {
  ObjString *str = ALLOCATE_OBJ(ObjString, OBJ_STRING);
  str->length = length;
  str->chars = chars;
  str->hash = hash;
  tableSet(&vm.strings, str, NIL_VAL);
  return str;
}

ObjFunction *newFunction() {
  ObjFunction *function = ALLOCATE_OBJ(ObjFunction, OBJ_FUNCTION);
  function->arity = 0;
  function->name = NULL;
  initChunk(&function->chunk);
  return function;
}

ObjNative *newNative(NativeFn function) {
  ObjNative *native = ALLOCATE_OBJ(ObjNative, OBJ_NATIVE);
  native->function = function;
  return native;
}