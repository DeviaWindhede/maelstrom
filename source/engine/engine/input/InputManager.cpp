#include "pch.h"

#include "../Engine.h"
#include "../Window.h"
#include "InputManager.h"
#include "iostream"
#include <string> 

InputManager* InputManager::myInstance = nullptr;

InputManager::InputManager()
{
	myTentativeMousePosition = { 0, 0 };
	myCurrentMousePosition = { 0, 0 };
	myPreviousMousePosition = { 0, 0 };

	myMouseDelta = { 0, 0 };

	myMouseWheelStateUp = false;
	myMouseWheelStateDown = false;

	myMouse = std::make_unique<DirectX::Mouse>();
}

InputManager::~InputManager()
{
	myMouse->SetVisible(true);
	myMouse->SetMode(DirectX::Mouse::MODE_ABSOLUTE);
	ShowCursor(true);
	myMouseIsVisible = true;
	myMouse.reset();
	myMouse = nullptr;
	myInstance = nullptr;
}

void InputManager::Init()
{
	myMouse->SetWindow(engine->GetWindow()->GetHWND());
	myMouse->SetVisible(true);
	myMouse->SetMode(DirectX::Mouse::MODE_ABSOLUTE);
	ShowCursor(true);

	myMouseIsVisible = false;
}

bool InputManager::IsMouseVisible()
{
	return myMouseIsVisible;
}

bool InputManager::UpdateEvents(UINT message, WPARAM wParam, LPARAM lParam)  //LPARAM lParam
{
	message;
	wParam;
	lParam;
	switch (message)
	{
	case WM_KEYDOWN:
		myTentativeState[wParam] = true;
		return true;
	case WM_KEYUP:
		myTentativeState[wParam] = false;
		return true;
	case WM_RBUTTONDOWN:
		myTentativeState[VK_RBUTTON] = true;
		return true;
	case WM_RBUTTONUP:
		myTentativeState[VK_RBUTTON] = false;
		return false;
	case WM_ACTIVATE:
	case WM_ACTIVATEAPP:
	case WM_INPUT:
	case WM_MOUSEMOVE:
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_MBUTTONDOWN:
	case WM_MBUTTONUP:
	case WM_MOUSEWHEEL:
	case WM_XBUTTONDOWN:
	case WM_XBUTTONUP:
	case WM_MOUSEHOVER:
	case WM_MOUSEACTIVATE:
	{
		myCurrentMouseState = myMouse->GetState();
		myMouse->ProcessMessage(message, wParam, lParam);

		if (GetCurrentMousePosition().x + GetCurrentMousePosition().y != 0)
		{
			myTentativeMousePosition = GetCurrentMousePositionPoint();
		}
		return true;
	}
	}

	return false;
}


void InputManager::LockMouseScreen(HWND mWindow)
{
	RECT rect;
	GetClientRect(mWindow, &rect);

	POINT ul = { rect.left, rect.top };
	POINT lr = { rect.right, rect.bottom };

	MapWindowPoints(mWindow, nullptr, &ul, 1);
	MapWindowPoints(mWindow, nullptr, &lr, 1);

	rect.left = ul.x;
	rect.top = ul.y;

	rect.right = lr.x;
	rect.bottom = lr.y;

	ClipCursor(&rect);
	myMouse->SetMode(DirectX::Mouse::MODE_RELATIVE);
	if (IsMouseVisible())
	{
		ShowCursor(0);
		myMouse->SetVisible(false);
		myMouseIsVisible = false;
	}
}

void InputManager::UnlockMouseScreen()
{
	ClipCursor(nullptr);
	myMouse->SetMode(DirectX::Mouse::MODE_ABSOLUTE);
	if (!IsMouseVisible())
	{
		ShowCursor(1);
		myMouse->SetVisible(true);
		myMouseIsVisible = true;
	}
	//myMouse->IsVisible();
}

void InputManager::Update()
{
	myPreviousState = myCurrentState;
	myCurrentState = myTentativeState;
	myPreviousMouseState = myCurrentMouseState;
	myCurrentMouseState = {};
}
