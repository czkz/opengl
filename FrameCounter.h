#pragma once
#include <Stopwatch.h>

class FrameCounter {
    Stopwatch stopwatch;
public:
    float deltaTime = 0;
    size_t frameCount = 0;
    void tick() {
        deltaTime = stopwatch.tick();
        frameCount++;
    }
};
