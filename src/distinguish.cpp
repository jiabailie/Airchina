#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <map>
#include <climits>
#include "../include/type.h"
#include "../include/path.h"
#include "../include/bitmap.h"
#include "../include/minifunc.h"
#include "../include/point.h"
#include "../include/constant.h"
#include "../include/common.h"
#include "../include/templates.h"
#include "../include/graphics.h"
#include "../include/moment.h"
#include "../include/distinguish.h"

using namespace std;

common gd_common;
moment gd_moment;
graphics gd_graphics;

distinguish::distinguish(int width, int height, byte *&data, byte ret[CHAR_PROCESS_NUM], const char *filePath)
{
	gd_common.imageSegment(width, height, data, left_top, right_bottom);

	int status = readTemplates(TEMPLATE_SAVE_PATH);
	
	for(int i = 0; i < CHAR_PROCESS_NUM; ++i)
	{
		ret[i] = distinguishOneCharacterByCos(i, width, data, filePath);
	}
}

byte
distinguish::distinguishOneCharacterByCos(int index, int width, byte *data, const char *openFile)
{
	int i = 0, j = 0;
	int cnt = 0;
	byte ret = 'a';
	double tmp = 0, disMax = INT_MIN;

	int iWidth = right_bottom[index].w - left_top[index].w + 1;
	int iHeight = right_bottom[index].h - left_top[index].h + 1;

	double selfMoments[MOMENT_LENGTH];

	byte *iData = (byte*)malloc(sizeof(byte) * iWidth * iHeight);

	for(i = left_top[index].h; i <= right_bottom[index].h; ++i)
	{
		for(j = left_top[index].w; j <= right_bottom[index].w; ++j)
		{
			iData[cnt] = data[i * width + j];
			++cnt;
		}
	}

	gd_moment.calRegularMoment(iWidth, iHeight, iData, selfMoments);

	tmp = disMax = gd_moment.calMomentSimilarByCos(selfMoments, char_templates[0].moments);
	ret = char_templates[0].name;

	for(i = 1; i < int(char_templates.size()); ++i)
	{
		tmp = gd_moment.calMomentSimilarByCos(selfMoments, char_templates[i].moments);

		if(tmp > disMax)
		{
			disMax = tmp;
			ret = char_templates[i].name;
		}
	}

	return ret;
}

byte
distinguish::distinguishOneCharacter(int index, int width, byte *data, const char *openFile)
{
	bool flg = false;
	int i = 0, j = 0;
	int cnt = 0, itmp = 0;
	byte ret = 'a', cmin = 'a';
	double tmp = 0, max = INT_MIN;
	double momentSimilar = INT_MAX;
	map<char, int> mDistance;
	map<char, int> mCountKinds;
	
	int iWidth = right_bottom[index].w - left_top[index].w + 1;
	int iHeight = right_bottom[index].h - left_top[index].h + 1;

	double selfMoments[MOMENT_LENGTH];

	byte *iData = (byte*)malloc(sizeof(byte) * iWidth * iHeight);

	for(i = left_top[index].h; i <= right_bottom[index].h; ++i)
	{
		for(j = left_top[index].w; j <= right_bottom[index].w; ++j)
		{
			iData[cnt] = data[i * width + j];
			++cnt;
		}
	}

	gd_moment.calRegularMoment(iWidth, iHeight, iData, selfMoments);

	tmp = momentSimilar = gd_moment.calMomentSimilar(selfMoments, char_templates[0].moments);
	ret = cmin = char_templates[0].name;

	for(i = 0; i < int(char_templates.size()); ++i)
	{
		tmp = gd_moment.calMomentSimilar(selfMoments, char_templates[i].moments);

		if(tmp < momentSimilar)
		{
			cmin = char_templates[i].name;
			momentSimilar = tmp;
		}

		flg = false;
		for(map<char, int>::iterator it = mCountKinds.begin(); it != mCountKinds.end() && !flg; ++it)
		{
			flg = true;
		}

		if(flg)
		{
			++mCountKinds[char_templates[i].name];
		}
		else
		{
			mCountKinds[char_templates[i].name] = 1;
			mDistance[char_templates[i].name] = 0;
		}

		if(tmp < PROPER_DISTANCE)
		{
			++mDistance[char_templates[i].name];
		}
	}

	if(mDistance.size() > 0)
	{
		max = INT_MIN;
		for(map<char, int>::iterator it = mDistance.begin(); it != mDistance.end(); ++it)
		{
			tmp = 1.0 * (*it).second / (mCountKinds[(*it).first]);
			if(tmp > max)
			{
				max = tmp;
				ret = (*it).first;
			}
		}
	}

	ret = cmin;

	free(iData);
	iData = 0;

	return ret;
}

int
distinguish::readTemplates(const char *path)
{
	char content[LineLength];
	ifstream fin(path);
	
	while(fin.getline(content, LineLength))
	{
		char name = content[0];
		double moments[MOMENT_LENGTH];

		gd_common.transferData(content, name, moments);
		char_templates.push_back(templates(0, 0, byte(name), 0, moments));
	}
	fin.clear();
	fin.close();

	return FILE_ACCESS_STATUS_SUCCESS;
}

void
distinguish::readTemplateData(byte name, const char *path)
{
	byte *data = 0;
	BMPIMAGE image = (bmpImage*)malloc(sizeof(bmpImage));
	BMPINFO info = (infoFetch*)malloc(sizeof(infoFetch));

	if(gd_graphics.open(path, image, info) == FILE_ACCESS_STATUS_SUCCESS)
	{
		double moments[MOMENT_LENGTH];

		byte *fake = (byte*)malloc(sizeof(byte) * info->bmpSize);

		data = (byte*)malloc(sizeof(byte) * info->w * info->h);

		memcpy(fake, info->data, sizeof(byte) * info->bmpSize);

		gd_common.singlizeImage(info->w, info->h, data, fake);

		gd_moment.calRegularMoment(info->w, info->h, data, moments);

		char_templates.push_back(templates(info->w, info->h, name, data, moments));

		free(fake);
		fake = 0;
	}

	free(image);
	free(info);
	image = 0;
	info = 0;
}

void
distinguish::clearOneLine(int startCol, int endCol, int startRow, int endRow, int width, byte *data)
{
	int ih = 0, iw = 0;

	for(ih = startCol; ih <= endCol; ++ih)
	{
		for(iw = startRow; iw <= endRow; ++iw)
		{
			data[ih * width + iw] = c_WHITE;
		}
	}
}

void
distinguish::removeRedundantPixels(int width, int height, byte *data)
{
	for(int i = 0; i < CHAR_PROCESS_NUM; ++i)
	{
		removeBlackPixels(0, left_top[i].h - 1, left_top[i].w, right_bottom[i].w, width, data);
		removeBlackPixels(right_bottom[i].h + 1, height - 1, left_top[i].w, right_bottom[i].w, width, data);
	}
}

void
distinguish::removeBlackPixels(int verticalStart, int verticalEnd, int horizontalStart, int horizontalEnd, int width, byte *data)
{
	int vertical = 0, horizontal = 0;

	for(vertical = verticalStart; vertical <= verticalEnd; ++vertical)
	{
		for(horizontal = horizontalStart; horizontal <= horizontalEnd; ++horizontal)
		{
			data[vertical * width + horizontal] = c_WHITE;
		}
	}
}