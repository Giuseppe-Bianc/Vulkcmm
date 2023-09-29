#pragma once
#include "headers.h"
inline static constexpr long double TIMEFACTOR = 1e9L;
class FPSCounter {
public:
    inline FPSCounter() noexcept : startTime(std::chrono::high_resolution_clock::now()) {}

    void update() noexcept {
        frameCount++;
        const auto currentTime = std::chrono::high_resolution_clock::now();
        const auto elapsedTime = std::chrono::duration<long double, std::nano>(currentTime - startTime).count();

        if(elapsedTime >= TIMEFACTOR) {  // Update FPS every 1 second (1e9 nanoseconds)
            fps = C_L(frameCount / (elapsedTime / TIMEFACTOR));
            frameCount = 0;
            startTime = currentTime;
        }
    }

    [[nodiscard]] inline long getFPS() const noexcept { return fps; }

private:
    long frameCount{};
    long fps{};
    std::chrono::high_resolution_clock::time_point startTime;
};
