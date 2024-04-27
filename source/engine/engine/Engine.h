#pragma once
#include <windows.h>
#include "DX12.h"

class Window;
class GraphicsEngine;
class InputManager;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

#define engine Engine::GetInstance()

class Engine
{
public:
	static Engine* CreateInstance();
	static void DestroyInstance();
	inline static Engine* GetInstance() { return myInstance; };

	bool Init(
		const int& aWindowWidth,
		const int& aWindowHeight,
		const wchar_t* aClassName,
		const wchar_t* aWindowName,
		const wchar_t* aIconPath,
		HINSTANCE& hInstance,
		WNDPROC WndProc
	);
	bool BeginFrame();
	void EndFrame();

	Window* GetWindow() const { return myWindow; }
	DX12& DX() { return myDX12; }
private:
	Engine();
	~Engine();
	Engine(const Engine&) = delete;

	DX12 myDX12;
	Window* myWindow;
	bool myShouldExit = false;
	static Engine* myInstance;
};