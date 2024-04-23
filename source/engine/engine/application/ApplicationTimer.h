#pragma once
#include <chrono>

struct TimerData
{
	float deltaTime;
	double totalTime;
};

class ApplicationTimer
{
public:
	ApplicationTimer();
	ApplicationTimer(const ApplicationTimer& aApplicationTimer) = delete;
	ApplicationTimer& operator=(const ApplicationTimer& aApplicationTimer) = delete;

	void Update();

	float GetDeltaTime() const { return myTimerData.deltaTime; }
	double GetTotalTime() const { return myTimerData.totalTime; }
	TimerData& GetTimerData() { return myTimerData; }
private:
	std::chrono::high_resolution_clock::time_point myLastTimePoint;
	TimerData myTimerData;
};
