#include <bmp.h>
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

    std::ofstream out(path, std::ios::binary);
    if (!out)
        return;
    out.write(reinterpret_cast<const char *>(&fileHeader), sizeof(fileHeader));
    out.write(reinterpret_cast<const char *>(&infoHeader), sizeof(infoHeader));
    out.write(reinterpret_cast<const char *>(palette), sizeof(palette));
    out.write(reinterpret_cast<const char *>(pixelData.data()), pixelData.size());
    out.write(reinterpret_cast<const char *>(extraPad), sizeof(extraPad));
}