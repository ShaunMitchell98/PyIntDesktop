#pragma once

#include "../../framework.h"
#include "../FileHandler/FileHandler.h"

namespace IO 
{
	class FileWriter : public FileHandler
	{
	public:
		void WriteFile(char* fileText, PWSTR filePath);
		~FileWriter();
	};
}

