/*
 * January, 28th, 2014.
 * Do image distinguish.
 */
#pragma once

#include <vector>
#include "type.h"
#include "constant.h"
#include "point.h"
#include "templates.h"

struct distinguish
{
private:
    vector<point> left_top;
    vector<point> right_bottom;
    vector<templates> char_templates;
public:
    distinguish(int, int, byte*&, byte[], const char*);

    int readTemplates(const char*);
    void readTemplateData(byte, const char*);
    void clearOneLine(int, int, int, int, int, byte*);
    void removeRedundantPixels(int, int, byte*);
    void removeBlackPixels(int, int, int, int, int, byte*);

    ll calMpq(int, int, int, int, byte*);
    double calCenterMoment(int, int, int, int, byte*);
    void calRegularMoment(int, int, byte*, double[]);
    double calMomentSimilar(double[], double[]);
    double calMomentSimilarByCos(double[], double[]);

    byte distinguishOneCharacter(int, int, byte*, const char*);
    
    byte distinguishOneCharacterByCos(int, int, byte*, const char*);
}