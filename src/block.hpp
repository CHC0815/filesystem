#pragma once

#define BLOCK_SIZE 32
#define MIN_BLOCK_SIZE 1024

using word_t = intptr_t;

// 32 bytes large
struct Block
{
    size_t header;
    Block* next;
    word_t data[1];
};