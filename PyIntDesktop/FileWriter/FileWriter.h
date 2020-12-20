#pragma once

#include "../framework.h"
#include "../FileHandler/FileHandler.h"

class FileWriter : public FileHandler
{
public:
	void WriteFile(char* fileText, PWSTR filePath);
	~FileWriter();
};
