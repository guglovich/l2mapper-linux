#pragma once

#include "../_afx.h"

// ---------------------------------------------------------------------------
// L2Time — таймер на основе glfwGetTime() вместо Win32 GetTickCount()
// ---------------------------------------------------------------------------
class L2Time
{
public:
    void update();

    uint32 getTicks()       { return _ticks; }
    uint32 getTicksDelta()  { return _ticksDelta; }

public:
    L2Time();
    ~L2Time() {}

private:
    uint32 _ticks;
    uint32 _ticksPrev;
    uint32 _ticksDelta;
};
