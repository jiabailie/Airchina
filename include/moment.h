/*
 * aryFebruary, 16th, 2014.
 * Calculate image moments.
 */
#pragma once

#include "type.h"

struct moment
{
	ll calMpq(int, int, int, int, byte*);

	double calCenterMoment(int, int, int, int, byte*);

	void calRegularMoment(int, int, byte*, double[]);

	double calMomentSimilar(double[], double[]);

	double calMomentSimilarByCos(double[], double[]);
};