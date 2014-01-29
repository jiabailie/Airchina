/*
 * January, 23rd, 2014.
 * Bitmap related data structs.
 */
#pragma once

#include "type.h"
#include "infofetch.h"

struct RGBQUAD
{
    byte rgbBlue;
    byte rgbGreen;
    byte rgbRed;
    byte rgbReserved;
};

struct FILEHEADER
{
    word  bfType;
    dword bfSize;
    word  bfReserved1;
    word  bfReserved2;
    dword bfOffBits;
};

struct INFOHEADER
{
    dword biSize;
    dword biWidth;
    dword biHeight;
    word  biPlanes;
    word  biBitCount;
    dword biCompression;
    dword biSizeImage;
    dword biXPelsPerMeter;
    dword biYPelsPerMeter;
    dword biClrUsed;
    dword biClrImportant;
};

struct bmpImage
{
    INFOHEADER infoHeader;
    FILEHEADER fileHeader;
    RGBQUAD    rgbQuad;
};

typedef RGBQUAD*    BITMAPRGBQUAD;
typedef FILEHEADER* BITMAPFILEHEADER;
typedef INFOHEADER* BITMAPINFOHEADER;
typedef bmpImage*   BMPIMAGE;
typedef infoFetch*  BMPINFO;