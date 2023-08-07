#include <iostream>

#include "FileObserver.hpp"

int main(int argc, char* argv[])
{
	std::function<bool(std::string&)> filter = [](std::string& line) { 
            const size_t position = line.rfind('/');
            const std::string fileName = line.substr(position + 1, line.size());
        
            if (fileName.rfind(".yaml") == std::string::npos)
                return false;
            const char letter = fileName[0];
            if (letter == '#' || letter == '!')
                return false;

            return true;
        };

	FileObserver FileObserver{"/workspaces/Hibiscus/data", filter};
	FileObserver.start();
	return 0;
}