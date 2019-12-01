/** @defgroup can_defines CAN defines

@ingroup STM32F_defines

@brief <b>libopencm3 Defined Constants and Types for STM32 CAN </b>

@version 1.0.0

@author @htmlonly &copy; @endhtmlonly 2010 Piotr Esden-Tempski <piotr@esden.net>

@date 12 November 2012

LGPL License Terms @ref lgpl_license
*/
/*
 * This file is part of the libopencm3 project.
 *
 * Copyright (C) 2010 Piotr Esden-Tempski <piotr@esden.net>
 *
 * This library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <libopencm3/stm32/memorymap.h>
#include <libopencm3/cm3/common.h>

#if defined(STM32F0)
#       include <libopencm3/stm32/f0/can.h>
#elif defined(STM32F1)
#       include <libopencm3/stm32/f1/can.h>
#elif defined(STM32F2)
#       include <libopencm3/stm32/f2/can.h>
#elif defined(STM32F3)
#       include <libopencm3/stm32/f3/can.h>
#elif defined(STM32F4)
#       include <libopencm3/stm32/f4/can.h>
#else
#       error "CAN not supported for this device."
#endif
