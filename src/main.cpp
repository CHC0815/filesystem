#include <iostream>
#include "fs.hpp"
#include "block.hpp"

int main()
{
    FileSystem *fs = new FileSystem();
    fs->load();
    if(!fs->is_valid())
    {
        fs->init();
    }

    std::cout << fs->getBlockCount() << std::endl;

    return 0;
}