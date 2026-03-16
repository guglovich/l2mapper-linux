cmake_minimum_required(VERSION 3.16)
project(l2mapper CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ---------------------------------------------------------------------------
# Зависимости
# ---------------------------------------------------------------------------
find_package(OpenGL REQUIRED)
find_package(GLEW   REQUIRED)
find_package(glfw3  REQUIRED)
find_package(PkgConfig REQUIRED)

# ---------------------------------------------------------------------------
# MyGUI — явный поиск Engine + OpenGL3Platform
# ---------------------------------------------------------------------------
find_path(MYGUI_INCLUDE_DIR NAMES MyGUI.h
    PATHS
        /usr/include/MYGUI
        /usr/local/include/MYGUI
        /usr/include/mygui
        /usr/local/include/mygui
)

find_library(MYGUI_ENGINE_LIB
    NAMES MyGUIEngine libMyGUIEngine
    PATHS /usr/lib /usr/local/lib /usr/lib64
)

find_library(MYGUI_PLATFORM_LIB
    NAMES "MyGUI.OpenGL3Platform" "libMyGUI.OpenGL3Platform"
    PATHS /usr/lib /usr/local/lib /usr/lib64
)

if (NOT MYGUI_INCLUDE_DIR)
    message(FATAL_ERROR "MyGUI headers not found")
endif()
if (NOT MYGUI_ENGINE_LIB)
    message(FATAL_ERROR "MyGUIEngine library not found")
endif()
if (NOT MYGUI_PLATFORM_LIB)
    message(FATAL_ERROR "MyGUI.OpenGL3Platform library not found. Run: pacman -Ql mygui | grep so")
endif()

message(STATUS "MyGUI include: ${MYGUI_INCLUDE_DIR}")
message(STATUS "MyGUI engine:  ${MYGUI_ENGINE_LIB}")
message(STATUS "MyGUI platform: ${MYGUI_PLATFORM_LIB}")

# ---------------------------------------------------------------------------
# FreeImage
# ---------------------------------------------------------------------------
find_library(FREEIMAGE_LIBRARIES NAMES freeimage FreeImage)
find_path(FREEIMAGE_INCLUDE_DIRS NAMES FreeImage.h)
if (NOT FREEIMAGE_LIBRARIES)
    message(FATAL_ERROR "FreeImage not found. Install: sudo pacman -S freeimage")
endif()
message(STATUS "FreeImage: ${FREEIMAGE_LIBRARIES}")

# ---------------------------------------------------------------------------
# Исходники
# ---------------------------------------------------------------------------
file(GLOB L2LIB_SOURCES src/L2Lib/*.cpp)
list(FILTER L2LIB_SOURCES EXCLUDE REGEX "Platform_Win32\\.cpp$")
list(FILTER L2LIB_SOURCES EXCLUDE REGEX "/math\\.cpp$")

set(SOURCES
    src/main.cpp
    src/config.cpp
    src/L2Window.cpp
    src/core/L2Time.cpp
    src/core/L2InputManager.cpp
    src/scene/L2Camera.cpp
    src/scene/L2Scene.cpp
    src/scene/L2LevelManager.cpp
    src/L2TextureManager.cpp
    src/L2Geodata.cpp
    src/L2Skybox.cpp
    src/GUI/L2UIMain.cpp
    src/GUI/L2UIBaseWidget.cpp
    src/GUI/L2UIBusyScreen.cpp
    src/GUI/L2UIEditor.cpp
    src/GUI/L2UIMap.cpp
    ${L2LIB_SOURCES}
    src/L2Lib/Platform_Linux.cpp
)

add_executable(l2mapper ${SOURCES})

# ---------------------------------------------------------------------------
# Include dirs
# ---------------------------------------------------------------------------
target_include_directories(l2mapper PRIVATE
    src
    src/L2Lib
    ${OPENGL_INCLUDE_DIR}
    ${GLEW_INCLUDE_DIRS}
    ${FREEIMAGE_INCLUDE_DIRS}
    ${MYGUI_INCLUDE_DIR}
)

# ---------------------------------------------------------------------------
# Линковка
# ---------------------------------------------------------------------------
target_link_libraries(l2mapper
    pthread
    ${OPENGL_LIBRARIES}
    GLEW::GLEW
    glfw
    ${FREEIMAGE_LIBRARIES}
    ${MYGUI_ENGINE_LIB}
    ${MYGUI_PLATFORM_LIB}
)

# ---------------------------------------------------------------------------
# Флаги компилятора
# ---------------------------------------------------------------------------
target_compile_options(l2mapper PRIVATE
    -Wall
    -Wno-deprecated-declarations
    -Wno-unused-variable
    -Wno-sign-compare
    -Wno-write-strings
)


target_compile_definitions(l2mapper PRIVATE
    GL_GLEXT_PROTOTYPES=1
)

install(TARGETS l2mapper DESTINATION bin)
