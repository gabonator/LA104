/*
 * Terminal-BASIC is a lightweight BASIC-like language interpreter
 * 
 * Copyright (C) 2016-2018 Andrey V. Skvortsov <starling13@mail.ru>
 * Copyright (C) 2019,2020 Terminal-BASIC team
 *     <https://bitbucket.org/%7Bf50d6fee-8627-4ce4-848d-829168eedae5%7D/>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "basic.hpp"

#if USEMATH

#include "basic_math.hpp"
#include "math.hpp"
#include <string.h>

#include "ascii.hpp"
#include "helper.hpp"

namespace BASIC
{

static const uint8_t mathTokens[] PROGMEM = {
#if M_REVERSE_TRIGONOMETRIC
#if USE_LONG_REALS
	'A', 'C', 'S', '!', ASCII_NUL,
#endif
	'A', 'C', 'S', ASCII_NUL,
#if USE_LONG_REALS
	'A', 'S', 'N', '!', ASCII_NUL,
#endif
	'A', 'S', 'N', ASCII_NUL,
#if M_ADDITIONAL
#if USE_LONG_REALS
	'A', 'T', 'N', '!', ASCII_NUL,
#endif
#if USE_LONG_REALS
	'A', 'T', 'N', '2', '!', ASCII_NUL,
#endif
	'A', 'T', 'N', '2', ASCII_NUL,
#endif
	'A', 'T', 'N', ASCII_NUL,
#endif // M_REVERSE_TRIGONOMETRIC
#if M_ADDITIONAL
#if USE_LONG_REALS
	'C', 'B', 'R', '!', ASCII_NUL,
#endif
	'C', 'B', 'R', ASCII_NUL,
#endif // M_ADDITIONAL
#if M_TRIGONOMETRIC
#if USE_LONG_REALS
	'C', 'O', 'S', '!', ASCII_NUL,
#endif
#endif
#if M_HYPERBOLIC
#if USE_LONG_REALS
	'C', 'O', 'S', 'H', '!', ASCII_NUL,
#endif
        'C', 'O', 'S', 'H', ASCII_NUL,
#endif
#if M_TRIGONOMETRIC
	'C', 'O', 'S', ASCII_NUL,
#if USE_LONG_REALS
	'C', 'O', 'T', '!', ASCII_NUL,
#endif
	'C', 'O', 'T', ASCII_NUL,
#endif // M_TRIGONOMETRIC
#if USE_LONG_REALS
	'E', 'X', 'P', '!', ASCII_NUL,
#endif
	'E', 'X', 'P', ASCII_NUL,
#if M_ADDITIONAL
#if USE_LONG_REALS
	'H', 'Y', 'P', '!', ASCII_NUL,
#endif
	'H', 'Y', 'P', ASCII_NUL,
#endif // M_ADDITIONAL
#if USE_LONG_REALS
	'L', 'O', 'G', '!', ASCII_NUL,
#if M_ADDITIONAL
	'L', 'O', 'G', '1', '0', '!', ASCII_NUL,
#endif
#endif // USE_LONG_REALS
#if M_ADDITIONAL
	'L', 'O', 'G', '1', '0', ASCII_NUL,
#endif
	'L', 'O', 'G', ASCII_NUL,
#if USE_LONG_REALS
	'P', 'I', '!', ASCII_NUL,
#endif
	'P', 'I', ASCII_NUL,
#if M_TRIGONOMETRIC
#if USE_LONG_REALS
	'S', 'I', 'N', '!', ASCII_NUL,
#endif
#endif
#if M_HYPERBOLIC
#if USE_LONG_REALS
	'S', 'I', 'N', 'H', '!', ASCII_NUL,
#endif
	'S', 'I', 'N', 'H', ASCII_NUL,
#endif
#if M_TRIGONOMETRIC
	'S', 'I', 'N', ASCII_NUL,
#endif
#if USE_LONG_REALS
	'S', 'Q', 'R', '!', ASCII_NUL,
#endif
	'S', 'Q', 'R', ASCII_NUL,
#if M_TRIGONOMETRIC
#if USE_LONG_REALS
	'T', 'A', 'N', '!', ASCII_NUL,
#endif
#endif // M_TRIGONOMETRIC
#if M_HYPERBOLIC
#if USE_LONG_REALS
	'T', 'A', 'N', 'H', '!', ASCII_NUL,
#endif
	'T', 'A', 'N', 'H', ASCII_NUL,
#endif // M_HYPERBOLIC
#if M_TRIGONOMETRIC
	'T', 'A', 'N', ASCII_NUL,
#endif // M_TRIGONOMETRIC
	ASCII_ETX
};

const FunctionBlock::function Math::funcs[] PROGMEM = {
#if M_REVERSE_TRIGONOMETRIC
#if USE_LONG_REALS
	Math::func_acs_lr,
#endif
	Math::func_acs,
#if USE_LONG_REALS
	Math::func_asn_lr,
#endif
	Math::func_asn,
#if M_ADDITIONAL
#if USE_LONG_REALS
	Math::func_atn_lr,
#endif
#if USE_LONG_REALS
	Math::func_atn2_lr,
#endif
	Math::func_atn2,
#endif
	Math::func_atn,
#endif
#if M_ADDITIONAL
#if USE_LONG_REALS
	Math::func_cbr_lr,
#endif
	Math::func_cbr,
#endif
#if M_TRIGONOMETRIC
#if USE_LONG_REALS
	Math::func_cos_lr,
#endif
#endif
#if M_HYPERBOLIC
#if USE_LONG_REALS
	Math::func_cosh_lr,
#endif
        Math::func_cosh,
#endif
#if M_TRIGONOMETRIC
	Math::func_cos,
#if USE_LONG_REALS
	Math::func_cot_lr,
#endif
	Math::func_cot,
#endif
#if USE_LONG_REALS
	Math::func_exp_lr,
#endif
	Math::func_exp,
#if M_ADDITIONAL
#if USE_LONG_REALS
	Math::func_hyp_lr,
#endif
	Math::func_hyp,
#endif // M_ADDITIONAL
#if USE_LONG_REALS
	Math::func_log_lr,
#endif
#if M_ADDITIONAL
#if USE_LONG_REALS
	Math::func_log10_lr,
#endif
	Math::func_log10,
#endif // M_ADDITIONAL
	Math::func_log,
#if USE_LONG_REALS
	Math::func_pi_lr,
#endif
	Math::func_pi,
#if M_TRIGONOMETRIC
#if USE_LONG_REALS
	Math::func_sin_lr,
#endif
#endif
#if M_HYPERBOLIC
#if USE_LONG_REALS
	Math::func_sinh_lr,
#endif
	Math::func_sinh,        
#endif
#if M_TRIGONOMETRIC
	Math::func_sin,
#endif
#if USE_LONG_REALS
	Math::func_sqr_lr,
#endif
	Math::func_sqr
#if M_TRIGONOMETRIC
#if USE_LONG_REALS
	,Math::func_tan_lr
#endif
#endif // M_TRIGONOMETRIC
#if M_HYPERBOLIC
#if USE_LONG_REALS
	,Math::func_tanh_lr
#endif
	,Math::func_tanh
#endif // M_HYPERBOLIC
#if M_TRIGONOMETRIC
	,Math::func_tan
#endif
};

Math::Math()
{
	functions = funcs;
	functionTokens = mathTokens;
}

#if M_REVERSE_TRIGONOMETRIC

#if USE_LONG_REALS
bool
Math::func_acs_lr(Interpreter &i)
{
	return general_func(i, &acs_lr);
}

bool
Math::func_asn_lr(Interpreter &i)
{
	return general_func(i, &asn_lr);
}

bool
Math::func_atn_lr(Interpreter &i)
{
	return general_func(i, &atn_lr);
}
#endif // USE_LONG_REALS

bool
Math::func_acs(Interpreter &i)
{
	return general_func(i, &acs_r);
}

bool
Math::func_asn(Interpreter &i)
{
	return general_func(i, &asn_r);
}

bool
Math::func_atn(Interpreter &i)
{
	return general_func(i, &atn_r);
}

#if M_ADDITIONAL
bool
Math::func_atn2(Interpreter &i)
{
	Parser::Value v, v2;
	if (i.popValue(v2)) {
		if (i.popValue(v)) {
			v = Real(atan2(Real(v2), Real(v)));
			if (i.pushValue(v))
				return true;
		}
	}
	return false;
}
#if USE_LONG_REALS
bool
Math::func_atn2_lr(Interpreter &i)
{
	Parser::Value v, v2;
	if (i.popValue(v2)) {
		if (i.popValue(v)) {
			v = LongReal(atan2(LongReal(v2), LongReal(v)));
			if (i.pushValue(v))
				return true;
		}
	}
	return false;
}
#endif
#endif // M_ADDITIONAL
#endif // M_REVERSE_TRIGONOMETRIC

bool
Math::func_exp(Interpreter &i)
{
	return general_func(i, &exp_r);
}

bool
Math::func_log(Interpreter &i)
{
	return general_func(i, &log_r);
}

#if USE_LONG_REALS
bool
Math::func_exp_lr(Interpreter &i)
{
	return general_func(i, &exp_lr);
}

bool
Math::func_log_lr(Interpreter &i)
{
	return general_func(i, &log_lr);
}
#endif

#if M_TRIGONOMETRIC
bool
Math::func_cos(Interpreter &i)
{
	return general_func(i, &cos_r);
}

bool
Math::func_cot(Interpreter &i)
{
	return general_func(i, &cot_r);
}

bool
Math::func_sin(Interpreter &i)
{
	return general_func(i, &sin_r);
}

bool
Math::func_tan(Interpreter &i)
{
	return general_func(i, &tan_r);
}

#if USE_LONG_REALS
bool
Math::func_cos_lr(Interpreter &i)
{
	return general_func(i, &cos_lr);
}

bool
Math::func_cot_lr(Interpreter &i)
{
	return general_func(i, &cot_lr);
}

bool
Math::func_sin_lr(Interpreter &i)
{
	return general_func(i, &sin_lr);
}

bool
Math::func_tan_lr(Interpreter &i)
{
	return general_func(i, &tan_lr);
}
#endif // USE_LONG_REALS

Real
Math::sin_r(Real v)
{
	return sinf(v);
}

Real
Math::cos_r(Real v)
{
	return cosf(v);
}

Real
Math::cot_r(Real v)
{
	return Real(1) / tanf(v);
}

Real
Math::tan_r(Real v)
{
	return tanf(v);
}

#if USE_LONG_REALS
LongReal
Math::sin_lr(LongReal v)
{
	return sin(v);
}

LongReal
Math::cos_lr(LongReal v)
{
	return cos(v);
}

LongReal
Math::cot_lr(LongReal v)
{
	return LongReal(1) / tan(v);
}

LongReal
Math::tan_lr(LongReal v)
{
	return tan(v);
}
#endif // USE_LONG_REALS
#endif // M_TRIGONOMETRIC

#if M_HYPERBOLIC
bool
Math::func_cosh(Interpreter &i)
{
	return general_func(i, &cosh_r);
}

bool
Math::func_sinh(Interpreter &i)
{
	return general_func(i, &sinh_r);
}

bool
Math::func_tanh(Interpreter &i)
{
	return general_func(i, &tanh_r);
}

Real
Math::cosh_r(Real v)
{
	return coshf(v);
}

Real
Math::sinh_r(Real v)
{
	return sinhf(v);
}

Real
Math::tanh_r(Real v)
{
	return tanhf(v);
}

#if USE_LONG_REALS
bool
Math::func_cosh_lr(Interpreter &i)
{
	return general_func(i, &cosh_lr);
}

bool
Math::func_sinh_lr(Interpreter &i)
{
	return general_func(i, &sinh_lr);
}

bool
Math::func_tanh_lr(Interpreter &i)
{
	return general_func(i, &tanh_lr);
}

LongReal
Math::cosh_lr(LongReal v)
{
	return coshf(v);
}

LongReal
Math::sinh_lr(LongReal v)
{
	return sinhf(v);
}

LongReal
Math::tanh_lr(LongReal v)
{
	return tanhf(v);
}
#endif // USE_LONG_REALS
#endif // M_HYPERBOLIC

bool
Math::func_sqr(Interpreter &i)
{
	return general_func(i, &sqr_r);
}

bool
Math::func_pi(Interpreter &i)
{
	Parser::Value v(Real(M_PIf));
	return i.pushValue(v);
}

#if USE_LONG_REALS

bool
Math::func_sqr_lr(Interpreter &i)
{
	return general_func(i, &sqr_lr);
}

bool
Math::func_pi_lr(Interpreter &i)
{
	Parser::Value v(LongReal(M_PI));
	return i.pushValue(v);
}

#endif // USE_LONG_REALS

#if M_ADDITIONAL
bool
Math::func_cbr(Interpreter &i)
{
	return general_func(i, &cbr_r);
}

bool
Math::func_hyp(Interpreter& i)
{
	Parser::Value v, v2;
	if (i.popValue(v2)) {
		if (i.popValue(v)) {
			v = Real(hypotf(Real(v), Real(v2)));
			if (i.pushValue(v))
				return true;
		}
	}
	return false;
}

bool
Math::func_log10(Interpreter &i)
{
	return general_func(i, &log10_r);
}

#if USE_LONG_REALS
bool
Math::func_cbr_lr(Interpreter &i)
{
	return general_func(i, &cbr_lr);
}

bool
Math::func_hyp_lr(Interpreter& i)
{
	Parser::Value v, v2;
	if (i.popValue(v2)) {
		if (i.popValue(v)) {
			v = LongReal(hypot(LongReal(v), LongReal(v2)));
			if (i.pushValue(v))
				return true;
		}
	}
	return false;
}

bool
Math::func_log10_lr(Interpreter &i)
{
	return general_func(i, &log10_lr);
}
#endif // USE_LONG_REALS
#endif // M_ADDITIONAL

#if M_REVERSE_TRIGONOMETRIC

#if USE_LONG_REALS
LongReal
Math::acs_lr(LongReal v)
{
	return acos(v);
}

LongReal
Math::asn_lr(LongReal v)
{
	return asin(v);
}

LongReal
Math::atn_lr(LongReal v)
{
	return atan(v);
}
#endif // USE_LONG_REALS

Real
Math::acs_r(Real v)
{
	return acosf(v);
}

Real
Math::asn_r(Real v)
{
	return asinf(v);
}

Real
Math::atn_r(Real v)
{
	return atanf(v);
}
#endif // M_REVERSE_TRIGONOMETRIC

Real
Math::exp_r(Real v)
{
	return expf(v);
}

Real
Math::log_r(Real v)
{
	return logf(v);
}

Real
Math::sqr_r(Real v)
{
	return sqrtf(v);
}

#if USE_LONG_REALS

LongReal
Math::exp_lr(LongReal v)
{
	return exp(v);
}

LongReal
Math::log_lr(LongReal v)
{
	return log(v);
}

LongReal
Math::sqr_lr(LongReal v)
{
	return sqrt(v);
}

#endif // USE_LONG_REALS

#if M_ADDITIONAL
Real
Math::cbr_r(Real v)
{
	return cbrt(v);
}

Real
Math::log10_r(Real v)
{
	return log10(v);
}

#if USE_LONG_REALS
LongReal
Math::cbr_lr(LongReal v)
{
	return cbrt(v);
}

LongReal
Math::log10_lr(LongReal v)
{
	return log10(v);
}
#endif // USE_LONG_REALS
#endif // M_ADDITIONAL

} // namespace BASIC

#endif // USEMATH
