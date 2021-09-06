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

/**
 * @file basic_math.hpp
 * @brief Mathematical functions container
 */

#ifndef BASIC_MATH_HPP
#define BASIC_MATH_HPP

#include "basic_functionblock.hpp"
#include "basic_interpreter.hpp"

#if USEMATH
#if !USE_REALS
#error Math module requires real arithmetics support
#endif
#endif // USEMATH

namespace BASIC
{

class Math : public FunctionBlock
{
public:
	explicit Math();
private:
#if M_TRIGONOMETRIC
	static bool func_cos(Interpreter&);
	static bool func_cot(Interpreter&);
	static bool func_sin(Interpreter&);
	static bool func_tan(Interpreter&);
	
	static Real cos_r(Real);
	static Real cot_r(Real);
	static Real sin_r(Real);
	static Real tan_r(Real);
#if USE_LONG_REALS
	static bool func_cos_lr(Interpreter&);
	static bool func_cot_lr(Interpreter&);
	static bool func_sin_lr(Interpreter&);
	static bool func_tan_lr(Interpreter&);
	
	static LongReal cos_lr(LongReal);
	static LongReal cot_lr(LongReal);
	static LongReal sin_lr(LongReal);
	static LongReal tan_lr(LongReal);
#endif // USE_LONG_REALS
#endif // M_TRIGONOMETRIC
#if M_REVERSE_TRIGONOMETRIC
	static bool func_acs(Interpreter&);
	static bool func_asn(Interpreter&);
	static bool func_atn(Interpreter&);
	
	static Real acs_r(Real);
	static Real asn_r(Real);
	static Real atn_r(Real);
#if USE_LONG_REALS
	static bool func_acs_lr(Interpreter&);
	static bool func_asn_lr(Interpreter&);
	static bool func_atn_lr(Interpreter&);
	
	static LongReal acs_lr(LongReal);
	static LongReal asn_lr(LongReal);
	static LongReal atn_lr(LongReal);
#endif
#if M_ADDITIONAL
	static bool func_atn2(Interpreter&);
#if USE_LONG_REALS
	static bool func_atn2_lr(Interpreter&);
#endif
#endif // M_ADDITIONAL
#endif // M_REVERSE_TRIGONOMETRIC
#if M_HYPERBOLIC
	static bool func_cosh(Interpreter&);
	static bool func_sinh(Interpreter&);
	static bool func_tanh(Interpreter&);
        
	static Real cosh_r(Real);
	static Real sinh_r(Real);
	static Real tanh_r(Real);
#if USE_LONG_REALS
	static bool func_cosh_lr(Interpreter&);
	static bool func_sinh_lr(Interpreter&);
	static bool func_tanh_lr(Interpreter&);
        
	static LongReal cosh_lr(LongReal);
	static LongReal sinh_lr(LongReal);
	static LongReal tanh_lr(LongReal);
#endif // USE_LONG_REALS
#endif // M_HYPERBOLIC
	static bool func_exp(Interpreter&);
	static bool func_log(Interpreter&);
	static bool func_sqr(Interpreter&);
	static bool func_pi(Interpreter&);
	
	static Real exp_r(Real);
	static Real log_r(Real);
	static Real sqr_r(Real);
#if USE_LONG_REALS
	static bool func_exp_lr(Interpreter&);
	static bool func_log_lr(Interpreter&);
	static bool func_sqr_lr(Interpreter&);
	static bool func_pi_lr(Interpreter&);
	
	static LongReal exp_lr(LongReal);
	static LongReal log_lr(LongReal);
	static LongReal sqr_lr(LongReal);
#endif // USE_LONG_REALS
#if M_ADDITIONAL
	static bool func_cbr(Interpreter&);
	static bool func_hyp(Interpreter&);
	static bool func_log10(Interpreter&);
	
	static Real cbr_r(Real);
	static Real log10_r(Real);
#if USE_LONG_REALS
	static bool func_cbr_lr(Interpreter&);
	static bool func_hyp_lr(Interpreter&);
	static bool func_log10_lr(Interpreter&);
	
	static LongReal cbr_lr(LongReal);
	static LongReal log10_lr(LongReal);
#endif // USE_LONG_REALS
#endif // M_ADDITIONAL
	static const FunctionBlock::function funcs[] PROGMEM;;
};

} // namespace BASIC

#endif // BASIC_MATH_HPP
