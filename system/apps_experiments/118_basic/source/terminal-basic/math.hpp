/*
 * ArduinoExt is a set of utility libraries for Arduino
 * Copyright (C) 2016-2018 Andrey V. Skvortsov <starling13@mail.ru>
 * Copyright (C) 2019,2020 Terminal-BASIC team
 *
 * This program is free software: is free software: you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation, either version 3 of
 * the License, or (at your option) any later version.

 * ArduinoExt library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU Lesser General Public
 * License along with Posixcpp library. If not, see
 * <http://www.gnu.org/licenses/>.
 */

#ifndef MATH_HPP
#define MATH_HPP

#include <Arduino.h>
#include <math.h>
#include <limits.h>
#include <stdlib.h>
#include <float.h>
#include <inttypes.h>

#ifdef abs
#undef abs
#endif

#ifdef round
#undef round
#endif

#ifndef M_PI
#define M_PI		3.14159265358979323846
#endif

#ifndef M_PI_2
#define M_PI_2		1.57079632679489661923
#endif

#ifndef M_PI_2l
#define M_PI_2l M_PI_2
#endif

/**************************************************
 *
 *    ftoa - converts float to string
 *
 ***************************************************
 *
 *    This is a simple implemetation with rigid
 *    parameters:
 *            - Buffer must be 8 chars long
 *            - 3 digits precision max
 *            - absolute range is -524,287 to 524,287 
 *            - resolution (epsilon) is 0.125 and
 *              always rounds down
 **************************************************/

extern void ftoa(float Value, char* Buffer);

union Ufloat
{
	float f;
	uint32_t i;
};

union UDouble
{
	double f;
	uint64_t i;
};

#define RAD2DEGf(a) (a/float(M_PI)*180.f)
#define DEG2RADf(a) (a/180.f*float(M_PI))

#define RAD2DEG(a) (a/M_PI*180.)
#define DEG2RAD(a) (a/180.*M_PI)

#define RAD2DEGl(a) (a/M_PIl*180.l)
#define DEG2RADl(a) (a/180.l*M_PIl)

#define M_PIf float(M_PI)
#define M_PId M_PI

#define M_PI_2f float(M_PI_2)
#define M_PI_2d M_PI_2

#define M_PI3_2f (3.f*float(M_PI_2))
#define M_PI3_2d (3.*M_PI_2)
#define M_PI3_2l (3.l*M_PI_2l)

template <typename REAL>
class math
{
public:
	static REAL abs(REAL);
	static constexpr REAL pi();
	static constexpr REAL pi_2();
	static constexpr REAL pi3_2();
	static constexpr REAL minimum();
	static constexpr REAL maximum();
	static constexpr REAL epsilon();
	static REAL floor(REAL);
	static REAL round(REAL);
	static bool almost_zero(REAL, unsigned = 1);
	static bool almost_equal(REAL, REAL, unsigned = 1);
};

template <>
inline float math<float>
::abs(float arg)
{
	return ::fabsf(arg);
}

template <>
inline double math<double>
::abs(double arg)
{
	return ::fabs(arg);
}

template <>
inline float math<float>
::floor(float arg)
{
	return ::floorf(arg);
}

template <>
inline double math<double>
::floor(double arg)
{
	return ::floor(arg);
}

template <>
inline float math<float>
::round(float arg)
{
	return ::round(arg);
}

template <>
inline double math<double>
::round(double arg)
{
	return ::round(arg);
}

template <>
inline constexpr float math<float>
::pi()
{
	return M_PIf;
}

template <>
inline constexpr double math<double>
::pi()
{
	return M_PId;
}

template <>
inline constexpr long double math<long double>
::pi()
{
#ifdef M_PIl
	return M_PIl;
#else
	return M_PId;
#endif
}

template <>
inline constexpr float math<float>
::pi_2()
{
	return M_PI_2f;
}

template <>
inline constexpr double math<double>
::pi_2()
{
	return M_PI_2d;
}

template <>
inline constexpr long double math<long double>
::pi_2()
{
#ifdef M_PI_2l
	return M_PI_2l;
#else
	return M_PI_2d;
#endif
}

template <>
inline constexpr float math<float>
::pi3_2()
{
	return M_PI3_2f;
}

template <>
inline constexpr double math<double>
::pi3_2()
{
	return M_PI3_2d;
}

template <>
inline constexpr long double math<long double>
::pi3_2()
{
	return M_PI3_2l;
}

template <>
inline constexpr float math<float>
::minimum()
{
	return FLT_MIN;
}

template <>
inline constexpr double math<double>
::minimum()
{
	return DBL_MIN;
}

template <>
inline constexpr long double math<long double>
::minimum()
{
	return LDBL_MIN;
}

template <>
inline constexpr float math<float>
::maximum()
{
	return FLT_MAX;
}

template <>
inline constexpr double math<double>
::maximum()
{
	return DBL_MAX;
}

template <>
inline constexpr long double math<long double>
::maximum()
{
	return LDBL_MAX;
}

template <>
inline constexpr float math<float>
::epsilon()
{
	return FLT_EPSILON;
}

template <>
inline constexpr double math<double>
::epsilon()
{
	return DBL_EPSILON;
}

template <>
inline constexpr long double math<long double>
::epsilon()
{
	return LDBL_EPSILON;
}

/**
 * @brief Fuzzy compares the real number with zero using non-normalized precision
 *
 * @param d number
 * @param ulp precision
 */
template <typename REAL>
inline bool math<REAL>::
almost_zero(REAL d, unsigned ulp)
{
	return abs(d) <= minimum() * REAL(ulp);
}

/**
 * @brief Fuzzy compares the real numbers
 *
 * Code from http://en.cppreference.com/w/cpp/types/numeric_limits/epsilon
 */
template<typename T>
inline bool math<T>::
almost_equal(T x, T y, unsigned ulp)
{
	if (x != y) {
		if (x == T(0))
			return almost_zero(y, ulp);
		else if (y == T(0))
			return almost_zero(x, ulp);
		else
			// the machine epsilon has to be scaled to the magnitude of the values used
			// and multiplied by the desired precision in ULPs (units in the last place)
			return fabs(x - y) <= math<T>::epsilon() * max(abs(x), abs(y)) * ulp;
	} else
		return true;
}

inline bool
equals_zero(float f)
{
	Ufloat data = {.f = f};
	return (data.i & 0x7fffffff) == 0;
}

inline bool
equals_zero(double f)
{
	UDouble data = {.f = f};
	return (data.i & 0x7fffffffffffffff) == 0;
}

inline bool
equals_zero(long double f)
{
	return equals_zero(double(f));
}

enum class CartesianCoordinates2D_t : uint8_t
{
	X = 0, Y = 1, NUM_COORD
};

template <typename T>
struct CartesianCoordinates2D
{
	T get(CartesianCoordinates2D_t v) const
	{
		return data[uint8_t(v)];
	}
	
	T &coord(CartesianCoordinates2D_t v)
	{
		return data[uint8_t(v)];
	}
	
	T x() const { return data[0]; }
	T y() const { return data[1]; }
	void setX(T v) { data[0] = v; }
	void setY(T v) { data[1] = v; }
	
	T data[uint8_t(CartesianCoordinates2D_t::NUM_COORD)];
};

enum class CartesianCoordinates3D_t
{
	X = 0, Y = 1, Z = 2, NUM_COORD
};

enum class CartesianCoordinates4D_t
{
	X = 0, Y = 1, Z = 2, W = 3, NUM_COORD
};

#endif // MATH_HPP
