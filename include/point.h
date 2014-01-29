/*
 * January, 23rd, 2014.
 * Represpents a point.
 */
#pragma once

struct point
{
    int h;
    int w;
    
    point() : h(0), w(0) {}

    point(int _h, int _w) : h(_h), w(_w) {}

    point(const point& _point) : h(_point.h), w(_point.w) {}

    point& operator=(const point& _point);
};