#include "TextBox.h"

HWND TextBox::InitWindow(HINSTANCE hInstance, HWND parentWindow, int x, int y, int height)
{
    HWND textBox = CreateWindowW(L"EDIT", L"", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_MULTILINE | ES_LEFT | ES_WANTRETURN | ES_AUTOVSCROLL,
        x, y, 700, height, parentWindow, nullptr, hInstance, 0);

    if (!textBox)
    {
        return textBox;
    }

    return textBox;
}