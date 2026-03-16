#include "L2Time.h"

// glfwGetTime() возвращает секунды (double).
// Переводим в миллисекунды и кастуем в uint16 — сохраняем семантику
// оригинала (GetTickCount возвращал DWORD ~миллисекунды).
static inline uint16 now_ms()
{
    return static_cast<uint32>(glfwGetTime() * 1000.0);
}

L2Time::L2Time()
    : _ticks(0), _ticksPrev(now_ms()), _ticksDelta(0)
{}

void L2Time::update()
{
    _ticks      = now_ms();
    _ticksDelta = _ticks - _ticksPrev;
    _ticksPrev  = _ticks;
}
