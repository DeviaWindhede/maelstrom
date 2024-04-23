#include "pch.h"
#include "GameApplication.h"

Application* CreateApplication()
{
	return new GameApplication();
}

LRESULT CALLBACK WndProc(_In_ HWND hWnd, _In_ UINT uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam)
{
	return GameApplication::WindowProcedure(hWnd, uMsg, wParam, lParam);
}

#include <engine/application/ApplicationEntryPoint.cpp>
