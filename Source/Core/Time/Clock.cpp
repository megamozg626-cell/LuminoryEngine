#include "Clock.hpp"

namespace Luminary {

Clock::Clock() {
    m_StartTime = HighResolutionClock::now();
    m_LastTickTime = m_StartTime;
}

void Clock::Start() {
    m_StartTime = HighResolutionClock::now();
    m_LastTickTime = m_StartTime;
    m_IsRunning = true;
}

void Clock::Stop() {
    m_IsRunning = false;
}

double Clock::Tick() {
    if (!m_IsRunning) return 0.0;

    auto currentTime = HighResolutionClock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(
        currentTime - m_LastTickTime);

    m_DeltaTime = duration.count() / 1e9;  // Convert to seconds
    m_LastTickTime = currentTime;
    m_TickCount++;

    return m_DeltaTime;
}

double Clock::GetElapsedTime() const {
    auto currentTime = HighResolutionClock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(
        currentTime - m_StartTime);
    return duration.count() / 1e9;  // Convert to seconds
}

} // namespace Luminary
