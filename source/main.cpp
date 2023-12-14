#include <iostream>

#include "FileObserver.hpp"

int main(int argc, char* argv[])
{
    std::cout << "Start" << std::endl;
    FileObserver FileObserver { "/workspaces/Hibiscus/data", 5000 };
    FileObserver.start();
    return 0;
}