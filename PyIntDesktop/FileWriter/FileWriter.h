#pragma once

#include "../framework.h"
#include "../FileHandler/FileHandler.h"

class FileWriter : public FileHandler
{
	PWSTR _filePath;

private:
	void WriteFileInternal(PWSTR filePath, char* fileText);
public:
	void WriteFile(char* fileText);
	~FileWriter();
};
