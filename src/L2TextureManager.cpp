#include "L2TextureManager.h"

L2TextureManager::L2TextureManager(void) {}
L2TextureManager::~L2TextureManager(void) {}

GLuint L2TextureManager::get(char* fileName)
{
    return get(fileName, false);
}

GLuint L2TextureManager::get(char* fileName, bool clampToEdge)
{
    // Загружаем через FreeImage
    FREE_IMAGE_FORMAT fif = FreeImage_GetFileType(fileName, 0);
    if (fif == FIF_UNKNOWN)
        fif = FreeImage_GetFIFFromFilename(fileName);

    if (fif == FIF_UNKNOWN) {
        fprintf(stderr, "l2mapper: unknown format: %s\n", fileName);
        return 0;
    }

    FIBITMAP* raw = FreeImage_Load(fif, fileName, 0);
    if (!raw) {
        fprintf(stderr, "l2mapper: cannot load texture: %s\n", fileName);
        return 0;
    }

    // Конвертируем в 32-bit BGRA
    FIBITMAP* image = FreeImage_ConvertTo32Bits(raw);
    FreeImage_Unload(raw);
    if (!image) {
        fprintf(stderr, "l2mapper: cannot convert texture: %s\n", fileName);
        return 0;
    }

    int width  = (int)FreeImage_GetWidth(image);
    int height = (int)FreeImage_GetHeight(image);

    // FreeImage хранит снизу-вверх и BGRA — конвертируем в RGBA сверху-вниз
    uint8* pixels = new uint8[width * height * 4];
    for (int y = 0; y < height; y++) {
        BYTE* src = FreeImage_GetScanLine(image, height - 1 - y);
        uint8* dst = pixels + y * width * 4;
        for (int x = 0; x < width; x++) {
            dst[x*4+0] = src[x*4 + FI_RGBA_RED];
            dst[x*4+1] = src[x*4 + FI_RGBA_GREEN];
            dst[x*4+2] = src[x*4 + FI_RGBA_BLUE];
            dst[x*4+3] = src[x*4 + FI_RGBA_ALPHA];
        }
    }
    FreeImage_Unload(image);

    GLuint tex;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);

    gluBuild2DMipmaps(GL_TEXTURE_2D, 4, width, height,
                      GL_RGBA, GL_UNSIGNED_BYTE, pixels);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    if (clampToEdge) {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    }

    delete[] pixels;
    return tex;
}
