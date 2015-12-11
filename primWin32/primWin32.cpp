// primWin32.cpp : Defines the entry point for the application.
//
#include "stdafx.h"
#include "primWin32.h"
#include <atltypes.h>

//#include "PngToBitmap.h"

#define MAX_LOADSTRING 100
#define BUTTON_01 1
//#define IDM_BTN1 6010
#define BUTTONMAX 2

#pragma comment(lib, "msimg32")

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name
HBITMAP hBM[BUTTONMAX];
static HWND static_label;
HWND hWndBtn[4];
int posClick = 0;
HDC cleanHDC = NULL;
bool saveDC = true;
HDC hdcMem = NULL;
HDC dest_dc[BUTTONMAX];
RoundButton *updateRBut;
RoundButton *aboutRBut;

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
void OnDrawItem(HWND hwnd, const DRAWITEMSTRUCT * lpDrawItem);
void onPaint(HDC, HBITMAP, int);
void onPaint(HDC, HBITMAP, int, int, int, int);
void wndShape(HWND hWnd, RECT *wndRect, HDC *hdc);

/*void checkIfComputerIsOpen(HWND hwnd){
	LPCWSTR janela = L"Computador";
	HWND computador = FindWindow(NULL, janela);

	if (!computador)
		MessageBox(hwnd, L"Erro", L"Janela não encontrada", MB_OK | MB_ICONERROR);
	else
		MessageBox(hwnd, L"Encontrada", L"Janela encontrada", MB_OK | MB_ICONEXCLAMATION);
}*/

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_PRIMWIN32, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_PRIMWIN32));

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	HBRUSH brush = CreateSolidBrush(RGB(250, 250, 2));

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_PRIMWIN32));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= /*(HBRUSH)(COLOR_WINDOW+1)*/ brush;
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_PRIMWIN32);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_CREATE:{
		LPCWSTR button01_ID = L"BUTTON";
		LPCWSTR button01_text = L"Verificar";
		//HWND button01 = CreateWindowEx(NULL, button01_ID, button01_text, BS_DEFPUSHBUTTON | WS_VISIBLE | WS_BORDER | WS_CHILD, 100, 50, 100, 30, hWnd, (HMENU)BUTTON_01, NULL, NULL);
		//hWndBtn[0] = CreateWindowEx(NULL, L"BUTTON", L"Button 1", WS_VISIBLE | BS_OWNERDRAW | WS_CHILD, 5, 55, 32, 32, hWnd, (HMENU)BUTTON_01, hInst, NULL);
		updateRBut = new RoundButton(L"BUTTON", L"Button 1", 5, 55, 32, 32, hWnd, (HMENU)(BUTTON_01), hInst, "pics\\icon_updated2.png");
		aboutRBut = new RoundButton(L"BUTTON", L"Button 2", 5, 105, 32, 32, hWnd, (HMENU)(BUTTON_01 + 1), hInst, "pics\\icon_about.png");
		hWndBtn[0] = (*updateRBut).getRoundButton();
		hWndBtn[1] = (*aboutRBut).getRoundButton();
		//hBM[0] = PngToBitmap::CreateBitmapFromResource( "pics\\icon_updated2.png");

		static_label = CreateWindowEx(NULL, L"Static", L"Please Enter A Number", WS_CHILD | WS_VISIBLE, 45, 15, 175, 45, hWnd, 0, hInst, 0);
	}
		break;
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case BUTTON_01:
		{
			//checkIfComputerIsOpen(hWnd);
			//SetWindowText(static_label, L"TEXT");
		}
		break;
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		//BitBlt(hdc, 0, 0, 500, 500, NULL, 0, 0, BLACKNESS);
		// TODO: Add any drawing code here...
		EndPaint(hWnd, &ps);
		break;
	case WM_DRAWITEM: (*updateRBut).OnDrawItem(hWnd, (const DRAWITEMSTRUCT*)lParam);
		(*aboutRBut).OnDrawItem(hWnd, (const DRAWITEMSTRUCT*)lParam);
		//return (OnDrawItem(hWnd, (const DRAWITEMSTRUCT*)lParam), 0);
		break;
	case WM_DESTROY:
		updateRBut->~RoundButton();
		aboutRBut->~RoundButton();
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}