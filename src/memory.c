#include <stdlib.h>

#include "common.h"
#include "memory.h"
#include "object.h"
#include "vm.h"

void *reallocate(void *ptr, size_t oldSize, size_t newSize) {
  if (newSize == 0) {
    free(ptr);
    return NULL;
  }

  void *result = realloc(ptr, newSize);
  if (!result) {
    exit(EXIT_FAILURE);
  }

  return result;
}

static void freeObject(Obj *object) {
  switch (object->type) {
    case OBJ_STRING:
      ObjString *str = (ObjString *)object;
      FREE_ARRAY(char, str->chars, str->length + 1);
      FREE(ObjString, object);
      break;
  }
}

void freeObjects() {
  Obj *object = vm.objects;
  while (object != NULL) {
    Obj *next = object->next;
    freeObject(object);
    object = next;
  }
}