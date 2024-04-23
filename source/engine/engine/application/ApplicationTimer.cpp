#include "pch.h"
#include "ApplicationTimer.h"

ApplicationTimer::ApplicationTimer()
{
    myLastTimePoint = std::chrono::high_resolution_clock::now();
    myTimerData.totalTime = 0.0;
    myTimerData.deltaTime = 0.0;
}

void ApplicationTimer::Update()
{
    auto currentTimePoint = std::chrono::high_resolution_clock::now();

    myTimerData.deltaTime = std::chrono::duration<float>(currentTimePoint - myLastTimePoint).count();
    myTimerData.totalTime += myTimerData.deltaTime;

    myLastTimePoint = currentTimePoint;
}
