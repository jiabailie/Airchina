#include <cstdio>
#include <cstdlib>
#include <time.h>
#include "../include/common.h"
#include "../include/distinguish.h"

using namespace std;

common gd_common;

distinguish::distinguish(int width, int height, byte *&data, byte ret[CHAR_PROCESS_NUM], const char *filePath)
{
    
}

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