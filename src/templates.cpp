#include <cstring>
#include "../include/templates.h"

void
templates::release()
{
    if(data)
    {
        delete[] data;
        data = 0;
    }
}

void
templates::create()
{
    if(h * w > 0)
    {
        data = new byte[h * w];
    }
}

void
templates::init(
    const int _h, 
    const int _w, 
    const byte _name, 
    const byte *_data, 
    const double _moments[MOMENT_LENGTH])
{
    data = 0;
    release();

    if(!_data)
    {
        h = 0;
        w = 0;
        name = '0';

        return;
    }

    h = _h;
    w = _w;
    name = _name;

    create();

    memcpy(data, _data, sizeof(byte) * h * w);
    memcpy(moments, _moments, sizeof(moments));
}

templates::templates(int _h, int _w, byte _name, const byte *_data, const double _moments[MOMENT_LENGTH]) 
{
    init(_h, _w, _name, _data, _moments);
}

templates::templates(const templates& _templates)
{
    if(this == &_templates)
    {
        return;
    }
    init(_templates.h, _templates.w, _templates.name, _templates.data, _templates.moments);
}

templates&
templates::operator=(const templates& _templates)
{
    if(this != &_templates)
    {
        init(_templates.h, _templates.w, _templates.name, _templates.data, _templates.moments);
    }

    return *this;
}

templates::~templates()
{
    release();
}