#include "CharConverter.h"

char* CharConverter::LptstrToCharPointer(const wchar_t* wstr, unsigned int codePage)
{
    int sizeNeeded = WideCharToMultiByte(codePage, 0, wstr, -1, NULL, 0, NULL, NULL);
    char* encodedStr = new char[sizeNeeded];
    WideCharToMultiByte(codePage, 0, wstr, -1, encodedStr, sizeNeeded, NULL, NULL);
    return encodedStr;
}

LPWSTR CharConverter::CharPointerToLpwstr(char* string, unsigned int codePage)
{
    int stringLength = strlen(string);
    _text = (LPWSTR)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, stringLength * 2);
    MultiByteToWideChar(codePage, 0, string, -1, _text, stringLength);
    return _text;
}

CharConverter::~CharConverter() {

    if (_text != NULL)
    {
        HeapFree(GetProcessHeap(), 0, _text);
    }
}