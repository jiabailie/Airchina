/*
 * January, 23rd, 2014.
 * Mini-size functions.
 */
#pragma once

template<typename T>
inline T iabs(T x)
{
    return x > 0 ? x : -x;
}

template<typename T>
inline T imax(T x, T y)
{
    return x > y ? x : y;
}

template<typename T>
inline T imin(T x, T y)
{
    return x < y ? x : y;
}

template<typename T>
inline T index(T x, T y, T width)
{
    return (x * width + y) * 3;
}

// To determine whether x is equal to 0.
inline int iszero(double x)
{
	if(x - 0 < 1e-9) { return 1; }
	return 0;
}