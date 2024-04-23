#pragma once
#include <thread>
#include <engine/application/Application.h>

class Engine;
class Game;

class GameApplication : public Application {
public:
	GameApplication();
	~GameApplication() override;

	void Init(HINSTANCE hInstance, WNDPROC wndProc) override;
	void Update(const TimerData& aTimerData) override;
	void Render() override;

	bool UpdateSplashScreen(const float& dt);

	bool BeginFrame() override;
	void EndFrame() override;

	static LRESULT WindowProcedure(_In_ HWND hWnd, _In_ UINT uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam);
	Game* GetGame() { return myGame; }

private:
	Game* myGame = nullptr;

	//std::thread myGameInitThread = {};
};
