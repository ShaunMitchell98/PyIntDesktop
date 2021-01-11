#pragma once
#include "../../framework.h"

namespace Infrastructure
{
	class CharConverter
	{
		LPWSTR _text;
	public:
		char* LptstrToCharPointer(const wchar_t* wstr, unsigned int codePage);
		LPWSTR CharPointerToLpwstr(char* string, unsigned int codePage);
		~CharConverter();
	};
}


