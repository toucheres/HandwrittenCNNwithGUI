#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <cstdint>

#pragma pack(push, 1)
struct BITMAPFILEHEADER
{
    uint16_t bfType = 0x4D42; // 'BM'
    uint32_t bfSize = 184;    // 与1.bmp一致
    uint16_t bfReserved1 = 0;
    uint16_t bfReserved2 = 0;
    uint32_t bfOffBits = 62; // 文件头(14)+信息头(40)+调色板(8)=62
};

struct BITMAPINFOHEADER
{
    uint32_t biSize = 40; // 与1.bmp一致
    int32_t biWidth;
    int32_t biHeight;
    uint16_t biPlanes = 1;
    uint16_t biBitCount = 1;
    uint32_t biCompression = 0;
    uint32_t biSizeImage;
    int32_t biXPelsPerMeter = 0;
    int32_t biYPelsPerMeter = 0;
    uint32_t biClrUsed = 0;
    uint32_t biClrImportant = 0;
};

struct RGBQUAD
{
    uint8_t rgbBlue;
    uint8_t rgbGreen;
    uint8_t rgbRed;
    uint8_t rgbReserved;
};
#pragma pack(pop)
