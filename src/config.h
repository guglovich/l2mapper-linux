#pragma once

#include "_afx.h"

// ---------------------------------------------------------------------------
// config — настройки запуска.
//
// Оригинал показывал Win32-диалог (DialogBoxA / DlgProc).
// В Linux-версии параметры берутся из конфиг-файла "l2mapper.cfg"
// (простой key=value), либо остаются дефолтными.
// ---------------------------------------------------------------------------
class config
{
public:
    // Загружает "l2mapper.cfg" рядом с бинарником, если существует.
    // Возвращает true всегда (диалога нет).
    bool showDialog();

public:
    bool CFG_FULLSCREEN;
    int  CFG_SCREENRES_X;
    int  CFG_SCREENRES_Y;
    int  CFG_SCREENBITS;
    int  CFG_ANTIALIASING;

public:
    config();
    ~config() {}
};
