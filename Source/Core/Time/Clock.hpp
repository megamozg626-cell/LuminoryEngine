#pragma once

#include <chrono>
#include <cstdint>

namespace Luminary {

class Clock {
public:
    Clock();
    ~Clock() = default;

    void Start();
    void Stop();
    double Tick();

    double GetElapsedTime() const;
    uint64_t GetTickCount() const { return m_TickCount; }
    double GetDeltaTime() const { return m_DeltaTime; }

private:
    using HighResolutionClock = std::chrono::high_resolution_clock;
    using TimePoint = HighResolutionClock::time_point;

    TimePoint m_StartTime;
    TimePoint m_LastTickTime;
    double m_DeltaTime = 0.0;
    uint64_t m_TickCount = 0;
    bool m_IsRunning = false;
};

} // namespace Luminary
