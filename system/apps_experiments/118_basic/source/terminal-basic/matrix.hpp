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

/**
 * @file matrix.hpp
 * @brief Simle template package of row-major matrix operations
 */

#ifndef MATRIX_HPP
#define MATRIX_HPP

#include "arduinoext.hpp"
#include "math.hpp"
#include "types.hpp"

#define DEBUG 0
#if DEBUG
#include <iostream>
#define printV(a) std::cout << a << std::endl;
#define out(m, M, N) \
for (size_t _i=0; _i<M; ++_i) { \
	for (size_t _j=0; _j<M; ++_j) { \
		std::cout << m[_i*N+_j] << '\t'; \
	} \
	std::cout << '\n'; \
} \
std::cout << std::endl;

#else
#define printV(a)
#define out(m, M, N)
#endif

/**
 * @package Matrix
 * @brief Row-major matrix operations generic package
 */
template <typename T>
Package(Matricies)
{
	EXT_PACKAGE(Matricies)
public:
#define elm(d,m,n) (d[m*N+n])
	/**
	 * @brief In-place transposiion of the matrix
	 * 
	 * @param data
	 * @param M
	 * @param N
	 */
	static void transpose(T *data, size_t M, size_t N)
	{
		if (M == N) { // square matrix
			for (size_t n = 0; n < (N - 1); ++n) {
				for (size_t m = n + 1; m < N; ++m) {
					const T buf = elm(data, n, m);
					elm(data, n, m) = elm(data, m, n);
					elm(data, m, n) = buf;
				}
			}
		} else { // non-square, cycles algorithm
			// code from https://softwareengineering.stackexchange.com/
			// questions/271713/transpose-a-matrix-without-a-buffering-one
			for (size_t start = 0; start <= N * M - 1; ++start) {
				size_t next = start;
				size_t i = 0;
				do {
					++i;
					next = (next % M) * N + next / M;
				} while (next > start);

				if (next >= start && i != 1) {
					const T tmp = data[start];
					next = start;
					do {
						i = (next % M) * N + next / M;
						data[next] = (i == start) ? tmp : data[i];
						next = i;
					} while (next > start);
				}
			}
		}
	}

	/**
	 * @brief Out-of-place multiplication
	 * @param data left operand
	 * @param N
	 * @param M
	 * @param data2 right operand
	 * @param N2
	 * @param M2
	 * @param buf result of size N*M2
	 * @return flag of success
	 */
	static bool mul(T *data, size_t N, size_t M, T *data2, size_t N2,
	    size_t M2, T * buf)
	{
#define A(d, n, i, j) d[i*n+j]
		if (M != N2)
			return false;

		for (size_t i = 0; i < N; ++i) {
			for (size_t j = 0; j < M2; ++j) {
				A(buf, M2, i, j) = 0;
				for (size_t k = 0; k < M; ++k) {
					A(buf, M2, i, j) += A(data, M, i, k) *
						A(data2, M2, k, j);
				}
			}
		}
#undef A
		return true;
	}

	static bool invert(T *data, size_t N, T * buffer)
	{
#define A(i,j) data[i*N+j]
#define B(i,j) result[i*N+j]
		for (size_t k = 0; k < N; ++k) {
			T pivot = A(k, k);
			printV(pivot);
			if (typespec<T>::isinteger) {
				if (pivot == 0)
					return false;
			} else if (typespec<T>::isreal) {
				if (math<T>::almost_zero(pivot))
					return false;
			} else
				return false;

			for (size_t j = 0; j < k; ++j)
				A(j, k) = -A(j, k) / pivot;
			A(k, k) = 0;
			for (size_t j = k + 1; j < N; ++j)
				A(j, k) = -A(j, k) / pivot;
			out(data, N, N);

			T *column = buffer;
			for (size_t i = 0; i < N; ++i)
				column[i] = A(i, k);
			T *row = buffer + N;
			for (size_t i = 0; i < N; ++i)
				row[i] = A(k, i);
			T *result = row + N;
			if (!mul(column, N, 1, row, 1, N, result))
				return false;
			for (size_t i = 0; i < N; ++i)
				for (size_t j = 0; j < N; ++j)
					A(i, j) += B(i, j);
			out(data, N, N);

			for (size_t j = 0; j < k; ++j)
				A(k, j) = A(k, j) / pivot;
			A(k, k) = T(1) / pivot;
			for (size_t j = k + 1; j < N; ++j)
				A(k, j) = A(k, j) / pivot;
			out(data, N, N);
		}
#undef A
#undef B
		return true;
	}

	static bool determinant(const T *data, size_t N, T &det, T * buffer)
	{
#define A(i,j) data[i*N+j]
#define B(i,j) buffer[i*(N-1)+j]
		det = 0;
		if (N == 1) {
			det = A(0, 0);
			return true;
		}

		int8_t sign = 1;

		auto getCofactor = [&](size_t f)
		{
			size_t i = 0, j = 0;
			for (size_t row = 1; row < N; ++row) {
				for (size_t col = 0; col < N; ++col) {
					if (col != f) {
						B(i, j++) = A(row, col);
						// Row is filled, so increase row index and
						// reset col index
						if (j == N - 1) {
							j = 0;
							++i;
						}
					}
				}
			}
		};

		for (size_t f = 0; f < N; ++f) {
			getCofactor(f);
			T D;
			determinant(buffer, N - 1, D, buffer + (N - 1)*(N - 1));
			det += sign * A(0, f) * D;
			sign = -sign;
		}

		return true;
#undef A
#undef B
#undef elm
	}
};

#endif // MATRIX_HPP
