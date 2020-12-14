#include "Button.h"
#include "../Resource.h"

HWND Button::InitWindow(HINSTANCE hInstance, HWND parentWindow)
{
    HWND button = CreateWindowW(L"BUTTON", L"SUBMIT", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
        600, 500, 100, 100, parentWindow, nullptr, hInstance, 0);

    if (!button)
    {
        return button;
    }

    return button;
}