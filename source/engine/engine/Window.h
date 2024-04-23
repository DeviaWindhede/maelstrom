#pragma once
#include <windows.h>

class GraphicsWindow;
class InputManager;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

struct WindowData
{
	int x = 0;
	int y = 0;
	int width = 0;
	int height = 0;
	int viewPortWidth = 0;
	int viewPortHeight = 0;
};

class Window
{
public:
	Window();
	~Window();
	Window(const Window&) = delete;

	HWND GetHWND() const { return myHWnd; }

	bool Init(
		const int& aWindowWidth,
		const int& aWindowHeight,
		const wchar_t* aClassName,
		const wchar_t* aWindowName,
		const wchar_t* aIconPath,
		HINSTANCE& hInstance,
		WNDPROC WndProc
	);

	//void SetResolution(const int aWidth, const int aHeight);
	//void SetWindowDimensions(
	//	const int aX,
	//	const int aY,
	//	int aWidth,
	//	int aHeight,
	//	const bool aFullscreen = true
	//);

	HCURSOR LoadCustomCursor(LPCWSTR aCurFileName);

	const WindowData& GetWindowData() const { return myWindowData; }
private:

	WindowData myWindowData;
	HWND myHWnd = {};
	bool myIsFullscreen;
};