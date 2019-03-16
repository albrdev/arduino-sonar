#ifndef __MATH_H__
#define __MATH_H__

#include <math.h>   /* M_PI, M_E, abs(), fabs(), floor(), ceil(), fmod(), log10(), pow(), sqrt() */

#define sgn(x) (((x) > 0) - ((x) < 0))

#define absdiff(a, b) (abs((a) - (b)))

#define normalize01(x, min, max) (((x) - (min)) / ((max) - (min)))

#endif // __MATH_H__
