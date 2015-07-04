#pragma once

#include <Windows.h>

namespace o2
{
	/** Timer class, serves for measure the time. */
	class Timer
	{
		LONGLONG      mLastElapsedTime;
		LARGE_INTEGER mFrequency;
		LARGE_INTEGER mStartTime;

	public:
		/** ctor. */
		Timer();

		/** dtor. */
		~Timer();

		/** Resets time. */
		void Reset();

		/** Returns time in seconds from last Reset() call. */
		float GetTime();

		/** Returns time in seconds from last Reset() or GetElapsedTime() call. */
		float GetElapsedTime();
	};
}