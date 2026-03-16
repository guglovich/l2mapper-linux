#include "config.h"

#include <cstdio>
#include <cstring>

config::config()
{
    // Дефолтные значения
    CFG_FULLSCREEN   = false;
    CFG_SCREENRES_X  = 1280;
    CFG_SCREENRES_Y  = 800;
    CFG_SCREENBITS   = 32;
    CFG_ANTIALIASING = 0;

    // Пытаемся загрузить l2mapper.cfg
    FILE* f = fopen("l2mapper.cfg", "r");
    if (!f) return;

    char line[256];
    while (fgets(line, sizeof(line), f))
    {
        // Пропускаем комментарии и пустые строки
        if (line[0] == '#' || line[0] == '\n' || line[0] == '\r') continue;

        char key[64], val[64];
        if (sscanf(line, "%63[^=]=%63s", key, val) != 2) continue;

        // Убираем пробелы из ключа
        char* k = key;
        while (*k == ' ') ++k;
        char* end = k + strlen(k) - 1;
        while (end > k && (*end == ' ' || *end == '\r' || *end == '\n')) *end-- = '\0';

        if (strcmp(k, "fullscreen") == 0)
            CFG_FULLSCREEN = (atoi(val) != 0);
        else if (strcmp(k, "width") == 0)
            CFG_SCREENRES_X = atoi(val);
        else if (strcmp(k, "height") == 0)
            CFG_SCREENRES_Y = atoi(val);
        else if (strcmp(k, "antialiasing") == 0)
            CFG_ANTIALIASING = atoi(val);
    }
    fclose(f);
}

bool config::showDialog()
{
    // На Linux диалога нет — просто возвращаем true.
    // Настройки уже считаны в конструкторе из l2mapper.cfg.
    return true;
}
