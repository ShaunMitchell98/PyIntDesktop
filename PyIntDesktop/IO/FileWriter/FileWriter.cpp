#include "FileWriter.h"
#include <shobjidl.h>

namespace IO 
{
	static void WriteFileInternal(char* fileText, PWSTR filePath)
	{
		HANDLE handle = CreateFile(filePath, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, TRUNCATE_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL);

		if (handle == INVALID_HANDLE_VALUE)
		{
			MessageBox(NULL, L"Unable to open file", L"Error", MB_OK);
		}

		int textLength = strlen(fileText);

		OVERLAPPED ol = { 0 };
		BOOL success = WriteFileEx(handle, fileText, textLength, &ol, NULL);

		CloseHandle(handle);

		if (!success)
		{
			return;
		}
	}

	void FileWriter::WriteFile(char* fileText, PWSTR filePath)
	{
		if (filePath == NULL)
		{
			_filePath = RequestFilePath();
		}
		else {
			_filePath = filePath;
		}

		WriteFileInternal(fileText, _filePath);
	}

	FileWriter::~FileWriter()
	{
		CoUninitialize();
	}
}

