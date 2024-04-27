#include "pch.h"
#include "Application.h"

#include <iostream>

LRESULT Application::WindowProcedure(_In_ HWND hWnd, _In_ UINT uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_SIZE:
	{
		int i = 0;
		i++;
		//HANDE RESIZE
		break;
	}
	case WM_KILLFOCUS:
	{
		break;
	}
	case WM_SHOWWINDOW:
	{
		// SW_OTHERUNZOOM	| The window is being uncovered because a maximize window was restored or minimized.
		// SW_OTHERZOOM		| The window is being covered by another window that has been maximized.
		// SW_PARENTCLOSING | The window's owner window is being minimized.
		// SW_PARENTOPENING | The window's owner window is being restored.
		break;
	}
	case WM_SETFOCUS:
	{
		int i = 0;
		i++;
		//HANDLE SET FOCUS
		break;
	}
	case WM_DESTROY:
	case WM_CLOSE:
	case WM_QUIT:
		PostQuitMessage(0);
		break;
	case WM_MOUSEACTIVATE:
		// https://github.com/microsoft/DirectXTK/wiki/Mouse
		// When you click to activate the window, we want Mouse to ignore that event.
		return MA_ACTIVATEANDEAT;
	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
	return 0;

}

Application::Application() : myTimer() {}
Application::~Application() {}

void Application::Run()
{
	TimerData content = { 0.0f, 0.0 };

	//const auto[deltaTime, totalTime] = content;

	while (BeginFrame())
	{
		Update(myTimer.GetTimerData());
		Render();
		EndFrame();
	}
}

void Application::EndFrame()
{
	myTimer.Update();
}

