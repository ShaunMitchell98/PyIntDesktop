#pragma once

#include "../../framework.h"

namespace IO 
{
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
}
