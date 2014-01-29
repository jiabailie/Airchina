#include <cstdio>
#include <cstdlib>
#include <string>
#include "../include/path.h"
#include "../include/graphics.h"

int
graphics::open(const char *path, BMPIMAGE image, BMPINFO info)
{
    int rgb = 0;
    long bmpSize = 0;
    
    FILE *pFile = 0;
    
    fopen_s(&pFile, path, "rb");
    
    if(!pFile)
    {
        printf("File %s is not exist.\n", path);
        return FILE_ACCESS_STATUS_NOT_EXIST;
    }
    
    fseek(pFile, 0, SEEK_END);
    bmpSize = ftell(pFile);
    rewind(pFile);
    
    fread(&(image->fileHeader), 1, FILEHEADERSIZE, pFile);
    fread(&(image->infoHeader), 1, INFOHEADERSIZE, pFile);
    
    info->w = image->infoHeader.biWidth;
    info->h = image->infoHeader.biHeight;
    info->bitCount = image->infoHeader.biBitCount;
    
    switch(image->infoHeader.biBitCount)
    {
    case 1:
        rgb = 2;
        break;
    case 4:
        rgb = 16;
        break;
    case 8:
        rgb = 256;
        break;
    case 24:
        rgb = 0;
        break;
    default:
        break;
    }
    
    fread(&(image->rgbQuad), rgb * sizeof(RGBQUAD), 1, pFile);
    if(image->infoHeader.biBitCount == 24)
    {
        info->bmpSize = bmpSize - BMPHEADERSIZE;
        info->data = (byte*)malloc(sizeof(byte) * (bmpSize - BMPHEADERSIZE));
        fread(info->data, 1, bmpSize - BMPHEADERSIZE, pFile);
    }
    
    info->lenLine = ((image->infoHeader.biBitCount * image->infoHeader.biWidth) + 31) / 8;
    
    fclose(pFile);
    
    return FILE_ACCESS_STATUS_SUCCESS;
}

void
graphics::save(const char *path, BMPINFO info)
{
    save(path, info->w, info->h, info->data);
}

void
graphics::save(const char *path, long width, long height, byte *data)
{
    FILE *pFile = 0;
    long lineLength = 0;
    long totalLength = 0;
    long mask = 0x000000ff;
    
    int byteperpixel = 3;
    
    byte header[BMPHEADERSIZE] = {
        0x42, 0x4d, 0, 0, 0, 0, 0, 0, 0, 0, 54, 0, 0, 0,
        40, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 24, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0};
    
    long fileSize = width * height * byteperpixel + BMPHEADERSIZE;

    header[2] = byte(fileSize         & mask);
    header[3] = byte((fileSize >> 8)  & mask);
    header[4] = byte((fileSize >> 16) & mask);
    header[5] = byte((fileSize >> 24) & mask);
    
    header[18] = byte(width    & mask);
    header[19] = byte((width >> 8)  & mask);
    header[20] = byte((width >> 16) & mask);
    header[21] = byte((width >> 24) & mask);

    header[22] = byte(height         & mask);
    header[23] = byte((height >> 8)  & mask);
    header[24] = byte((height >> 16) & mask);
    header[25] = byte((height >> 24) & mask);
    
    fopen_s(&pFile, path, "wb");
    
    if(!pFile)
    {
        printf("Error.\n");
        exit(0);
    }
    else
    {
        fwrite(header, sizeof(byte), BMPHEADERSIZE, pFile);
        
        lineLength = width * byteperpixel;
        
        while(lineLength % 4 != 0)
        {
            lineLength++;
        }
        
        totalLength = lineLength * height;
        
        fwrite(data, sizeof(byte), size_t(totalLength), pFile);
        fclose(pFile);
        
        printf("Save file success.\n");
    }    
}