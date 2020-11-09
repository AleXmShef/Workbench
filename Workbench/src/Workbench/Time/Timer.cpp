#include "wbpch.h"
#include "Timer.h"

namespace Workbench {

	void Timer::Start() {
		m_AbsoluteStartTime = m_Clock.now();

		m_LastTickTime = m_AbsoluteStartTime;

		m_TimeSinceStart = m_TimeSinceStart.zero();
		m_TimeSinceLastTick = m_TimeSinceLastTick.zero();

		m_isOnPause = false;
	}

	void Timer::Reset() {
		Start();
	}

	void Timer::Tick() {
		if (!m_isOnPause) {
			auto newTime = m_Clock.now();

			m_TimeSinceStart = newTime - m_AbsoluteStartTime;
			m_TimeSinceLastTick = newTime - m_LastTickTime;

			m_LastTickTime = newTime;
		}
	}

	void Timer::Pause() {
		//Is it needed??
		//Tick();
		m_isOnPause = true;
	}

	void Timer::Resume() {
		m_isOnPause = false;
		Tick();
	}
}
