#include "pch.h"

#include "Engine.h"
#include "input/InputManager.h"
#include "Window.h"

Engine* Engine::myInstance = nullptr;

Engine* Engine::CreateInstance()
{
	if (myInstance == nullptr)
	{
		myInstance = new Engine();
	}
	return myInstance;
}

void Engine::DestroyInstance()
{
	if (myInstance != nullptr)
	{
		delete myInstance;
		myInstance = nullptr;
	}
}

Engine::Engine()
{
	myShouldExit = false;
	myWindow = new Window();
	InputManager::CreateInstance();
}

Engine::~Engine()
{
	delete myWindow;
	InputManager::DestroyInstance();
}

bool Engine::Init(
	const int& aWindowWidth,
	const int& aWindowHeight,
	const wchar_t* aClassName,
	const wchar_t* aWindowName,
	const wchar_t* aIconPath,
	HINSTANCE& hInstance,
	WNDPROC WndProc
)
{
	myWindow->Init(aWindowWidth, aWindowHeight, aClassName, aWindowName, aIconPath, hInstance, WndProc);
	
	DX().Init(myWindow);

	InputManager::GetInstance()->Init();

	return true;
}

bool Engine::BeginFrame()
{
	if (myShouldExit) { return false; }

	MSG msg = { 0 };
	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		if (msg.message == WM_QUIT)
		{
			myShouldExit = true;
		}
	}

	return DX().BeginFrame();
}


void Engine::EndFrame()
{
	//DX().Render();
	DX().EndFrame();
	//DX().swapChain->Present(1, 0);
}
