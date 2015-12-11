#pragma once

#include "stdafx.h"
#include "PngToBitmap.h"
#include <atltypes.h>

#ifndef RoundButton_h
#define RoundButtton_h

class RoundButton{
public:
	RoundButton(_In_opt_ LPCTSTR   lpClassName,
		_In_opt_ LPCTSTR   lpWindowName,
		_In_     int       x,
		_In_     int       y,
		_In_     int       nWidth,
		_In_     int       nHeight,
		_In_opt_ HWND      hWndParent,
		_In_opt_ HMENU     hMenu,
		_In_opt_ HINSTANCE hInstance,
		char *caminhoDoFicheiro);

	//~RoundButton();
	  
	HWND getRoundButton();
	void OnDrawItem(HWND hwnd, const DRAWITEMSTRUCT * lpDrawItem);

private:
#define BUTTON_01 1
#define BUTTONMAX 2

	HWND hWButton = NULL;
	HBITMAP hBmp;
	bool saveDC;
	HDC hdcMem;
	int rBid;
	static int quantrB;

	void wndShape(HWND hWnd, RECT *wndRect, HDC *hdc);
	void onPaint(HDC hdc, HBITMAP hBM, int nIdx);
	void onPaint(HDC hdc, HBITMAP hBM, int transparency, int originx, int originy, int nIdx);
};

#endif