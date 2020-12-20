#pragma once

#include "../framework.h"
#include "../FileHandler/FileHandler.h"

class FileReader : public FileHandler
{
private:
    char* ReadFileInternal(PWSTR filePath);
public:
	char* ReadFile();
	~FileReader();
};
