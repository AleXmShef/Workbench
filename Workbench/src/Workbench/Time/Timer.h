#pragma once
#include "wbpch.h"

namespace Workbench {

	class Timer {
	public:
		Timer() = default;

		void Start();
		void Tick();
		void Pause();
		void Resume();
		void Reset();

		template<typename Type = std::ratio<1>>
		float GetAbsoluteTime() {
			return (std::chrono::duration_cast<std::chrono::duration<float, Type>>(m_TimeSinceStart)).count();
		};

		template<typename Type = std::ratio<1>>
		float GetTickTime() {
			return (std::chrono::duration_cast<std::chrono::duration<float, Type>>(m_TimeSinceLastTick)).count();
		};

	protected:
		bool m_isOnPause = true;

		std::chrono::steady_clock m_Clock;

		std::chrono::time_point<std::chrono::steady_clock> m_AbsoluteStartTime;

		std::chrono::time_point<std::chrono::steady_clock> m_LastTickTime;

		std::chrono::duration<int64_t, std::nano> m_TimeSinceStart;
		std::chrono::duration<int64_t, std::nano> m_TimeSinceLastTick;

	};

	class GameTimer : public Timer {
	public:
		float GetFps();
		float GetMsPerFrame();
	};
}

#define WB_GAME_TIMER GameTimer

