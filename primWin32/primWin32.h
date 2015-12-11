#pragma once

#include "stdafx.h"
#include "resource.h"
#include "PngToBitmap.h"
#include "RoundButton.h"

#ifndef primWin32_H
#define primWin32_H
class primWin32{
private:
	void OnDrawItem(HWND hwnd, const DRAWITEMSTRUCT*);
};
#endif // !primWin32_H