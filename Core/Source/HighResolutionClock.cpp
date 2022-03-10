#include "../Include/HighResolutionClock.h"

using namespace std::chrono;

HighResolutionClock::HighResolutionClock()
	: mPrevTime(high_resolution_clock::now())
	, mDeltaTime(0)
	, mTotalTime(0)
{
}

void HighResolutionClock::Tick()
{
	auto currTime = high_resolution_clock::now();
	mDeltaTime = currTime - mPrevTime;
	mTotalTime += mDeltaTime;
	mPrevTime = currTime;
}

void HighResolutionClock::Reset()
{
	mPrevTime = high_resolution_clock::now();
	mDeltaTime = high_resolution_clock::duration();
	mTotalTime = high_resolution_clock::duration();
}

double HighResolutionClock::GetDeltaNanoseconds() const
{
	return mDeltaTime.count() * 1.0;
}

double HighResolutionClock::GetDeltaMicroseconds() const
{
	return mDeltaTime.count() * 1e-3;
}

double HighResolutionClock::GetDeltaMilliseconds() const
{
	return mDeltaTime.count() * 1e-6;
}

double HighResolutionClock::GetDeltaSeconds() const
{
	return mDeltaTime.count() * 1e-9;
}

double HighResolutionClock::GetTotalNanoseconds() const
{
	return mTotalTime.count() * 1.0;
}

double HighResolutionClock::GetTotalMicroseconds() const
{
	return mTotalTime.count() * 1e-3;
}

double HighResolutionClock::GetTotalMilliseconds() const
{
	return mTotalTime.count() * 1e-6;
}

double HighResolutionClock::GetTotalSeconds() const
{
	return mTotalTime.count() * 1e-9;
}