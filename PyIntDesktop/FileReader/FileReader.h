#pragma once

#include "../framework.h"
#include "../FileHandler/FileHandler.h"

class FileReader : public FileHandler
{
	PWSTR _filePath;
	char* _fileText;

private:
    char* ReadFileInternal(PWSTR filePath);
public:
	char* ReadFile();
	~FileReader();
};
