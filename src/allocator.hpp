#pragma once

#include "block.hpp"

enum class SearchMode
{
    FirstFit,
    NextFit
};

class Allocator
{
    static Block* heapStart;
    static Block* top;

    //next fit
    static Block *searchStart;

    static Block *largestKnownBlock;

    static auto SearchMode = SearchMode::FirstFit;

    Allocator()
    {
        heapStart = nullptr;
        top = nullptr;
        searchStart = nullptr;
        largestKnownBlock = nullptr;
    }
    ~Allocator(){}

    word_t *alloc(size_t size){
        size = align(size);

        if(auto block = findBlock(size))
        {
            return block->data;
        }

        // TODO: check for more space and generate bigger blocks
        return nullptr;
    }

    inline size_t align(size_t n){
        return (n + sizeof(word_t) -1) & ~(sizeof(word_t) - 1);
    }

    Block *getHeader(word_t *data)
    {
        return (Block*)((char*)data + sizeof(std::declval<Block>().data) - sizeof(Block));
    }

    void free(word_t *data)
    {
        Block* block = getHeader(data);
        if(canCoalesce(block))
        {
            block = coalesce(block);
        }
        setUsed(block, false);
        // TODO: implement clear data block
    }

    Block *getLargestBlock()
    {
        if(!isUsed(largestKnownBlock))
            return largestKnownBlock;
        return nullptr;
    }

    Block *firstFit(size_t size){
        auto block = heapStart;

        while(block != nullptr)
        {
            // if file is larger than biggest block -> split file over multiple blocks
            if(block->size > largestKnownBlock->size)
            {
                largestKnownBlock = block;
            }
            if(isUsed(block) || block->size < size)
            {
                block = block->next;
                continue;
            }
            return listAllocate(block, size);
        }
    }

    Block *nextFit(size_t size)
    {
        // TODO: implement
        return nullptr;
    }

    Block *findBlock(size_t size)
    {
        switch(SearchMode)
        {
            case SearchMode::FirstFit:
                return firstFit(size);
            case SearchMode::NextFit:
                return nextFit(size);
        }
    }

    Block *split(Block* block, size_t size)
    {
        Block* newBlock = new Block();
        newBlock->size = block->size - (size + sizeof(Block));
        newBlock->next = block->next;
        setUsed(newBlock, false);

        //return old block with exact size
        block->size = size;
        block->next = newBlock;
        setUsed(block, true);
        return block;
    }

    inline bool canSplit(Block *block, size_t size)
    {
        if(size > MIN_BLOCK_SIZE)
        {
            // blockheader * 2 + size + min block size
            return block->size > (sizeof(Block) *2 + size + MIN_BLOCK_SIZE);
        }else{
            // two min blocks -> in one block data 
            return block->size > (sizeof(Block) + MIN_BLOCK_SIZE) * 2;
        }
    }
    Block *listAllocate(Block *block, size_t size)
    {
        if(canSplit(block, size))
        {
            block = split(block, size);
        }
        setUsed(block, true);
        block->size = size;
        return block;
    }
    bool canCoalesce(Block *block)
    {
        return block->next && !isUsed(block->next);
    }
    Block *coalesce(Block *block)
    {
        if(canCoalesce(block))
        {
            block->size += block->next->size;
            block->next = block->next->next;
        }
        return block;
    }

    inline size_t getSize(Block *block)
    {
        return block->header & ~1L;
    }
    inline void setSize(Block *block, size_t size)
    {
        block->header = size | (block->header & 1L);
    }
    inline bool isUsed(Block *block)
    {
        return block->header & 1;
    }
    inline void setUsed(Block *block, bool used)
    {
        if(used)
            block->header |= 1;
        else
            block->header &= ~1;
    }
};
