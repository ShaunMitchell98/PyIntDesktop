#pragma once

#include "../framework.h"

class FileHandler
{
protected:
	PWSTR _filePath;
	char* _fileText;
	PWSTR RequestFilePath();
	FileHandler();
public:
	PWSTR GetFilePath();
};

