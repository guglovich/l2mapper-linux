#pragma once

// ---------------------------------------------------------------------------
// OpenGLImageLoader_FreeImage — загрузчик изображений для MyGUI OpenGL3
// ---------------------------------------------------------------------------

#include <cstdlib>
#include <cstring>
#include <FreeImage.h>

#include <MyGUI_DataManager.h>
#include <MyGUI_OpenGL3ImageLoader.h>
#include <MyGUI_OpenGL3DataManager.h>

class OpenGLImageLoader_FreeImage : public MyGUI::OpenGL3ImageLoader
{
public:
    OpenGLImageLoader_FreeImage()
    {
        FreeImage_Initialise();
    }

    virtual ~OpenGLImageLoader_FreeImage()
    {
        FreeImage_DeInitialise();
    }

    void* loadImage(int& _width, int& _height,
                    MyGUI::PixelFormat& _format,
                    const std::string& _filename) override
    {
        std::string fullname =
            MyGUI::OpenGL3DataManager::getInstance().getDataPath(_filename);

        FREE_IMAGE_FORMAT fif = FreeImage_GetFileType(fullname.c_str(), 0);
        if (fif == FIF_UNKNOWN)
            fif = FreeImage_GetFIFFromFilename(fullname.c_str());
        if (fif == FIF_UNKNOWN)
            return nullptr;

        FIBITMAP* raw = FreeImage_Load(fif, fullname.c_str(), 0);
        if (!raw) return nullptr;

        FIBITMAP* image = FreeImage_ConvertTo32Bits(raw);
        FreeImage_Unload(raw);
        if (!image) return nullptr;

        _width  = (int)FreeImage_GetWidth(image);
        _height = (int)FreeImage_GetHeight(image);
        _format = MyGUI::PixelFormat::R8G8B8A8;

        size_t size = (size_t)_width * _height * 4;
        unsigned char* result = new unsigned char[size];

        // FreeImage: строки снизу-вверх, пиксели в BGRA
        // MyGUI ожидает сверху-вниз RGBA
        for (int y = 0; y < _height; ++y)
        {
            BYTE* src = FreeImage_GetScanLine(image, _height - 1 - y);
            unsigned char* dst = result + y * _width * 4;
            for (int x = 0; x < _width; ++x)
            {
                dst[x*4+0] = src[x*4 + FI_RGBA_RED];
                dst[x*4+1] = src[x*4 + FI_RGBA_GREEN];
                dst[x*4+2] = src[x*4 + FI_RGBA_BLUE];
                dst[x*4+3] = src[x*4 + FI_RGBA_ALPHA];
            }
        }

        FreeImage_Unload(image);
        return result;
    }

    void saveImage(int _width, int _height,
                   MyGUI::PixelFormat _format,
                   void* _texture,
                   const std::string& _filename) override
    {
        // Реализация аналогична loadImage в обратную сторону — опционально
        (void)_width; (void)_height; (void)_format; (void)_texture; (void)_filename;
    }
};
