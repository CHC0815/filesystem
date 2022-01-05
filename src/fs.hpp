#pragma once

#include <vector>
#include <fstream>
#include <algorithm>
#include <iterator>
#include <iostream>
#include <cstring>

#include "folder.hpp"
#include "file.hpp"
#include "block.hpp"
#include "allocator.hpp"

#define FILESYSTEM_SIZE 10000

class FileSystem
{
public:
    std::vector<unsigned char> buffer;
    Block *heapStart;

    FileSystem(){}
    ~FileSystem(){}

    void load()
    {
        std::ifstream input("./bin/fs.bin", std::ios::binary);
        input.unsetf(std::ios::skipws);
        std::streampos fileSize;
        input.seekg(0, std::ios::end);
        fileSize = input.tellg();
        input.seekg(0, std::ios::beg);
        buffer.reserve(fileSize);
        buffer.insert(buffer.begin(), std::istream_iterator<unsigned char> (input), std::istream_iterator<unsigned char>());
    }

    void save()
    {
        if(buffer.size() <= 0)
            return; 
        std::cout << "Saving fs" << std::endl;
        std::ofstream output("./bin/fs.bin", std::ios::binary);
        // copy data
        copy(buffer.begin(), buffer.end(), std::ostream_iterator<unsigned char>(output, ""));
    }

    void init()
    {
        buffer.clear();
        buffer.reserve(FILESYSTEM_SIZE);
        for(ulong i = 0; i < FILESYSTEM_SIZE; i++)
        {
            buffer.push_back(0);
        }
        // magic number
        buffer[0] = 'c';
        buffer[1] = 'h';
        buffer[2] = 'c';
        
        // use blocks
        // Folder folder;
        // for(int i = 0; i < 32;i++)
            // folder.name[i] = '\0';
        // strcpy(folder.name, "/");
        // insert_folder_at(3, folder);

        // create master block
        Block* block = new Block();
        block-> = FILESYSTEM_SIZE-3; // - magic number
        insert_at<Block>(3, *block);
        heapStart = block;
        save();
    }
    bool is_valid()
    {
        return (buffer.size() >= 3) && buffer[0] == 'c' && buffer[1] == 'h' && buffer[2] == 'c';
    }

    template <typename T>
    void insert_at(long index, T &t)
    {
        unsigned char* data = reinterpret_cast<unsigned char*>(&t);
        for(size_t i = 0; i < sizeof(T); i++)
        {
            buffer[index + i] = data[i];
        }
    }
    size_t getBlockCount()
    {
        Block* current = heapStart;
        size_t count = 0;
        while(current != nullptr)
        {
            count++;
            current = current->next;
        }
        return count;
    }
};