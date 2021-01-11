#pragma once
#include "../../framework.h"

namespace UI 
{
	class TextBox
	{
	private:
		HWND _hwnd;
		int _x;
		int _y;
		int _width;
		int _height;
	public:
		TextBox(HINSTANCE hInstance, HWND parentWindow, int x, int y, int height);
		INT_PTR PaintWindow(HDC hdc);
		HWND GetHandle();
	};
}

