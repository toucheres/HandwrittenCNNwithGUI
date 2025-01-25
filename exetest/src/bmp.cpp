#include <bmp.h>
#include <iostream>
#include <filesystem>
void createBlackWhiteBMP(const std::vector<int> &bw, const std::string &path)
{
    const int width = 30, height = 30;
    if (bw.size() != width * height)
        return;

    // 每行4字节, 共30行
    const int rowSize = 4, dataSize = rowSize * height;
    BITMAPFILEHEADER fileHeader;
    BITMAPINFOHEADER infoHeader;
    infoHeader.biWidth = width;
    infoHeader.biHeight = height;
    infoHeader.biSizeImage = dataSize;

    // 调色板: 0=黑,1=白
    RGBQUAD palette[2] = {
        {0xFF, 0xFF, 0xFF, 0x00}, 
        {0x00, 0x00, 0x00, 0x00}};

    std::vector<uint8_t> pixelData(dataSize, 0);

    // 按自下而上行序写入bits, 0=黑(0),1=白(1)
    for (int row = 0; row < height; ++row)
    {
        uint8_t *pRow = &pixelData[row * rowSize];
        for (int col = 0; col < width; ++col)
        {
            int bit = bw[(height - 1 - row) * width + col] & 1;
            pRow[col >> 3] |= bit << (7 - (col & 7));
        }
    }

    // 额外2字节拼至184字节总长
    uint8_t extraPad[2] = {0, 0};

    //找到目录路径
    std::string path_dir = path;
    size_t index_1 = path.rfind("/");
    size_t index_2 = path.rfind("\\");
    if (index_1 == std::string::npos) { index_1 = 0; }
    if (index_2 == std::string::npos) { index_2 = 0; }
    path_dir.resize(index_1>index_2?index_1:index_2);

    
    if (!std::filesystem::exists(path_dir))
    {
        //路径不存在
        std::filesystem::create_directories(path_dir);
    }

    std::ofstream out(path, std::ios::binary);
    if (!out)
    {
        std::wcout << "打开";
        std::cout<< path;
        std::wcout << "失败" << std::endl;
        return;
    }
    out.write(reinterpret_cast<const char *>(&fileHeader), sizeof(fileHeader));
    out.write(reinterpret_cast<const char *>(&infoHeader), sizeof(infoHeader));
    out.write(reinterpret_cast<const char *>(palette), sizeof(palette));
    out.write(reinterpret_cast<const char *>(pixelData.data()), pixelData.size());
    out.write(reinterpret_cast<const char *>(extraPad), sizeof(extraPad));
}