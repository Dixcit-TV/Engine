#pragma once
#include <profileapi.h>

class Timer
{
public:
	Timer()
		: SecPerCount(0.0)
		, DeltaTime(0.0)
		, PrevTime(0)
		, CurrentTime(0)
		, StartTime(0)
		, StopTime(0)
		, TotalPauseDuration(0)
		, bPaused(false)
	{
		__int64 countsPerSec;
		QueryPerformanceFrequency((LARGE_INTEGER*)&countsPerSec);
		SecPerCount = 1.0 / (double)countsPerSec;
	}

	void Reset()
	{
		QueryPerformanceCounter((LARGE_INTEGER*)&CurrentTime);

		PrevTime = CurrentTime;
		StartTime = CurrentTime;
		StopTime = 0;
		TotalPauseDuration = 0;

		bPaused = false;
	}

	void Tick()
	{
		if (!bPaused)
		{
			QueryPerformanceCounter((LARGE_INTEGER*)&CurrentTime);
			DeltaTime = max((CurrentTime - PrevTime) * SecPerCount, 0.0);
			PrevTime = CurrentTime;
		}
		else
		{
			DeltaTime = 0;
		}
	}

	void Stop()
	{
		if (bPaused) return;

		bPaused = true;
		QueryPerformanceCounter((LARGE_INTEGER*)&StopTime);
	}

	void Start()
	{
		if (!bPaused) return;

		bPaused = false;
		QueryPerformanceCounter((LARGE_INTEGER*)&CurrentTime);
		TotalPauseDuration += max(CurrentTime - StopTime, 0);
		StopTime = 0;
		PrevTime = CurrentTime;
	}

	double GetDeltaTime() const { return DeltaTime; }
	double GetTotalTime() const 
	{ 
		__int64 internalCurrTime = bPaused ? StopTime : CurrentTime;
		return (internalCurrTime - StartTime - TotalPauseDuration) * SecPerCount;
	}

private:
	double SecPerCount;
	double DeltaTime;

	__int64 PrevTime;
	__int64 CurrentTime;
	__int64 StartTime;
	__int64 StopTime;
	__int64 TotalPauseDuration;

	bool bPaused;
};


struct FPSCounter
{
	typedef void (*UpdateCallback)(UINT, double);

	FPSCounter(UpdateCallback inCallback = nullptr, float inSecondsPerUpdate = 1.f) : Callback(inCallback), SecondsPerUpdate(inSecondsPerUpdate) {};

	inline void Update(double inDeltaTime) 
	{
		++FrameCount;

		FPSCounterElapsedTime += (float)inDeltaTime;
		if (FPSCounterElapsedTime >= SecondsPerUpdate)
		{
			UINT FPS = FrameCount - FrameCountSinceLastUpdate;
			float FrameTime = SecondsPerUpdate * 1000.f / FPS;

			Callback(FPS, FrameTime);

			FrameCountSinceLastUpdate = FrameCount;
			FPSCounterElapsedTime -= SecondsPerUpdate;
		}
	}

private:
	UpdateCallback Callback;
	float FPSCounterElapsedTime = 0.f;
	float SecondsPerUpdate = 1.f;
	UINT FrameCount = 0;
	UINT FrameCountSinceLastUpdate = 0;
};