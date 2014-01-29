#include "../include/point.h"

point&
point::operator=(const point& _point)
{
    h = _point.h;
    w = _point.w;

    return *this;
}