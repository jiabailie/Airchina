/*
 * January, 23rd, 2014.
 * Get bitmap main informations.
 */
#pragma once

struct infoFetch
{
    int   w;
    int   h;
    int   bitCount;
    int   lenLine;
    long  bmpSize;
    byte  *data;

    infoFetch() : w(0), h(0), bitCount(24), lenLine(0), bmpSize(0), data(0) {}

    ~infoFetch()
    {
        if(data)
        {
            delete[] data;
            data = 0;
        }
    }
};