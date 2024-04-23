#include "pch.h"

#include "GameApplication.h"
#include <engine/input/InputManager.h>
#include <engine/Engine.h>

LRESULT GameApplication::WindowProcedure(_In_ HWND hWnd, _In_ UINT uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam)
{
	if (IsWindowEnabled(hWnd))
	{
		if (InputManager::GetInstance()->UpdateEvents(uMsg, wParam, lParam))
			return 0;
	}

	return ::Application::WindowProcedure(hWnd, uMsg, wParam, lParam);
}

GameApplication::GameApplication() : Application()
{
	//myGame = nullptr;
	//myEngine = Engine::CreateInstance(&myTimer);
	engine->CreateInstance();
}

GameApplication::~GameApplication()
{
	InputManager::GetInstance()->DestroyInstance();

	//myEngine->DestroyInstance();

	//delete myGame;
	//myGame = nullptr;
}

bool GameApplication::BeginFrame()
{
	//return myEngine->BeginFrame();
	return true;
}

void GameApplication::Init(HINSTANCE hInstance, WNDPROC wndProc)
{
	hInstance; wndProc;

	DirectX::XMINT2 resolution = { 1920, 1080 };
	std::wstring icon = L"";

	{
		std::vector<RECT> monitorRects;
		EnumDisplayMonitors(NULL, NULL, [](HMONITOR, HDC, LPRECT lprcMonitor, LPARAM dwData)->BOOL {
			std::vector<RECT>* monitorRects = reinterpret_cast<std::vector<RECT>*>(dwData);
			monitorRects->push_back(*lprcMonitor);
			return TRUE;
			}, reinterpret_cast<LPARAM>(&monitorRects));

		RECT primaryMonitorRect = monitorRects[0];
		resolution.x = primaryMonitorRect.right - primaryMonitorRect.left;
		resolution.y = primaryMonitorRect.bottom - primaryMonitorRect.top;
	}

	engine->Init(resolution.x, resolution.y, L"WindowClass", L"DirectX 12 App", icon.c_str(), hInstance, wndProc);
}

void GameApplication::Update(const TimerData& aTimerData)
{
	::Application::Update(aTimerData);

	if (GetKeyState(VK_ESCAPE))
	{
		PostQuitMessage(0);
	}

	InputManager::GetInstance()->Update();
}

void GameApplication::Render()
{

}

void GameApplication::EndFrame()
{
	::Application::EndFrame();
	engine->EndFrame();
}




//#ifdef _RELEASE
//	if (!mySplashScreen.isDone)
//	{
//		UpdateSplashScreen(dt);
//		mySplashScreen.guiElements->Update({});
//		mySplashScreen.guiElements->Render();
//
//		GraphicsEngine::GetInstance()->GetSpriteRenderer().Render();
//
//		return;
//	}
//#endif
//
//#ifndef _RELEASE
//	auto* im = InputManager::GetInstance();
//	if (im->IsKeyPressed('0'))
//	{
//		myShouldStep = !myShouldStep;
//	}
//
//	if (myShouldStep)
//	{
//		if (im->IsKeyPressed('9'))
//		{
//			float multiplier = 1.0f;
//			if (im->IsKeyHeld(VK_SHIFT))
//				multiplier = 10.0f;
//			myGame->Update(0.016f * multiplier, aTotalTime);
//		}
//	}
//	else
//	{
//		myGame->Update(dt, aTotalTime);
//	}
//#else
//	myGame->Update(dt, aTotalTime);
//#endif