/*
 * January, 28th, 2014.
 * Do image distinguish.
 */
#pragma once

#include <vector>
#include "templates.h"

struct distinguish
{
private:
	std::vector<point> left_top;
	std::vector<point> right_bottom;
	std::vector<templates> char_templates;
public:
	distinguish(int, int, byte*&, byte[], const char*);

	int readTemplates(const char*);

	void readTemplateData(byte, const char*);

	void clearOneLine(int, int, int, int, int, byte*);

	void removeRedundantPixels(int, int, byte*);
	
	void removeBlackPixels(int, int, int, int, int, byte*);

	byte distinguishOneCharacter(int, int, byte*, const char*);
	
	byte distinguishOneCharacterByCos(int, int, byte*, const char*);
};