/*
 * January, 23rd, 2014.
 * A template object represents a image template.
 */
#pragma once

#include "type.h"
#include "constant.h"

struct templates
{
    int   h;
    int   w;
    byte  name;
    byte  *data;
    double moments[MOMENT_LENGTH];

    templates() : h(0), w(0), name('0'), data(0) {}

    templates(const templates& _templates);

    templates& operator=(const templates& _templates);

    templates(
        int _h, 
        int _w, 
        byte _name, 
        const byte *_data, 
        const double _moments[MOMENT_LENGTH]);

    void init(
        const int _h, 
        const int _w, 
        const byte _name, 
        const byte *_data, 
        const double _moments[MOMENT_LENGTH]);

    void release();

    void create();

    ~templates();
};