/*
 * January, 26th, 2014.
 * All related image processions.
 */
#pragma once

#include <vector>
#include "type.h"
#include "point.h"
#include "constant.h"

struct process
{
    void removePixels(int&, int&, const char*, byte*&);

    void removePixelsStep(int, int, int, int, long, byte*, byte*&, const char*);    

    void findThickRectangle(int, int, std::vector<point>&, std::vector<point>&, byte*);

    void mergeRectangle(std::vector<point>&, std::vector<point>&);

    void fillEdges(int, int, byte*);

    void changePixels(int, int, int, long, byte*);

    void mergeImages(int, int, const byte*, const byte*, byte*);

    void findNearestPixel(int, int, int, int, byte, int&, const byte*);
 
    void drawRectangle(std::vector<point>&, std::vector<point>&, int, int, byte*);

    void removeLonelyPixel(int, int, byte*);

    void imageGrowing(int, int, byte*);

    void recoverImage(int, int, const byte*, byte*);
};