/*
 * January, 23rd, 2014.
 * Some common functions.
 */
#pragma once

#include <vector>
#include "type.h"
#include "point.h"

struct common
{
    // Reverse an array.
    void reverseArray(int, int, char*);

    // Transform an integer into a char array.
    void changeInt2CharArray(int, char*&);

    // Return a bool array to represent the binary form of an integer.
    void findNonZeroBits(int, bool*&);

    // Which is used to spell the file path of template.
    void replaceFilePathForTemplate(char, char[]);

    // Spell the file path to save the processed images.
    void spellSaveFilePath1(int, int, char[]);

    void spellSaveFilePath2(int, std::vector<long>, char[]);

    // Spell the file name extension.
    void spellSaveFilePathTail(int, char[]);

    // Transform the bitmap into black-white image.
    void singlizeImage(int, int, byte*, byte*);

    // Calculate a^b.
    ll calPow(ll, ll);

    // During the process of reading parameters into memory,
    // this function is used to transform char array to double.
    void transferData(const char*, char&, double[]);

    // Do image binarization.
    void binarization(long, byte*);

    // Calculate sum(a[i] * a[i]), i = 0,...,(n-1).
    double calSquare(int, double*);

    // Spell the sample path.
    void spellSampleFilePath(int, char[]);

	// data[i] = data[i + 1] = data[i + 2] = (data[i] + data[i + 1] + data[i + 2]) / 3
	void averagePixels(long, byte*);

	void imageSegment2Four(int, int, byte*);

	void imageSubSegment(int, int, int, int, byte*, std::vector<point>&, std::vector<point>&);

	void saveImagePartBy2Points(int, int, int, int, int, byte*);

	void fillImageWhite(int, int, byte*, std::vector<point>&, std::vector<point>&);
};