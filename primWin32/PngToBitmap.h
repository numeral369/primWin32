#include "stdafx.h"
#include <iostream>
#include <windows.h>
#include "png.h"
#include <stdio.h>

#ifndef PngToBitmap_h
#define PngToBitmap_h

using namespace std;

class PngToBitmap{
public:
	static HBITMAP CreateBitmapFromResource(/*TCHAR**/ char*);
	static void PremultiplyBitmapAlpha(HDC, HBITMAP);
};

#endif



