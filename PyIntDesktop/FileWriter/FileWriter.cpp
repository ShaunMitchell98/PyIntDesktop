#include "FileWriter.h"
#include <shobjidl.h>

void FileWriter::WriteFileInternal(PWSTR filePath, char* fileText)
{
	HANDLE handle = CreateFile(filePath, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL);

	if (handle == INVALID_HANDLE_VALUE)
	{
		MessageBox(NULL, L"Unable to open file", L"Error", MB_OK);
	}

	LARGE_INTEGER fileSize;

	BOOL success = GetFileSizeEx(handle, &fileSize);

	if (!success) {
		return;
	}

	OVERLAPPED ol = { 0 };
	success = WriteFileEx(handle, fileText, fileSize.LowPart, &ol, NULL);

	CloseHandle(handle);

	if (!success)
	{
		return;
	}
}

void FileWriter::WriteFile(char* fileText)
{
	GetFilePath(&_filePath);
	return WriteFileInternal(_filePath, fileText);
}

FileWriter::~FileWriter()
{
	CoTaskMemFree(_filePath);
	CoUninitialize();
}
