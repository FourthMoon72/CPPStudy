#include "Timer.h"
#include <Windows.h>

namespace MyUtil
{
	Timer::Timer() : 
		mdSecondsPerCount{}, 
		mdDeltaTime{ -1.0 },
		mfScale { 1.0 },
		mllBaseTime{},
		mllPausedTime{},
		mllPrevTime{},
		mllCurrTime{},
		mllStopTime{},
		mbStopped{}
	{
		long long countsPerSec;
		QueryPerformanceFrequency((LARGE_INTEGER*)&countsPerSec);

		mdSecondsPerCount = 1.0 / static_cast<double>(countsPerSec);
		//CPU가 한번 일할때 걸리는 시간
	}

	void Timer::Start()
	{
		long long currTime;
		QueryPerformanceCounter((LARGE_INTEGER*)&currTime);

		mllBaseTime = currTime;
		mllPrevTime = currTime;
		mllStopTime = 0;
		mbStopped = false;
	}

	void Timer::Stop()
	{
		if (!mbStopped)
		{
			long long currTime;
			QueryPerformanceCounter((LARGE_INTEGER*)&currTime);

			mllStopTime = currTime;
			mbStopped = true;
		}
	}

	void Timer::Resume()
	{
		long long currTime;
		QueryPerformanceCounter((LARGE_INTEGER*)&currTime);

		if (mbStopped)
		{
			mllPausedTime += (currTime - mllStopTime);

			mllPrevTime = currTime;
			mllStopTime = 0;
			mbStopped = false;
		}
	}

	void Timer::Update()
	{
		if (mbStopped)
		{
			mdDeltaTime = 0.0f;
			return;
		}

		long long currTime;
		QueryPerformanceCounter((LARGE_INTEGER*)&currTime);
		mllCurrTime = currTime;

		mdDeltaTime = (mllCurrTime - mllPrevTime) * mdSecondsPerCount;
		
		mllPrevTime = currTime;

		if (mdDeltaTime < 0.0)
		{
			mdDeltaTime = 0.0;
		}
	}

	float Timer::TotalTime() const
	{
		if(mbStopped)
		{
			return static_cast<float>(((mllStopTime - mllPausedTime) - mllBaseTime) * mdSecondsPerCount);
		}
		else
		{
			return static_cast<float>(((mllCurrTime - mllPausedTime) - mllBaseTime) * mdSecondsPerCount);
		}
	}

	float Timer::DeltaTime() const
	{
		return static_cast<float>(mdDeltaTime * mfScale);
	}

	void Timer::SetScale(float scale)
	{
		mfScale = scale;
	}

}