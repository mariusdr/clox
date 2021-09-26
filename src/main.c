#include <stdlib.h>

#include "chunk.h"
#include "debug.h"

int main(int argc, char** argv) 
{
    Chunk chunk;
    initChunk(&chunk);
    writeChunk(&chunk, OP_RETURN);
    disassembleChunk(&chunk, "main");
    freeChunk(&chunk);
    return EXIT_SUCCESS;
}
