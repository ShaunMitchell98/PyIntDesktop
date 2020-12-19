#include "FileHandler.h"
#include <shobjidl.h>

void FileHandler::GetFilePath(PWSTR* filePath)
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

FileHandler::FileHandler()
{
	CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
}