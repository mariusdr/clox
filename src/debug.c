#include <stdio.h>

#include "debug.h"
#include "common.h"

void disassembleChunk(Chunk* chunk, const char* name) 
{
    printf("== %s ==\n", name);

    for (int offs = 0; offs < chunk->count;) 
    {
        offs = disassembleInstruction(chunk, offs);
    }
}

static int simpleInstruction(const char* name, int offset) 
{
    printf("%s\n", name);
    return offset + 1;
}

int disassembleInstruction(Chunk* chunk, int offset) 
{
    printf("%04d ", offset);

    uint8_t inst = chunk->code[offset];
    switch (inst)
    {
    case OP_RETURN:
        return simpleInstruction("OP_RETURN", offset);
    
    default:
        printf("unknown opcode %d\n", inst);
        return offset + 1;
    }
}