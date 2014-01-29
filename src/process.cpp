#include <cstdio>
#include <cstdlib>
#include <climits>
#include "../include/config.h"
#include "../include/graphics.h"
#include "../include/bitmap.h"
#include "../include/process.h"
#include "../include/minifunc.h"
#include "../include/common.h"

#define DEBUG_OUTPUT

#ifdef  NORMAL_MODE
    #undef DEBUG_OUTPUT
#endif

using namespace std;

common gp_common;
graphics gp_graphics;

#ifdef DEBUG_OUTPUT
byte *dfake = 0;
BMPINFO g_info = (infoFetch*)malloc(sizeof(infoFetch));
#endif

void
process::removePixels(int& width, int& height, const char *filePath, byte*& data)
{
    data = 0;
    BMPIMAGE image = (bmpImage*)malloc(sizeof(bmpImage));
    BMPINFO info = (infoFetch*)malloc(sizeof(infoFetch));

    if(gp_graphics.open(filePath, image, info) == FILE_ACCESS_STATUS_SUCCESS)
    {
#ifdef DEBUG_OUTPUT
        gp_graphics.save("E:/Airchina/bmp/data/01_removePixels_a.bmp", info);

        memcpy(g_info, info, sizeof(infoFetch));

        g_info->data = (byte*)malloc(sizeof(byte) * (info->bmpSize - BMPHEADERSIZE));

        memcpy(g_info->data, info->data, sizeof(byte) * (info->bmpSize - BMPHEADERSIZE));

        gp_graphics.save("E:/Airchina/bmp/data/02_removePixels_b.bmp", g_info);
#endif
        width = info->w;
        height = info->h;
        removePixelsStep(DT_CHANGE_WHITE_TIMES, DO_CHANGE_WHITE_TIMES, info->w, info->h, info->bmpSize, info->data, data, filePath);
    }

    free(image);
    free(info);

    image = 0;
    info = 0;
}

void
process::removePixelsStep(int dt_change, int do_change, int width, int height, long bmpSize, byte *data, byte *&rdata, const char *filePath)
{
    long i = 0;
    vector<point> left_top;
    vector<point> right_bottom;

    byte *dt_data = (byte*)malloc(sizeof(byte) * width * height);
    byte *do_data = (byte*)malloc(sizeof(byte) * width * height);

    gp_common.averagePixels(bmpSize, data);

#ifdef DEBUG_OUTPUT
    memcpy(g_info->data, data, sizeof(byte) * (g_info->bmpSize - BMPHEADERSIZE));
    gp_graphics.save("E:/Airchina/bmp/data/03_averagePixels.bmp", g_info);
#endif

    gp_common.binarization(bmpSize, data);

#ifdef DEBUG_OUTPUT
    memcpy(g_info->data, data, sizeof(byte) * width * height * 3 - BMPHEADERSIZE);
    gp_graphics.save("E:/Airchina/bmp/data/04_binarization.bmp", g_info);
#endif

    rdata = (byte*)malloc(sizeof(byte) * width * height);

    gp_common.singlizeImage(width, height, rdata, data);

#ifdef DEBUG_OUTPUT
    dfake = (byte*)malloc(sizeof(byte) * width * height * 3);
    recoverImage(width, height, rdata, dfake);
    memcpy(g_info->data, dfake, sizeof(byte) * width * height * 3 - BMPHEADERSIZE);
    gp_graphics.save("E:/Airchina/bmp/data/05_singlizeImage.bmp", g_info);
#endif

    findThickRectangle(width, height, left_top, right_bottom, rdata);

#ifdef DEBUG_OUTPUT
    recoverImage(width, height, rdata, dfake);
    memcpy(g_info->data, dfake, sizeof(byte) * width * height * 3 - BMPHEADERSIZE);
    gp_graphics.save("E:/Airchina/bmp/data/06_findThickRectangle.bmp", g_info);
#endif

    mergeRectangle(left_top, right_bottom);

    fillEdges(width, height, rdata);

#ifdef DEBUG_OUTPUT
    recoverImage(width, height, rdata, dfake);
    memcpy(g_info->data, dfake, sizeof(byte) * width * height * 3 - BMPHEADERSIZE);
    gp_graphics.save("E:/Airchina/bmp/data/07_fillEdges.bmp", g_info);
#endif

    memcpy(dt_data, rdata, sizeof(byte) * width * height);
    memcpy(do_data, rdata, sizeof(byte) * width * height);

    for(i = 0; i <= dt_change; ++i)
    {
        changePixels(DT_CHANGE_WHITE, width, height, bmpSize, dt_data);
    }
    changePixels(DO_CHANGE_WHITE, width, height, bmpSize, dt_data);

    for(i = 0; i <= do_change; ++i)
    {
        changePixels(DO_CHANGE_WHITE, width, height, bmpSize, do_data);
    }

    mergeImages(width, height, dt_data, do_data, rdata);

#ifdef DEBUG_OUTPUT
    recoverImage(width, height, rdata, dfake);
    memcpy(g_info->data, dfake, sizeof(byte) * width * height * 3 - BMPHEADERSIZE);
    gp_graphics.save("E:/Airchina/bmp/data/08_mergeImages.bmp", g_info);
#endif

    drawRectangle(left_top, right_bottom, width, height, rdata);

#ifdef DEBUG_OUTPUT
    recoverImage(width, height, rdata, dfake);
    memcpy(g_info->data, dfake, sizeof(byte) * width * height * 3 - BMPHEADERSIZE);
    gp_graphics.save("E:/Airchina/bmp/data/09_drawRectangle.bmp", g_info);
#endif

    for(i = 0; i < REMOVE_LONELY_PIXEL; ++i)
    {
        removeLonelyPixel(width, height, rdata);
    }

#ifdef DEBUG_OUTPUT
    recoverImage(width, height, rdata, dfake);
    memcpy(g_info->data, dfake, sizeof(byte) * width * height * 3 - BMPHEADERSIZE);
    gp_graphics.save("E:/Airchina/bmp/data/10_removeLonelyPixel.bmp", g_info);
#endif

    for(i = 0; i < IMAGE_GROW_TIMES; ++i)
    {
        imageGrowing(width, height, rdata);
    }

#ifdef DEBUG_OUTPUT
    recoverImage(width, height, rdata, dfake);
    memcpy(g_info->data, dfake, sizeof(byte) * width * height * 3 - BMPHEADERSIZE);
    gp_graphics.save("E:/Airchina/bmp/data/11_imageGrowing.bmp", g_info);
#endif

    free(dt_data);
    free(do_data);

    dt_data = 0;
    do_data = 0;

#ifdef DEBUG_OUTPUT
    free(dfake);
    dfake = 0;

    free(g_info->data);
    g_info->data = 0;

    free(g_info);
    g_info = 0;
#endif
}

void
process::findThickRectangle(int width, int height, vector<point>& left_top, vector<point>& right_bottom, byte *data)
{
    int m = 0, n = 0;
    int ih = 0, iw = 0;
    int jh = 0, jw = 0;
    int cnt = 0;
    bool flg = true;

    for(ih = 0; ih <= height - 2; ++ih)
    {
        for(iw = 0; iw <= width - 2; ++iw)
        {
            flg = true;
            for(jh = ih + 1; jh < height && flg; ++jh)
            {
                for(jw = iw + 1; jw < width && flg; ++jw)
                {
                    cnt = 0;
                    for(m = ih; m <= jh && flg; ++m)
                    {
                        for(n = iw; n <=jw && flg; ++n)
                        {
                            if(data[m * width + n] == c_WHITE)
                            {
                                flg = false;
                                continue;
                            }
                            cnt++;
                        }
                    }

                    if(cnt == (jh - ih + 1) * (jw - iw + 1))
                    {
                        left_top.push_back(point(ih, iw));
                        right_bottom.push_back(point(jh, jw));
                    }
                }
            }
        }
    }
}

void
process::mergeRectangle(vector<point>& left_top, vector<point>& right_bottom)
{
    int i = 0, j = 0;
    int len = int(left_top.size());
    bool *valid = (bool*)malloc(sizeof(bool) * len);
    vector<point> t_left_top(left_top.begin(), left_top.end());
    vector<point> t_right_bottom(right_bottom.begin(), right_bottom.end());

    point lt, rb;

    memset(valid, true, sizeof(bool) * len);

    left_top.clear();
    right_bottom.clear();

    for(i = 0; i < len; ++i)
    {
        if(valid[i])
        {
            valid[i] = false;
            lt = t_left_top[i];
            rb = t_right_bottom[i];

            for(j = i; j < len; ++j)
            {
                // Current rectangle is smaller than another one
                if(valid[j] && t_left_top[j].h <= lt.h && t_left_top[j].w <= lt.w && t_right_bottom[j].h >= rb.h && t_right_bottom[j].w >= rb.w)
                {
                    valid[j] = false;
                    lt = t_left_top[j];
                    rb = t_right_bottom[j];
                }
                // Current rectangle is larger than another one
                if(valid[j] && t_left_top[j].h >= lt.h && t_left_top[j].w >= lt.w && t_right_bottom[j].h <= rb.h && t_right_bottom[j].w <= rb.w)
                {
                    valid[j] = false;
                }
            }

            left_top.push_back(lt);
            right_bottom.push_back(rb);
        }
    }
}

// Fill the edges which are all black.
void 
process::fillEdges(int width, int height, byte *data)
{
    int i = 0, j = 0, tmp = 0;

    // top
    for(i = 0; i < width; ++i)
    {
        for(j = 0; j < height && data[j * width + i] == WHITE; ++j)
        {
            data[j * width + i] = BLACK;
        }
    }

    // bottom
    for(i = 0; i < width; ++i)
    {
        for(j = height - 1; j >= 0 && data[j * width + i] == WHITE; --j)
        {
            data[j * width + i] = BLACK;
        }
    }

    // left
    for(i = 0; i < height; ++i)
    {
        for(j = 0; j < width && data[i * width + j] == WHITE; ++j)
        {
            data[i * width + j] = BLACK;
        }
    }

    // right
    for(i = 0; i < height; ++i)
    {
        for(j = width - 1; j >= 0 && data[i * width + j] == WHITE; --j)
        {
            data[i * width + j] = BLACK;
        }
    }
}

void
process::changePixels(int kind, int width, int height, long bmpSize, byte *data)
{
    int m = 0, n = 0;
    int ih = 0, iw = 0;
    int pos = 0, cnt[2];

    byte *fake = (byte*)malloc(sizeof(byte) * width * height);
    memcpy(fake, data, sizeof(byte) * width * height);

    for(ih = 0; ih < height; ++ih)
    {
        for(iw = 0; iw < width; ++iw)
        {
            memset(cnt, 0, sizeof(cnt));

            for(m = -1; m <= 1; ++m)
            {
                for(n = -1; n <= 1; ++n)
                {
                    if(ih + m >= 0 && ih + m < height && iw + n >= 0 && iw + n < width)
                    {
                        pos = (ih + m) * width + (iw + n);

                        if(fake[pos] == 255)
                        {
                            cnt[1]++;
                        }
                        else if(fake[pos] == 0)
                        {
                            cnt[0]++;
                        }
                    }
                }
            }

            pos = ih * width + iw;

            switch(kind)
            {
            case 0:
                data[pos] = cnt[0] < cnt[1] ? 255 : (data[pos] ? 255: 0);
                break;
            case 1:
                data[pos] = cnt[0] < cnt[1] ? 255 : 0;
                break;
            default:
                break;
            }
        }
    }

    free(fake);
    fake = 0;
}

void
process::mergeImages(int width, int height, const byte *dt_data, const byte *do_data, byte *data)
{
    int ih = 0, iw = 0;
    int k = 0, m = 0, n = 0;
    int i_dt = 0, i_do = 0;
    int tmp = 0;
    int dis[2], cnt[2];

    for(ih = 0; ih < height; ++ih)
    {
        for(iw = 0; iw < width; ++iw)
        {
            tmp = ih * width + iw;
            if(dt_data[tmp] == do_data[tmp])
            {
                data[tmp] = dt_data[tmp];
            }
            else
            {
                memset(cnt, 0, sizeof(cnt));
                for(m = -1; m <= 1; ++m)
                {
                    for(n = -1; n <= 1; ++n)
                    {
                        if(ih + m >= 0 && ih + m < height && iw + n >= 0 && iw + n < width)
                        {
                            k = (ih + m) * width + (iw + n);
                            i_dt = dt_data[k] ? 1 : 0;
                            i_do = do_data[k] ? 1 : 0;
                            cnt[i_dt] += 1;
                            cnt[i_do] += 1;
                        }
                    }
                }

                if(cnt[0] != cnt[1])
                {
                    data[tmp] = cnt[0] > cnt[1] ? 0 : 255;
                }
                else
                {
                    memset(dis, INT_MAX, sizeof(dis));
                    findNearestPixel(ih, iw, height, width, dt_data[tmp], dis[0], dt_data);
                    findNearestPixel(ih, iw, height, width, do_data[tmp], dis[1], do_data);

                    if(dis[0] != dis[1])
                    {
                        data[tmp] = dis[0] < dis[1] ? dt_data[tmp] : do_data[tmp];
                    }
                    else
                    {
                        data[tmp] = BOOL_WHITE;
                    }
                }
            }
        }
    }
}

void
process::findNearestPixel(int ih, int iw, int height, int width, byte pixel, int& dis, const byte *data)
{
    int i = 0, j = 0;
    int pos = 0;
    dis = INT_MAX;

    for(i = (-1) * MAX_RANGE; i <= MAX_RANGE; ++i)
    {
        for(j = (-1) * MAX_RANGE; j <= MAX_RANGE; ++j)
        {
            if(iabs(i) + iabs(j) < dis && ih + i >= 0 && ih + i < height && iw + j >= 0 && iw + j < width)
            {
                pos = (ih + i) * width + (iw + j);
                if(data[pos] == pixel)
                {
                    dis = iabs(i) + iabs(j);
                }
            }
        }
    }
}

void
process::drawRectangle(vector<point>& left_top, vector<point>& right_bottom, int width, int height, byte *data)
{
    int i = 0, ih = 0, iw = 0;
    int len = int(left_top.size());

    for(i = 0; i < len; i++)
    {
        for(ih = left_top[i].h; ih <= right_bottom[i].h; ++ih)
        {
            for(iw = left_top[i].w; iw <= right_bottom[i].w; ++iw)
            {
                data[ih * width + iw] = c_WHITE;
            }
        }
    }
}

// For one 3 by 3 image block, if the center is one kind of pixel
// and the other 8 are another kind of pixel, then replace the center pixel
void
process::removeLonelyPixel(int width, int height, byte *data)
{
    int m = 0, n = 0;
    int ih = 0, iw = 0;
    int tmp = 0;
    int cnt[2];

    byte *fake = (byte*)malloc(sizeof(byte) * width * height);
    memcpy(fake, data, sizeof(byte) * width * height);

    for(ih = 1; ih < height - 1; ++ih)
    {
        for(iw = 1; iw < width - 1; ++iw)
        {
            memset(cnt, 0, sizeof(cnt));
            for(m = -1; m <= 1; ++m)
            {
                for(n = -1; n <= 1; ++n)
                {
                    tmp = (ih + m) * width + (iw + n);
                    tmp = int(fake[tmp]) == 0 ? 0 : 1;

                    cnt[tmp] += 1;
                }
            }
            tmp = ih * width + iw;

            if(cnt[0] == 8 && fake[tmp] == WHITE)
            {
                data[tmp] = BLACK;
            }
            if(cnt[1] == 8 && fake[tmp] == BLACK)
            {
                data[tmp] = WHITE;
            }
        }
    }

    free(fake);
}

 // To the black pixels, if there are two white pixels lies at its 
 // (west, east), (north, south), (north-west, south-east), (north-east, south-west)
 // change the black to white
void
process::imageGrowing(int width, int height, byte *data)
{
    int ih = 0, iw = 0;
    int j = 0, tmp = 0;
    int t[2];

    for(ih = 1; ih < height - 1; ++ih)
    {
        for(iw = 1; iw < width - 1; ++iw)
        {
            tmp = ih * width + iw;
            if(data[tmp] == c_BLACK)
            {
                for(j = 0; j < 4; j++)
                {
                    t[0] = (ih + grow_direction[j][0]) * width + (iw + grow_direction[j][1]);
                    t[1] = (ih + grow_direction[j][2]) * width + (iw + grow_direction[j][3]);

                    if(data[t[0]] == c_WHITE && data[t[1]] == c_WHITE)
                    {
                        data[tmp] = c_WHITE;
                        break;
                    }
                }
            }
        }
    }
}

void 
process::recoverImage(int width, int height, const byte *fake, byte *data)
{
    int i = 0, j = 0, tmp = 0;

    for(i = 0; i < height; ++i)
    {
        for(j = 0; j < width; ++j)
        {
            tmp = index(i, j, width);
            data[tmp] = data[tmp + 1] = data[tmp + 2] = fake[i * width + j];
        }
    }
}