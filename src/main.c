#include <stdlib.h>

#include "chunk.h"
#include "debug.h"
#include "vm.h"

int main(int argc, char** argv) 
{
    initVM();

    Chunk chunk;
    initChunk(&chunk);

    int constant = addConstant(&chunk, 1.2);
    writeChunk(&chunk, OP_CONSTANT, 1);
    writeChunk(&chunk, constant, 2);
    
    constant = addConstant(&chunk, 3.4);
    writeChunk(&chunk, OP_CONSTANT, 1);
    writeChunk(&chunk, constant, 2);

    writeChunk(&chunk, OP_ADD, 1);

    constant = addConstant(&chunk, 5.6);
    writeChunk(&chunk, OP_CONSTANT, 1);
    writeChunk(&chunk, constant, 2);

    writeChunk(&chunk, OP_DIVIDE, 2);

    writeChunk(&chunk, OP_RETURN, 2);

    //disassembleChunk(&chunk, "main");
    interpret(&chunk);

    freeVM();
    freeChunk(&chunk);
    return EXIT_SUCCESS;
}
