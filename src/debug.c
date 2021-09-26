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

static int constantInstruction(const char* name, Chunk* chunk, int offset) 
{
    uint8_t constant = chunk->code[offset + 1];
    printf("%-16s %4d '", name, constant);
    printValue(chunk->constants.values[constant]);
    printf("'\n");
    return offset + 2;
}

int disassembleInstruction(Chunk* chunk, int offset) 
{
    printf("%04d ", offset);

    if (offset > 0 && chunk->lines[offset] == chunk->lines[offset - 1])
    {
        printf("   | ");
    } 
    else 
    {
        printf("%4d ", chunk->lines[offset]);
    }

    uint8_t inst = chunk->code[offset];
    switch (inst)
    {
    case OP_CONSTANT:
        return constantInstruction("OP_CONSTANT", chunk, offset);
    case OP_RETURN:
        return simpleInstruction("OP_RETURN", offset);
    
    default:
        printf("unknown opcode %d\n", inst);
        return offset + 1;
    }
}