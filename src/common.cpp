#include <cstdio>
#include <cstdlib>
#include <time.h>
#include "../include/common.h"
#include "../include/constant.h"
#include "../include/minifunc.h"
#include "../include/graphics.h"

using namespace std;

// Reverse an array.
void 
common::reverseArray(int begin, int end, char* ary)
{
    char c = 0;
    while(begin < end)
    {
        c = ary[begin];
        ary[begin]= ary[end];
        ary[end] = c;

        ++begin;
        --end;
    }
}

// Transform an integer into a char array.
void
common::changeInt2CharArray(int number, char*& aryNum)
{
    aryNum = (char*)malloc(sizeof(char) * NumLength);
    memset(aryNum, 0, sizeof(char) * NumLength);

    int cur = 0;
    int sign = number >= 0 ? 1 : -1;
    number = number * sign;

    if(number == 0)
    {
        aryNum[cur] = '0';
        aryNum[cur + 1] = 0;
    }
    else
    {
        while(number > 0)
        {
            aryNum[cur++] = char((number % 10) + '0');
            number /= 10;
        }

        if(sign < 0)
        {
            aryNum[cur++] = '-';
        }
        aryNum[cur] = 0;

        reverseArray(0, cur - 1, aryNum);
    }
}

// Return a bool array to represent the binary form of an integer.
void
common::findNonZeroBits(int number, bool*& bits)
{
    bits = (bool*)malloc(sizeof(bool) * (NumLength + 1));
    memset(bits, 0, sizeof(bool) * (NumLength + 1));
    for(int i = 0; i < NumLength + 1; ++i)
    {
        if(number & MASK[i])
        {
            bits[i] = true;
        }
    }
}

// Which is used to spell the file path of template.
void
common::replaceFilePathForTemplate(char character, char filePath[])
{
    int index = 0;
    int len = strlen(filePath);
    if(!filePath || len == 0)
    {
        return;
    }

    for(index = len - 1; index >= 0; index--)
    {
        if(filePath[index] == '.' && index > 0)
        {
            filePath[index - 1] = character;
            break;
        }
    }
}

// Spell the file path to save the processed images.
void
common::spellSaveFilePath1(int fileType, int filterBranch, char savePath[])
{
    int i = 0, j = 0;
    int curLen = strlen(savePath);
    char *tmp = (char*)malloc(sizeof(char) * NumLength);
    bool *bits = 0;

    findNonZeroBits(filterBranch, bits);

    for(i = 0; i <= NumLength; i++)
    {
        if(bits[i])
        {
            memset(tmp, 0, sizeof(char) * NumLength);
            changeInt2CharArray(i, tmp);
            for(j = 0; j < int(strlen(tmp)); j++)
            {
                savePath[curLen++] = tmp[j];
            }
            savePath[curLen++] = '_';
        }
    }
    spellSaveFilePathTail(fileType, savePath);
}

void
common::spellSaveFilePath2(int fileType, vector<long> nums, char savePath[])
{
    int i = 0;
    int curLen = strlen(savePath);
    char tmp[NumLength];

    savePath[curLen] = '_';
    curLen = curLen + 1;

    for(vector<long>::iterator it = nums.begin(); it != nums.end(); it++)
    {
        memset(tmp, 0, sizeof(tmp));
        _itoa_s((*it), tmp, 10);
        for(i = 0; i < int(strlen(tmp)); ++i)
        {
            savePath[curLen] = tmp[i];
            curLen = curLen + 1;
        }
        savePath[curLen] = '_';
        curLen = curLen + 1;
    }
    spellSaveFilePathTail(fileType, savePath);
}

// Spell the file name extension.
void
common::spellSaveFilePathTail(int fileType, char savePath[])
{
    int curLen = strlen(savePath);
    savePath[curLen - 1] = '.';

    switch(fileType)
    {
    case BMPTYPE:
        savePath[curLen++] = 'b';
        savePath[curLen++] = 'm';
        savePath[curLen++] = 'p';
        break;
    case TXTTYPE:
        savePath[curLen++] = 't';
        savePath[curLen++] = 'x';
        savePath[curLen++] = 't';
        break;
    default:
        break;
    }

    savePath[curLen] = 0;
}

// Transform the bitmap into black-white image.
// len(data) = width * height
// len(fake) = width * height * 3
void
common::singlizeImage(int width, int height, byte *data, byte *fake)
{
    int i = 0, j = 0;
    int tmp = 0, value = 0;

    for(i = 0; i < height; i++)
    {
        for(j = 0; j < width; j++)
        {
            tmp = index(i, j, width);
            value = ((fake[tmp] + fake[tmp + 1] + fake[tmp + 2]) / 3);
            value = (value < 255 - value) ? 0 : 255;
            data[i * width + j] = byte(value);
        }
    }
}

// Calculate a^b.
ll
common::calPow(ll base, ll exponent)
{
    if(exponent == 0) { return ll(1); }
    if(exponent == 1) { return base; }
    if(exponent == 2) { return base * base; }

    ll tmp = calPow(base, exponent >> 1);
    tmp = tmp * tmp;

    if((exponent & 1) == 0) { return tmp; }
    if((exponent & 1) == 1) { return tmp * base; }

    return ll(1);
}

// During the process of reading parameters into memory,
// this function is used to transform char array to double.
void
common::transferData(const char *data, char &name, double moments[MOMENT_LENGTH])
{
    int i = 0, j = 0, k = 0;
    int start = 0, end = 0;
    int sign = 0, len = strlen(data);

    while(data[start] == ' ') { ++start; }

    name = data[start];
    ++start;

    for(i = 0; i < MOMENT_LENGTH; ++i)
    {
        while(data[start] == ' ') { ++start; }

        end = start;

        while(data[end] != ' ' && end != len) { ++end; }

        sign = 1;
        if(data[start] == '-')
        {
            sign = -1;
            ++start;
        }

        char tmp[50];
        memset(tmp, 0, sizeof(tmp));
        for(j = start, k = 0; j <= end; ++j, ++k)
        {
            tmp[k] = data[j];
        }
        tmp[k - 1] = 0;
        moments[i] = atof(tmp) * sign;

        start = end + 1;
    }
}

// Do image binarization.
void
common::binarization(long bmpSize, byte *data)
{
    long isize = 0;
    for(isize = 0; isize < bmpSize; ++isize)
    {
        if(data[isize] > Threshold)
        {
            data[isize] = 255;
        }
        else
        {
            data[isize] = 0;
        }
    }
}

// Calculate sum(a[i] * a[i]), i = 0,...,(n-1).
double
common::calSquare(int length, double *ary)
{
    double ret = 0.0;
    for(int i = 0; i < length; ++i)
    {
        ret += ary[i] * ary[i] * 1.0;
    }
    return ret;
}

void
common::spellSampleFilePath(int number, char samplePath[])
{
    int i = 0, start = 0;
    char *tmp = (char*)malloc(sizeof(char) * NumLength);

    memset(tmp, 0, sizeof(tmp) * NumLength);

    changeInt2CharArray(number, tmp);

    for(start = 0; start < int(strlen(samplePath)) && samplePath[start] != '('; ++start);

    ++start;

    for(i = 0; i < int(strlen(tmp)); ++i, ++start)
    {
        samplePath[start] = tmp[i];
    }

    samplePath[start++] = ')';
    samplePath[start++] = '.';
    samplePath[start++] = 'b';
    samplePath[start++] = 'm';
    samplePath[start++] = 'p';
    samplePath[start] = 0;
}

void
common::averagePixels(long bmpSize, byte *data)
{
    long i = 0;
    byte avg = 0;

    for(i = 0; i < bmpSize; i += 3)
    {
        avg = byte(int((int(data[i]) + int(data[i + 1]) + int(data[i + 2])) / 3));
        data[i] = data[i + 1] = data[i + 2] = avg;
    }
}

void
common::imageSegment2Four(int width, int height, byte *data)
{
	vector<int> rows;
	int i = 0, j = 0, ct = 0;	
	int s_horizon[CHAR_PROCESS_NUM];
	int e_horizon[CHAR_PROCESS_NUM];

	vector<point> sample_rec_left_top;
	vector<point> sample_rec_right_bottom;

	byte *cdata = (byte*)malloc(sizeof(byte) * width * height);
	singlizeImage(width, height, cdata, data);

	// Find the pure black vertical lines' positions.
	for(i = 0; i < width; ++i)
	{
		for(j = 0; j < height && cdata[j * width + i] == c_BLACK; ++j);

		if(j == height) { rows.push_back(i); }
	}

	if(rows[rows.size() - 1] != width - 1)
	{
		rows.push_back(width - 1);
	}

	for(i = 1; i < int(rows.size()); ++i)
	{
		if(rows[i] - rows[i - 1] >= MIN_WIDTH)
		{
			s_horizon[ct] = rows[i - 1];
			e_horizon[ct] = rows[i];

			imageSubSegment(width, height, rows[i - 1], rows[i], cdata, sample_rec_left_top, sample_rec_right_bottom);
			++ct;
		}
	}

	for(i = 0; i < CHAR_PROCESS_NUM; ++i)
	{
		saveImagePartBy2Points(sample_rec_left_top[i].h, sample_rec_left_top[i].w, sample_rec_right_bottom[i].h, sample_rec_right_bottom[i].w, width, cdata);
	}

	free(cdata);
	cdata = 0;
}

void
common::imageSubSegment(int width, int height, int horizontal_start, int horizontal_end, byte *data, vector<point>& sample_rec_left_top, vector<point>& sample_rec_right_bottom)
{
	int i = 0, j = 0;
	int lt_height = 0;
	int lt_width = horizontal_start;
	int rb_height = height - 1;
	int rb_width = horizontal_end;

	for(i = 0; i < height; ++i)
	{
		for(j = horizontal_start; j <= horizontal_end && data[i * width + j] == c_BLACK; ++j);
		if(j <= horizontal_end)
		{
			lt_height = i;
			break;
		}
	}

	for(i = height - 1; i >= 0; --i)
	{
		for(j = horizontal_start; j <= horizontal_end && data[i * width + j] == c_BLACK; ++j);
		if(j <= horizontal_end)
		{
			rb_height = i;
			break;
		}
	}

	sample_rec_left_top.push_back(point(lt_height, lt_width));
	sample_rec_right_bottom.push_back(point(rb_height, rb_width));
}

// Note: if the real width cannot be divided by 4, the width has to be increased to a number which can be divided by 4.
void
common::saveImagePartBy2Points(int rec_left_height, int rec_left_width, int rec_right_height, int rec_right_width, int width, byte *data)
{
	int i = 0, j = 0;
	int rem = 0, cur = 0, tmp = 0;
	clock_t timer = clock();
	char savePath[PathLength];

	vector<long> v_times;

	int s_width = rec_right_width - rec_left_width + 1;
	int s_height = rec_right_height - rec_left_height + 1;

	while((s_width + rem) % 4 != 0) { ++rem; }

	byte *image_part = (byte*)malloc(sizeof(byte) * (s_width + rem) * s_height * 3);

	memcpy(savePath, TEMPLATE_OUTPUT_PATH, sizeof(savePath));

	v_times.push_back(long(timer));

	spellSaveFilePath2(BMPTYPE, v_times, savePath);

	for(i = 0; i < s_height; ++i)
	{
		for(j = 0; j < s_width; ++j)
		{
			cur = index(i, j, (s_width + rem));
			tmp = (rec_left_height + i) * width + (rec_left_width + j);
			image_part[cur] = image_part[cur + 1] = image_part[cur + 2] = data[tmp];
			cur += 3;
		}
		for(j = 0; j < rem; ++j)
		{
			image_part[cur] = image_part[cur + 1] = image_part[cur + 2] = c_BLACK;
			cur += 3;
		}
	}

	graphics cg;
	cg.save(savePath, s_width + rem, s_height, image_part);

	free(image_part);
	image_part = 0;
}

void
common::fillImageWhite(int width, int height, byte *data, vector<point>& sample_rec_left_top, vector<point>& sample_rec_right_bottom)
{
	bool flg = true;
	int i = 0, j = 0, k = 0, tmp = 0;

	for(i = 0; i < height; ++i)
	{
		for(j = 0; j < width; ++j)
		{
			flg = true;
			for(k = 0; k < int(sample_rec_left_top.size()) && flg; ++k)
			{
				if(i >= sample_rec_left_top[k].h && 
				   i <= sample_rec_right_bottom[k].h && 
				   j >= sample_rec_left_top[k].w && 
				   j <= sample_rec_right_bottom[k].w)
				{
					flg = false;
				}
			}

			if(flg)
			{
				tmp = index(i, j, width);
				data[tmp] = data[tmp + 1] = data[tmp = 2] = c_WHITE;
			}
		}
	}
}