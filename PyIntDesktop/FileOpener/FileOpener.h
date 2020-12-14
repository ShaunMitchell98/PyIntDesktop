#pragma once

#include "../framework.h"
class FileOpener
{
	PWSTR _filePath;
	char* _fileText;

private:
    char* OpenFileInternal(PWSTR filePath);
public:
	char* OpenFile();
	FileOpener();
	~FileOpener();
};
