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
  object->isMarked = false;
  object->next = vm.objects;
  vm.objects = object;
#ifdef DEBUG_LOG_GC
  printf("%p allocate %zu for %d\n", (void*)object, size, type);
#endif
  return object;
}

#define ALLOCATE_OBJ(type, objectType) \
  (type *)allocateObject(sizeof(type), objectType)

static ObjString *allocateString(char* chars, int length, uint32_t hash) {
  ObjString *str = ALLOCATE_OBJ(ObjString, OBJ_STRING);
  str->length = length;
  str->chars = chars;
  str->hash = hash;

  // tableSet may reallocate which may cause a GC run, but str is not reachable 
  // as a root object yet because it is not yet in the table!
  // So we need to save it temporarily on the stack and pop it after it was inserted.
  push(OBJ_VAL(str));
  tableSet(&vm.strings, str, NIL_VAL);
  pop();
  return str;
}

ObjFunction *newFunction() {
  ObjFunction *function = ALLOCATE_OBJ(ObjFunction, OBJ_FUNCTION);
  function->arity = 0;
  function->upvalueCount = 0;
  function->name = NULL;
  initChunk(&function->chunk);
  return function;
}

ObjNative *newNative(NativeFn function) {
  ObjNative *native = ALLOCATE_OBJ(ObjNative, OBJ_NATIVE);
  native->function = function;
  return native;
}

ObjClosure *newClosure(ObjFunction *function) {
  ObjUpvalue **upvalues = ALLOCATE(ObjUpvalue*, function->upvalueCount);
  for (int i = 0; i < function->upvalueCount; ++i) {
    upvalues[i] = NULL;
  }

  ObjClosure *closure = ALLOCATE_OBJ(ObjClosure, OBJ_CLOSURE);
  closure->function = function;
  closure->upvalues = upvalues;
  closure->upvalueCount = function->upvalueCount;
  return closure;
}

ObjUpvalue *newUpvalue(Value *slot) {
  ObjUpvalue *upvalue = ALLOCATE_OBJ(ObjUpvalue, OBJ_UPVALUE);
  upvalue->location = slot;
  upvalue->next = NULL;
  upvalue->closed = NIL_VAL;
  return upvalue;
}

ObjClass *newClass(ObjString *name) {
  ObjClass *klass = ALLOCATE_OBJ(ObjClass, OBJ_CLASS);
  klass->name = name;
  initTable(&klass->methods);
  return klass;
}

ObjInstance *newInstance(ObjClass *klass) {
  ObjInstance *instance = ALLOCATE_OBJ(ObjInstance, OBJ_INSTANCE);
  instance->klass = klass;
  initTable(&instance->fields);
  return instance;
}

ObjBoundMethod *newBoundMethod(Value receiver, ObjClosure *method) {
  ObjBoundMethod *bound = ALLOCATE_OBJ(ObjBoundMethod, OBJ_BOUND_METHOD);
  bound->receiver = receiver;
  bound->method = method;
  return bound;
}