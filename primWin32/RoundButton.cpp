#include "stdafx.h"
#include "RoundButton.h"



RoundButton::RoundButton(_In_opt_ LPCTSTR   lpClassName,
	_In_opt_ LPCTSTR   lpWindowName,
	_In_     int       x,
	_In_     int       y,
	_In_     int       nWidth,
	_In_     int       nHeight,
	_In_opt_ HWND      hWndParent,
	_In_opt_ HMENU     hMenu,
	_In_opt_ HINSTANCE hInstance,
	char *caminhoDoFicheiro){
	saveDC = true;
	hWButton = CreateWindowEx(NULL, lpClassName, lpWindowName, WS_VISIBLE | BS_OWNERDRAW | WS_CHILD, x, y, nWidth, nHeight, hWndParent, hMenu, hInstance, NULL);
	hBmp = PngToBitmap::CreateBitmapFromResource(caminhoDoFicheiro);
	quantrB++;
}

void RoundButton::OnDrawItem(HWND hwnd, const DRAWITEMSTRUCT * lpDrawItem)
{
	RECT rc;
	HDC hdc;
	//HDC hdcMem;
	HBITMAP hBM0;
	BITMAP bm;
	int nIdx;

	if (lpDrawItem->CtlID < BUTTON_01 || lpDrawItem->CtlID >(BUTTON_01 + BUTTONMAX))
		return;

	nIdx = lpDrawItem->CtlID - 1;
	rc = lpDrawItem->rcItem;
	hdc = lpDrawItem->hDC;
	//hBM0 = hBM[lpDrawItem->CtlID-IDM_BTN1];
	hBM0 = NULL;

	wndShape(hWButton, &rc, &hdc);

	GetObject(hBmp, sizeof(bm), &bm);
	if (saveDC){//to save the state of hdc before the onPaint process
		hBM0 = CreateCompatibleBitmap(hdc, rc.left - rc.right, rc.top - rc.bottom);
		hdcMem = CreateCompatibleDC(hdc);
		SelectObject(hdcMem, hBM0);
		BitBlt(hdcMem, 0, 0, bm.bmWidth, bm.bmHeight, hdc, 0, 0, SRCCOPY);
		saveDC = false;
	}

	if (lpDrawItem->itemState & ODS_SELECTED) {
		onPaint(hdc, hBmp, 255, 1, 1, nIdx);
		//BitBlt(hdc, 1, 1, bm.bmWidth, bm.bmHeight, hdcMem, 0, 0, SRCCOPY);
		//DrawEdge(lpDrawItem->hDC, &rc, EDGE_SUNKEN, BF_RECT);
		//SetWindowText(static_label, L"Clicked");
	}
	else {
		//BitBlt(hdc, 0, 0, bm.bmWidth, bm.bmHeight, hdcMem, 0, 0, SRCCOPY);
		onPaint(hdc, hBmp, nIdx);
		//DrawEdge(lpDrawItem->hDC, &rc, EDGE_RAISED, BF_RECT);
		//SetWindowText(static_label, L"Not clicked");
	}
	DeleteDC(hdcMem);
	//ReleaseDC(hWndBtn[lpDrawItem->CtlID - 1], hdc);
	DeleteDC(hdc);
}

void RoundButton::wndShape(HWND hWnd, RECT *wndRect, HDC *hdc){

	HRGN region;
	int save;
	save = SaveDC(*hdc);

	POINT rectTL;
	//RECT wndRect = *wndRec;

	rectTL.x = (*wndRect).left;
	rectTL.y = (*wndRect).top;
	POINT rectBR;
	rectBR.x = (*wndRect).right;
	rectBR.y = (*wndRect).bottom;

	ScreenToClient(hWnd, &rectBR);
	ScreenToClient(hWnd, &rectTL);

	(*wndRect).left = rectTL.x;
	(*wndRect).top = rectTL.y;
	(*wndRect).right = rectBR.x;
	(*wndRect).bottom = rectBR.y;

	BeginPath(*hdc);
	//Ellipse(*hdc, (*wndRect).left, (*wndRect).top, (*wndRect).right, (*wndRect).bottom);
	Ellipse(*hdc, 0, 0, 30, 30);
	EndPath(*hdc);

	region = PathToRegion(*hdc);
	SetWindowRgn(hWnd, region, TRUE);
	//RestoreDC(*hdc, save);
	//VERIFY(region.CreateFromPath(&dc));
	//VERIFY(SetWindowRgn((HRGN)region, TRUE));
	//region.Detach(); // must not close handle
	//dc.RestoreDC(save);
	//CButton::PreSubclassWindow();
}

void RoundButton::onPaint(HDC hdc, HBITMAP hBM, int nIdx){
	HDC dest_dc = CreateCompatibleDC(hdc);
	int intSaveDC = 0;
	intSaveDC = SaveDC(hdc);
	CRect rect(0, 0, 16, 16);

	//if ()
	SelectObject(dest_dc, hBM);
	static bool pmdone = false;
	if (!pmdone) {
		PngToBitmap::PremultiplyBitmapAlpha(dest_dc, hBM);
		pmdone = true;
	}

	BLENDFUNCTION bf;
	bf.BlendOp = AC_SRC_OVER;
	bf.BlendFlags = 0;
	bf.SourceConstantAlpha = 255;
	bf.AlphaFormat = AC_SRC_ALPHA;

	BitBlt(hdc, 0, 0, 32, 32, hdcMem, 0, 0, SRCCOPY);
	AlphaBlend(hdc, rect.left, rect.top, 31, 31, dest_dc, 0, 0, 31, 31, bf);//0x26011D0A.
	//AlphaBlend(hdc, 10, 10, 31, 31, dest_dc[nIdx], 0, 0, 31, 31, bf);
	DeleteDC(dest_dc);
	RestoreDC(hdc, intSaveDC);
}

void RoundButton::onPaint(HDC hdc, HBITMAP hBM, int transparency, int originx, int originy, int nIdx){//transparency range: 0-255;
	HDC dest_dc = CreateCompatibleDC(hdc);
	int intSaveDC = 0;

	//intSaveDC = SaveDC(hdc);

	CRect rect(0, 0, 16, 16);
	SelectObject(dest_dc, hBM);

	/*static bool pmdone = false;
	if (!pmdone) {
	PngToBitmap::PremultiplyBitmapAlpha(dest_dc, hBM);
	pmdone = true;
	}*/

	BLENDFUNCTION bf;
	bf.BlendOp = AC_SRC_OVER;
	bf.BlendFlags = 0;
	bf.SourceConstantAlpha = transparency;
	bf.AlphaFormat = AC_SRC_ALPHA;

	BitBlt(dest_dc, 0, 0, 32, 32, hdcMem, 0, 0, SRCCOPY);
	//BitBlt(hdc, 1, 1, 32, 32, dest_dc, 0, 0, SRCCOPY);
	TransparentBlt(hdc, 1, 1, 32, 32, dest_dc, 0, 0, 32, 32, RGB(0, 0, 0));
	//AlphaBlend(hdc, originx, originy, 31, 31, dest_dc, 0, 0, 31, 31, bf);//0x26011D0A.
	DeleteDC(dest_dc);
	//RestoreDC(hdc, intSaveDC);
}

HWND RoundButton::getRoundButton(){
	return hWButton;
}