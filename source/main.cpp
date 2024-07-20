#include "arguments.h"
#include "dfinder.h"

#include <iostream>

int main(int argc, char* argv[])
{
    try
    {
        CommandLineArgs args{argc, argv};

        DuplicateFinder finder{args};

        finder.Find();
        finder.PrintGroups();
    }
    catch (const std::exception& ex)
    {
        std::cerr << "Error: "<< ex.what() << std::endl;
    }

    return 0;
}
