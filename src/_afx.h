#pragma once

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <string>
#include <fstream>
#include <cstdint>

// GL/GLEW — до любых других GL заголовков
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>

// GLFW
#include <GLFW/glfw3.h>

// MyGUI — OpenGL3 платформа (единственная доступная на Arch)
#include "MyGUI.h"
#include "MyGUI_OpenGL3Platform.h"

// FreeImage
#include <FreeImage.h>

// L2Lib
#include "L2Lib/UEngine.h"

// Типы
typedef int64_t    int64;
typedef int32_t    int32;
typedef int16_t    int16;
typedef int8_t     int8;
typedef uint64_t   uint64;
typedef uint32_t   uint32;
typedef uint16_t   uint16;
typedef uint8_t    uint8;

#define _PI 3.141592653589793

#ifndef GL_CLAMP_TO_EDGE
#  define GL_CLAMP_TO_EDGE 0x812F
#endif

// VK алиасы → GLFW коды
#define VK_SPACE    GLFW_KEY_SPACE
#define VK_CONTROL  GLFW_KEY_LEFT_CONTROL
#define VK_ESCAPE   GLFW_KEY_ESCAPE
