#include <stdlib.h>

#include "common.h"
#include "memory.h"

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