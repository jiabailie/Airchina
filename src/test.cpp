#include <cstdio>
#include <cstdlib>
#include <string>
#include <iostream>
#include "../include/config.h"
#include "../include/type.h"
#include "../include/bitmap.h"
#include "../include/constant.h"
#include "../include/graphics.h"
#include "../include/templates.h"
#include "../include/common.h"
#include "../include/process.h"

common gc;
process gp;
graphics gg;

void test_changeInt2CharArray()
{
    int t = -1008;
    char *ary = 0;

    gc.changeInt2CharArray(t, ary);

    std::cout << ary << std::endl;
}

void test_findNonZeroBits()
{
    int t = 987;
    bool *ary = 0;

    gc.findNonZeroBits(t, ary);

    for(int i = 0; i <= NumLength; i++)
    {
        std::cout << (ary[i] ? 1 : 0);
    }
    std::cout << std::endl;
}

void test_replaceFilePathForTemplate()
{
    char c = 'a';
    char path[100] = "../../c.jpg";

    gc.replaceFilePathForTemplate(c, path);

    std::cout << path << std::endl;
}

void test_calPow()
{
    std::cout << gc.calPow(ll(2), ll(10)) << std::endl;
}

void test_removePixels()
{
    byte *data = 0;
    int w = 0, h = 0;

    char path[400] = "E:/Airchina/bmp/samples/1.bmp";

    gp.removePixels(w, h, path, data);

    free(data);
    data = 0;
}

void test_imageSegment2Four()
{
	byte *data = 0;
	byte *rdata = 0;

	int w = 0, h = 0;
    char path[400] = "E:/Airchina/bmp/samples/1.bmp";

    gp.removePixels(w, h, path, data);

	rdata = (byte*)malloc(sizeof(byte) * w * h * 3);
    gp.recoverImage(w, h, data, rdata);
	gg.save("E:/Airchina/bmp/data/12_test_imageSegment2Four.bmp", w, h, rdata);

	gc.imageSegment2Four(w, h, rdata);

    free(data);
	free(rdata);

    data = 0;
	rdata = 0;
}

int main()
{
	test_imageSegment2Four();

    return 0;
}