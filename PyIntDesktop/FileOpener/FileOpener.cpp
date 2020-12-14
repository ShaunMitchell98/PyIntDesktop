#include "FileOpener.h"
#include <shobjidl.h>

static void GetFilePath(PWSTR* filePath) 
{
	IFileOpenDialog* pFileOpenDialog;

	HRESULT hResult = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL, IID_IFileOpenDialog, reinterpret_cast<void**>(&pFileOpenDialog));

	if (SUCCEEDED(hResult))
	{
		hResult = pFileOpenDialog->Show(NULL);

		if (SUCCEEDED(hResult))
		{
			IShellItem* pShellItem;

			hResult = pFileOpenDialog->GetResult(&pShellItem);

			if (SUCCEEDED(hResult))
			{
				pShellItem->GetDisplayName(SIGDN_FILESYSPATH, filePath);
				pShellItem->Release();
			}
		}

		pFileOpenDialog->Release();
	}
}

char* FileOpener::OpenFileInternal(PWSTR filePath) 
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

char* FileOpener::OpenFile()
{
	GetFilePath(&_filePath);
	return OpenFileInternal(_filePath);
}

FileOpener::FileOpener()
{
	CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
}

FileOpener::~FileOpener() 
{
	CoTaskMemFree(_filePath);
	CoUninitialize();
	HeapFree(GetProcessHeap(), 0, _fileText);
}
