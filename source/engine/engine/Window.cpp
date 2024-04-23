#include "pch.h"

#include "Window.h"

#define WINDOWED_STYLE WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SIZEBOX
#define FULLSCREEN_STYLE WS_POPUP

static RECT GetClientArea(HWND hWnd) {
	RECT clientRect;
	GetClientRect(hWnd, &clientRect);
	return clientRect;
}

Window::Window() : myWindowData(), myIsFullscreen(false)
{

}

Window::~Window()
{

}
//
//
//void Window::SetWindowDimensions(const int aX,
//	const int aY,
//	int aWidth,
//	int aHeight,
//	const bool aFullscreen
//)
//{
//	aWidth = aWidth == -1 ? myWindowWidth : aWidth;
//	aHeight = aHeight == -1 ? myWindowHeight : aHeight;
//
//	RECT wr = {};
//	wr.left = aX;
//	wr.right = aWidth + wr.left;
//	wr.top = aY;
//	wr.bottom = aHeight + wr.top;
//
//	myIsFullscreen = aFullscreen;
//
//	if (aFullscreen)
//	{
//		SetWindowLong(hWnd, GWL_STYLE, FULLSCREEN_STYLE);
//	}
//	else
//	{
//		SetWindowLong(hWnd, GWL_STYLE, WINDOWED_STYLE);
//		AdjustWindowRect(&wr, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE);
//	}
//
//	aWidth = wr.right - wr.left;
//	aHeight = wr.bottom - wr.top;
//
//	//std::cout << aWidth << ", " << aHeight << std::endl;
//
//	//adjust window size to account for window borders
//	myWindowWidth = aWidth;
//	myWindowHeight = aHeight;
//	myWindowPositionX = aX;
//	myWindowPositionY = aY;
//	SetWindowPos(hWnd, nullptr, aX, aY, aWidth, aHeight, SWP_NOZORDER);
//}

bool Window::Init(
	const int& aWindowWidth,
	const int& aWindowHeight,
	const wchar_t* aClassName,
	const wchar_t* aWindowName,
	const wchar_t* aIconPath,
	HINSTANCE& hInstance,
	WNDPROC WndProc
)
{
	aIconPath;

	WNDCLASSEXW wcex = {};
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wcex.lpfnWndProc = WndProc;
	wcex.hInstance = hInstance;
	//wcex.hIcon = (HICON)LoadImage(nullptr, aIconPath, IMAGE_ICON, LR_DEFAULTSIZE, LR_DEFAULTSIZE, LR_LOADFROMFILE);
	//wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW);
	wcex.lpszClassName = aClassName;
	RegisterClassExW(&wcex);

	myWindowData.x = 0;
	myWindowData.y = 0;


	myHWnd = CreateWindowEx(
		0,
		aClassName,
		aWindowName,
		WS_OVERLAPPEDWINDOW,
		myWindowData.x,
		myWindowData.y,
		aWindowWidth,
		aWindowHeight,
		nullptr,
		nullptr,
		hInstance,
		nullptr
	);

	if (!myHWnd) { return false; }

	myWindowData.width = aWindowWidth;
	myWindowData.height = aWindowHeight;

	RECT clientRect = GetClientArea(myHWnd);
	myWindowData.viewPortWidth = clientRect.right - clientRect.left;
	myWindowData.viewPortHeight = clientRect.bottom - clientRect.top;

	// https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-showwindow
	ShowWindow(myHWnd, SW_SHOWDEFAULT);

	UpdateWindow(myHWnd);

	return true;
}

HCURSOR Window::LoadCustomCursor(LPCWSTR aCurFileName)
{
	HICON icon = (HICON)LoadImage(nullptr, aCurFileName, IMAGE_CURSOR, 32, 32, LR_LOADFROMFILE | LR_SHARED);
	ICONINFO cur;
	GetIconInfo(icon, &cur);
	cur.xHotspot = 0;
	cur.yHotspot = 0;

	HCURSOR hCursor = CreateIconIndirect(&cur);

	return hCursor;
}

//void Window::SetResolution(const int aWidth, const int aHeight)
//{
//	//myGraphicsWindow->InitializeSwapChain();
//
//	SetWindowDimensions(myWindowPositionX, myWindowPositionY, aWidth, aHeight, myIsFullscreen);
//}
