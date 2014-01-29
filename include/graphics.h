/*
 * January, 24th, 2014.
 * Bitmap related operations.
 */
#pragma once

#include "type.h"
#include "constant.h"
#include "bitmap.h"

struct graphics
{
    int open(const char*, BMPIMAGE, BMPINFO);
    
    void save(const char*, BMPINFO);

    void save(const char*, long, long, byte*);
};