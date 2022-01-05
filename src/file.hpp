#pragma once

struct File
{
    public:
        char name[32];
        long size;
        File* next = nullptr;
        File* prev = nullptr;
        char* data = nullptr;
}__attribute__ ((packed));