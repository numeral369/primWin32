#include "stdafx.h"
#include "PngToBitmap.h"
//#include <stdio.h>
//#include "png.h"

bool GetImageData(char* szPath, unsigned int *pWidth, unsigned int *pHeight, unsigned char ** dpData)
{
	FILE* fp = NULL;

	fopen_s(&fp, szPath, "rb");
	if (!fp) return false;

	png_infop info_ptr;
	png_structp png_ptr;

	// Check header
	png_byte header[8];
	fread(header, 1, 8, fp);
	if (png_sig_cmp(header, 0, 8) != 0)
	{
		fclose(fp);
		return false;
	}

	png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (png_ptr == NULL)
	{
		fclose(fp);
		return false;
	}

	info_ptr = png_create_info_struct(png_ptr);
	if (info_ptr == NULL)
	{
		png_destroy_read_struct(&png_ptr, NULL, NULL);
		fclose(fp);
		return false;
	}

	if (setjmp(png_jmpbuf(png_ptr)))
	{
		png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
		fclose(fp);
		return false;
	}


	png_init_io(png_ptr, fp);
	png_set_sig_bytes(png_ptr, 8);


	png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_EXPAND, 0);

	*pWidth = png_get_image_width(png_ptr, info_ptr);
	*pHeight = png_get_image_height(png_ptr, info_ptr);
	int color_type = png_get_color_type(png_ptr, info_ptr);

	png_bytep* row_ptrs = png_get_rows(png_ptr, info_ptr);

	unsigned int block_size;
	unsigned long pos = 0;
	block_size = color_type == 6 ? 4 : 3;
	*dpData = new png_byte[(*pWidth) * (*pHeight) * 4];

	for (unsigned int y = 0; y < *pHeight; ++y)
	{
		for (unsigned int x = 0; x < *pWidth * block_size; x += block_size)
		{
			(*dpData)[pos++] = row_ptrs[y][x + 2];
			(*dpData)[pos++] = row_ptrs[y][x + 1];
			(*dpData)[pos++] = row_ptrs[y][x + 0];
			(*dpData)[pos++] = row_ptrs[y][x + 3];
		}
	}

	png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
	fclose(fp);

	return true;
}

HBITMAP PngToBitmap::CreateBitmapFromResource(/*TCHAR**/ char* filename)
{
	if (filename == NULL)
		return NULL;
	//TCHAR szPath[MAX_PATH];
	char* szPath;
	szPath = filename;
	//GetResource(szPath, filename);

	unsigned int width, height;
	unsigned char* pData;
	if (!GetImageData(szPath, &width, &height, &pData))
		return NULL;
	HBITMAP hBitmap = CreateBitmap(width, height, 1, 32, pData);
	delete[] pData;
	return hBitmap;
}

void PngToBitmap::PremultiplyBitmapAlpha(HDC hDC, HBITMAP hBmp)
{
	BITMAP bm = { 0 };
	GetObject(hBmp, sizeof(bm), &bm);
	BITMAPINFO* bmi = (BITMAPINFO*)_alloca(sizeof(BITMAPINFOHEADER) + (256 * sizeof(RGBQUAD)));
	::ZeroMemory(bmi, sizeof(BITMAPINFOHEADER) + (256 * sizeof(RGBQUAD)));
	bmi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	BOOL bRes = ::GetDIBits(hDC, hBmp, 0, bm.bmHeight, NULL, bmi, DIB_RGB_COLORS);
	if (!bRes || bmi->bmiHeader.biBitCount != 32) return;
	LPBYTE pBitData = (LPBYTE) ::LocalAlloc(LPTR, bm.bmWidth * bm.bmHeight * sizeof(DWORD));
	if (pBitData == NULL) return;
	LPBYTE pData = pBitData;
	::GetDIBits(hDC, hBmp, 0, bm.bmHeight, pData, bmi, DIB_RGB_COLORS);
	for (int y = 0; y < bm.bmHeight; y++) {
		for (int x = 0; x < bm.bmWidth; x++) {
			pData[0] = (BYTE)((DWORD)pData[0] * pData[3] / 255);
			pData[1] = (BYTE)((DWORD)pData[1] * pData[3] / 255);
			pData[2] = (BYTE)((DWORD)pData[2] * pData[3] / 255);
			pData += 4;
		}
	}
	::SetDIBits(hDC, hBmp, 0, bm.bmHeight, pBitData, bmi, DIB_RGB_COLORS);
	::LocalFree(pBitData);
}