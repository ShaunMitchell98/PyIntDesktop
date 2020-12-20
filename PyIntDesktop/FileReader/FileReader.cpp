#include "FileReader.h"
#include <shobjidl.h>

char* FileReader::ReadFileInternal(PWSTR filePath) 
{
	HANDLE handle = CreateFile(filePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL);

	if (handle == INVALID_HANDLE_VALUE)
	{
		MessageBox(NULL, L"Unable to open file", L"Error", MB_OK);
		return NULL;
	}

	LARGE_INTEGER fileSize;

	BOOL success = GetFileSizeEx(handle, &fileSize);

	if (!success) 
	{
		return NULL;
	}

	_fileText = (char*)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, fileSize.LowPart*2);

	OVERLAPPED ol = { 0 };
	success = ReadFileEx(handle, _fileText, fileSize.LowPart, &ol, NULL);

	CloseHandle(handle);

	if (!success)
	{
		return NULL;
	}

	return _fileText;
}

char* FileReader::ReadFile()
{
	_filePath = RequestFilePath();
	return ReadFileInternal(_filePath);
}

FileReader::~FileReader() 
{
	CoTaskMemFree(_filePath);
	CoUninitialize();
	HeapFree(GetProcessHeap(), 0, _fileText);
}
