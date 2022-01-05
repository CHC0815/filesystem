#pragma once

#include "file.hpp"

struct Folder
{
public:
    char name[32];

    Folder* next_folder = nullptr;
    Folder* prev_folder = nullptr;
    File* next_file = nullptr;
    File* prev_file = nullptr;
}__attribute__ ((packed));