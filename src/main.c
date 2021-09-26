#include <stdlib.h>

#include "chunk.h"
#include "debug.h"

int main(int argc, char** argv) 
{
    Chunk chunk;
    initChunk(&chunk);
    writeChunk(&chunk, OP_RETURN, 0);

    int constant = addConstant(&chunk, 1.2);
    writeChunk(&chunk, OP_CONSTANT, 1);
    writeChunk(&chunk, constant, 2);

    constant = addConstant(&chunk, 1.337);
    writeChunk(&chunk, OP_CONSTANT, 3);
    writeChunk(&chunk, constant, 4);
    
    constant = addConstant(&chunk, -0.0042);
    writeChunk(&chunk, OP_CONSTANT, 5);
    writeChunk(&chunk, constant, 6);

    disassembleChunk(&chunk, "main");
    freeChunk(&chunk);

    return EXIT_SUCCESS;
}
