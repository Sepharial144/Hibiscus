#include <iostream>

#include "FileObserver.hpp"
#include "logger/logger.hpp"

int main(int argc, char* argv[])
{
    logger::info("Hibiscus start application ...");
    FileObserver FileObserver { "/workspaces/Hibiscus/data", 5000 };
    FileObserver.start();
    return 0;
}