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
	// Open a bitmap file using a path.
    int open(const char*, BMPIMAGE, BMPINFO);
    
	// Save BMPINFO as a bitmap file.
    void save(const char*, BMPINFO);

	// Save the byte array which size is ((width * height) * 3) as a bitmap file.
    void save(const char*, long, long, byte*);
};