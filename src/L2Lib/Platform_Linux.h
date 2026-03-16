#pragma once

// ---------------------------------------------------------------------------
// Platform_Linux.h — декларации платформенных функций для Linux.
// Реализация в Platform_Linux.cpp.
// ---------------------------------------------------------------------------

#include "UBase.h"
#include "jArray.h"

#define CM_SYSTEM_MAXNAME 256

// Возвращает список имён файлов в директории dir,
// соответствующих шаблону pattern (glob-стиль: "*.unr").
jfArray<char*, uint32>* UTIL_FindFiles(char* dir, char* pattern);
