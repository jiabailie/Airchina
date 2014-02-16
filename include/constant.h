/*
 * January, 23rd, 2014.
 * Constant values.
 */
#pragma once

// file type
// .bmp
const int BMPTYPE = 0;
// .txt
const int TXTTYPE = 1;

// The length of per file header.
const int FILEHEADERSIZE = 14;

// The length of per info header.
const int INFOHEADERSIZE = 40;

// The length of per bitmap's header = file header + info header.
const int BMPHEADERSIZE  = 54;

// The length of per image moment.
const int MOMENT_LENGTH = 6;

// The number of characters in one image.
const int CHAR_PROCESS_NUM = 4;

// Open and read file success.
const int FILE_ACCESS_STATUS_SUCCESS    = 0;

// Current file is not exist.
const int FILE_ACCESS_STATUS_NOT_EXIST  = 1;

// Do not change white positions.
const int DT_CHANGE_WHITE = 0;

// Change white positions.
const int DO_CHANGE_WHITE = 1;

// The times of procession which does not change white pixels.
const long DT_CHANGE_WHITE_TIMES = 0;

// The times of procession which change white pixels.
const long DO_CHANGE_WHITE_TIMES = 1;

// The operation times of removing lonely pixels.
const int REMOVE_LONELY_PIXEL = 1;

// How many times to do image growing.
const int IMAGE_GROW_TIMES = 6;

// The max range to search for position which has the same value.
const int MAX_RANGE = 20; 

// The integer value of black.
const int BLACK = 0;

// The integer value of white.
const int WHITE = 255;

// The value of black pixel.
const unsigned char c_BLACK = 0;

// The value of white pixel.
const unsigned char c_WHITE = 255;

// The bool value of white pixel.
const bool BOOL_WHITE = true;

// The approximate width of each character.
const int MIN_WIDTH = 20;

// The maximum length of file path.
const int PathLength = 400;

// The maximum length of number which will be used in this project.
const int NumLength = 50;

// Threshold value for binarization.
const int Threshold = 127;

// Represents from 2^0 to 2^10.
const int MASK[NumLength + 1] = {1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024};

// About this constant, no more to say.
const double PI = 3.1415926;

const int LineLength = 1000;

const double PROPER_DISTANCE = 5000;

const int grow_direction[4][4] = 
{
    { 0,  -1,  0,  1}, 
    {-1,   0,  1,  0}, 
    {-1,  -1,  1,  1}, 
    {-1,   1,  1, -1}
};