#pragma once

// ============================================================================
// Совместимость с не-MSVC компиляторами
// ============================================================================
#ifndef _MSC_VER
#  define __forceinline   __attribute__((always_inline)) inline
#  define __declspec(x)
#  define __cdecl
#  define __stdcall
#endif

// ============================================================================
// Стандартные заголовки
// ============================================================================
#include <cstdlib>
#include <cstdio>
#include <ctime>
#include <cstring>
#include <cstdarg>
#include <cstdint>
#include <exception>
#include <cmath>

// ============================================================================
// OpenGL (GLEW до GL)
// ============================================================================
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>

// ============================================================================
// Windows-типы которые использует L2Lib.
// Используем #ifndef чтобы не конфликтовать если они уже определены
// (например MyGUI или другие библиотеки могут их определять).
// ============================================================================
#ifndef FLOAT
typedef float           FLOAT;
#endif
#ifndef UINT
typedef unsigned int    UINT;
#endif
#ifndef INT
typedef int             INT;
#endif
#ifndef DWORD
typedef uint32_t        DWORD;
#endif
#ifndef WORD
typedef uint16_t        WORD;
#endif
#ifndef BYTE
typedef uint8_t         BYTE;
#endif
#ifndef BOOL
typedef int             BOOL;
#endif
#ifndef LPCSTR
typedef const char*     LPCSTR;
#endif
#ifndef LPSTR
typedef char*           LPSTR;
#endif

// GCC_ALIGN — выравнивание памяти
#ifdef __GNUC__
#  define GCC_ALIGN(n) __attribute__((aligned(n)))
#else
#  define GCC_ALIGN(n)
#endif

// strcasecmp вместо stricmp на Linux
#ifndef _WIN32
#  include <strings.h>
#  define stricmp strcasecmp
#endif

// ============================================================================
// Целочисленные типы L2Lib (MaNGOS-совместимые)
// ============================================================================
typedef int64_t     int64;
typedef int32_t     int32;
typedef int16_t     int16;
typedef int8_t      int8;
typedef uint64_t    uint64;
typedef uint32_t    uint32;
typedef uint16_t    uint16;
typedef uint8_t     uint8;

// ============================================================================
// DLLEXPORT / DBSPAPI / DLIBAPI
// ============================================================================
#ifdef _MSC_VER
#  define DBSPAPI   __cdecl
#  define DLIBAPI   __stdcall
#  define DLLEXPORT
#else
#  define DBSPAPI
#  define DLIBAPI
#  define DLLEXPORT
#endif

// ============================================================================
// Debug макросы
// ============================================================================
#ifndef _WIN32
#  define PRINTF_FORMAT(b,c) __attribute__((__format__(__printf__,b,c)))
#else
#  define PRINTF_FORMAT(b,c)
#endif

#ifdef NDEBUG
#  undef _DEBUG
#  undef DEBUG
#endif

#define GL_GLEXT_PROTOTYPES 1

#define PATHSEPARATOR(c)  ( (c) == '\\' || (c) == '/' )
#define LOWERCASE(c)      ( (c) <= 'Z' && (c) >= 'A' ? (c) + ('a'-'A') : (c) )
#define UPPERCASE(c)      ( (c) <= 'z' && (c) >= 'a' ? (c) - ('a'-'A') : (c) )

#define ENG_MAXNAME 48

#define CHECK(n)    if( n ) __excp_assertsfunc( #n, __FILE__, __LINE__ );
#define CHECKS(n,s) if( n ) __excp_assertsfunc(  s, __FILE__, __LINE__ );

#ifdef DEBUG
#  define ASSERT(expr) if (!(expr)) __excp_assertsfunc( #expr, __FILE__, __LINE__)
#else
#  define ASSERT(expr)
#endif

// ============================================================================
// Singleton, math
// ============================================================================
#include "Singleton.h"
#include "l2math.h"

#define g_math ::Singleton<math>::get_instance()

// ============================================================================
// Сериализатор UPackage — макрос генерирует operator>> для lvalue и rvalue
// ============================================================================
#define IMPLEMENT_USERIALIZER(cl) \
    friend inline UPackage& operator >> (UPackage& Ar, cl& Obj)  { return Obj.Serialize(Ar); } \
    friend inline UPackage& operator >> (UPackage& Ar, cl&& Obj) { return Obj.Serialize(Ar); }

#define UPACKAGE_UNREAL2_VERSION 100

// ============================================================================
// Константы Unreal Engine
// ============================================================================
const FLOAT P2UScale = 50.0f;
const FLOAT U2PScale = 0.02f;

const FLOAT Rad2U = 10430.2192f;
const FLOAT U2Rad = 0.000095875262f;

// ============================================================================
// Forward declarations (нужны для IMPLEMENT_USERIALIZER и шаблонов)
// ============================================================================
class       UPackage;
struct      UImport;
struct      UExport;

class       UObject;
class           AActor;
class               UBrush;
class               UStaticMeshActor;
class           UPrimitive;
class               UModel;
class               UPolys;
class               UStaticMesh;
class           UMaterial;
class               UTexture;
class           ULevel;
class           UTerrain;

class UString;

template <typename T, typename T1> class jfArray;

extern class FGlobalMath GMath;

#include "utils.h"
#include "Platform.h"

// Windows bit-manipulation macros
#ifndef LOWORD
#  define LOWORD(x) ((uint16)((uint32)(x) & 0xFFFF))
#endif
#ifndef HIWORD
#  define HIWORD(x) ((uint16)(((uint32)(x) >> 16) & 0xFFFF))
#endif
#ifndef LOBYTE
#  define LOBYTE(x) ((uint8)((x) & 0xFF))
#endif
#ifndef HIBYTE
#  define HIBYTE(x) ((uint8)(((x) >> 8) & 0xFF))
#endif
#ifndef MAKELONG
#  define MAKELONG(lo, hi) ((uint32)(uint16)(lo) | ((uint32)(uint16)(hi) << 16))
#endif

// min/max — в C++ они в <algorithm>, на MSVC были доступны глобально
#include <algorithm>
using std::min;
using std::max;

// TRUE/FALSE — Windows-макросы, на Linux не определены
#ifndef TRUE
#  define TRUE  1
#endif
#ifndef FALSE
#  define FALSE 0
#endif
