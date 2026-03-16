// ---------------------------------------------------------------------------
// Platform_Linux.cpp — реализация UTIL_FindFiles через POSIX opendir/readdir.
//
// Заменяет Platform_Win32.cpp (_findfirst / _findnext / _findclose).
// ---------------------------------------------------------------------------

#include "UBase.h"
#include "Platform_Linux.h"
#include "UPackage.h"
#include "utils.h"

#include <dirent.h>
#include <fnmatch.h>
#include <unistd.h>
#include <cstring>
#include <cstdlib>

jfArray<char*, uint32>* UTIL_FindFiles(char* dir, char* pattern)
{
    jfArray<char*, uint32>* result = new jfArray<char*, uint32>();

    // Сохраняем текущую директорию
    char savedDir[CM_SYSTEM_MAXNAME];
    if (!getcwd(savedDir, sizeof(savedDir)))
        return result;

    // Переходим в нужную директорию
    if (chdir(dir) == -1)
        return result;

    DIR* d = opendir(".");
    if (!d)
    {
        chdir(savedDir);
        return result;
    }

    struct dirent* entry;
    while ((entry = readdir(d)) != nullptr)
    {
        // Пропускаем директории
        if (entry->d_type == DT_DIR) continue;

        // Сопоставляем имя с шаблоном (fnmatch понимает *.unr и т.д.)
        // FNM_CASEFOLD — регистронезависимо, как на Windows (_findfirst)
        if (fnmatch(pattern, entry->d_name, FNM_CASEFOLD) == 0)
            result->add(UTIL_CopyString(entry->d_name));
    }

    closedir(d);
    chdir(savedDir);

    return result;
}
