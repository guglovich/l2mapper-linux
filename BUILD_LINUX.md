# l2mapper — Linux port

## Зависимости

```bash
# Ubuntu / Debian
sudo apt install \
    build-essential cmake \
    libglfw3-dev \
    libglew-dev \
    libfreeimage-dev \
    libmygui-dev \
    libgl-dev \
    libglu-dev
```

```bash
# Arch / Manjaro
sudo pacman -S cmake glfw glew freeimage mygui
```

```bash
# Fedora / RHEL
sudo dnf install cmake glfw-devel glew-devel freeimage-devel
# MyGUI может потребоваться собрать вручную (см. ниже)
```

## Сборка

```bash
git clone <repo>
cd l2mapper-linux

cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j$(nproc)
```

## Структура размещения файлов

Бинарник ожидает клиент L2 в родительской директории:

```
/some/path/
├── l2ct3_GoD/          ← папка L2-клиента
│   ├── Maps/
│   ├── Textures/
│   ├── StaticMeshes/
│   └── ...
└── build/              ← сюда кладём собранный l2mapper
    ├── l2mapper
    ├── l2mapper.cfg     ← настройки (создаётся вручную, необязательно)
    ├── geo_nswe.png
    ├── resources.xml
    ├── geodata/         ← *.l2j файлы геодаты (опционально)
    └── data/
        └── textures/
            └── skybox/
```

## Конфигурация (l2mapper.cfg)

Создайте рядом с бинарником файл `l2mapper.cfg` (все параметры опциональны):

```ini
# Разрешение окна
width=1280
height=800

# Полноэкранный режим (0/1)
fullscreen=0

# Мультисэмплинг: 0, 2, 4 (MSAA)
antialiasing=0
```

## Управление (без изменений)

- **ПКМ + мышь** — вращение камеры
- **W/A/S/D** — движение
- **Space** — вверх
- **Ctrl** — вниз
- **Escape** — выход

## Что изменилось в порте

| Оригинал (Windows) | Linux-порт |
|---|---|
| `WinMain` + Win32 message loop | `main()` + GLFW event loop |
| `L2Window` (HWND/HDC/HGLRC/wgl) | `L2Window` через `GLFWwindow*` |
| `GetTickCount()` | `glfwGetTime()` |
| `GetCursorPos` / `SetCursorPos` | `glfwGetCursorPos` / `glfwSetCursorPos` |
| `config` Win32 диалог (DialogBoxA) | Файл `l2mapper.cfg` |
| `OpenGLImageLoader_Gdiplus` (GDI+) | `OpenGLImageLoader_FreeImage` |
| `Platform_Win32.cpp` (_findfirst) | `Platform_Linux.cpp` (opendir/readdir) |
| `_afx.h` (windows.h, __int64) | `_afx.h` (GLFW, cstdint) |
| `UBase.h` (windows.h, wglext.h) | `UBase.h` (GL/glew.h, GL/glext.h) |
| `l2mapper.vcxproj` | `CMakeLists.txt` |

Весь L2-специфичный код (`src/L2Lib/U*.cpp`) — без изменений.
