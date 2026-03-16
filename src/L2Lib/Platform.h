#pragma once

// ---------------------------------------------------------------------------
// Platform.h — диспетчер платформенного слоя.
// Включает нужную реализацию в зависимости от ОС.
// ---------------------------------------------------------------------------

#ifdef _WIN32
#  include "Platform_Win32.h"
#else
#  include "Platform_Linux.h"
#endif

// PTRINT — целое, достаточно широкое для хранения указателя
#ifdef __LP64__
typedef unsigned long PTRINT;
#else
typedef unsigned int  PTRINT;
#endif
