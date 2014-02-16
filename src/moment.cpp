#include <cstdio>
#include <cstdlib>
#include <cmath>
#include "../include/common.h"
#include "../include/minifunc.h"
#include "../include/constant.h"
#include "../include/moment.h"

using namespace std;

common gm_common;

ll
moment::calMpq(int p, int q, int width, int height, byte *matrix)
{
    ll ret = 0;
    int i = 0, j = 0;

    for(i = 0; i < height; ++i)
    {
        for(j = 0; j < width; ++j)
        {
            if(matrix[i * width + j] == c_WHITE)
            {
                ret += gm_common.calPow(ll(i), ll(p)) * gm_common.calPow(ll(j), ll(q));
            }
        }
    }

    return ret;
}

double
moment::calCenterMoment(int p, int q, int width, int height, byte *matrix)
{
    double upq = 0;
    ll i = 0, j = 0;
    ll u00 = 0, m10 = 0, m01 = 0;

    m10 = calMpq(1, 0, width, height, matrix);
    m01 = calMpq(0, 1, width, height, matrix);

    for(i = 0; i < height; ++i)
    {
        for(j = 0; j < width; ++j)
        {
            if(matrix[i * width + j] == c_WHITE)
            {
                upq += double(gm_common.calPow(i - m10, p) * gm_common.calPow(j - m01, q));
                u00 += 1;
            }
        }
    }

    if(u00 == 0) { u00 = 1; }
    u00 = gm_common.calPow(u00, (p + q + 2) >> 1);

    return double(upq * 1.0 / u00);
}

void
moment::calRegularMoment(int width, int height, byte *matrix, double moments[MOMENT_LENGTH])
{
    int i = 0;
    double centerM[7];
    int p1[7] = {0, 0, 1, 1, 2, 2, 3};
    int p2[7] = {2, 3, 1, 2, 0, 1, 0};

    for(i = 0; i < 7; ++i)
    {
        centerM[i] = calCenterMoment(p1[i], p2[i], width, height, matrix);
    }

    double a1 = centerM[1] + centerM[4];
    double a2 = centerM[3] + centerM[6];
    double a3 = a2 * a2 - 3 * a1 * a1;
    double a4 = 3 * a2 * a2 - a1 * a1;
    double a5 = centerM[6] - 3 * centerM[3];
    double a6 = 3 * centerM[5] - centerM[1];
    double a7 = centerM[4] - centerM[0];

    double huM[7] = {
        iabs(centerM[0] + centerM[4]),
        a7 * a7 + 4 * centerM[2] * centerM[2],
        a5 * a5 + a6 * a6,
        a1 * a1 + a2 * a2,
        iabs(a2 * a3 * a5 + a1 * a4 + a6),
        iabs((a2 * a2 - a1 * a1) * a7 + 4 * centerM[2] * a1 * a2),
        iabs(a2 * a3 * a6 - a1 * a4 * a5)
    };

    double powHuM[4];
    powHuM[0] = huM[0] * huM[0];
    powHuM[1] = powHuM[0] * huM[0];
    powHuM[2] = powHuM[1] * huM[0];
    powHuM[3] = powHuM[1] * powHuM[1];

    double lg2 = log(double(2));

    if(iszero(huM[0]))
    {
        moments[0] = log(huM[1] * 1.0 / powHuM[0]) / lg2;
        moments[1] = log(huM[2] * 1.0 / powHuM[1]) / lg2;
        moments[2] = log(huM[3] * 1.0 / powHuM[1]) / lg2;
        moments[3] = log(huM[4] * 1.0 / powHuM[3]) / lg2;
        moments[4] = log(huM[5] * 1.0 / powHuM[2]) / lg2;
        moments[5] = log(huM[6] * 1.0 / powHuM[3]) / lg2;
    }
}

double
moment::calMomentSimilar(double moments1[MOMENT_LENGTH], double moments2[MOMENT_LENGTH])
{
    double ret = 0;
    for(int i = 0; i < MOMENT_LENGTH; ++i)
    {
        ret += i * PI * 1000.0 * gm_common.calPow(moments1[i] - moments2[2], 2);
    }
    return ret;
}

double
moment::calMomentSimilarByCos(double moments1[MOMENT_LENGTH], double moments2[MOMENT_LENGTH])
{
    double p1 = 0.0;
    double p2 = sqrt(gm_common.calSquare(MOMENT_LENGTH, moments1));
    double p3 = sqrt(gm_common.calSquare(MOMENT_LENGTH, moments2));

    for(int i = 0; i < MOMENT_LENGTH; ++i)
    {
        p1 += moments1[i] * moments2[2];
    }
    if(iszero(p1) || iszero(p3))
    {
        return 0;
    }

    return (p1 * 100.0) / (p2 * p3);
}