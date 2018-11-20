#pragma once
#include "pch.h"

template<typename T>
void fill_matrix_with_value(BasicMatrix<T>& matrix, const T value)
{
	for (long row = 0; row < matrix.rows(); row++)
	{
		for (long col = 0; col < matrix.cols(); col++)
		{
			matrix(row, col) = value;
		}
	}
}

void fill_matrix_with_random_int_values(BasicMatrix<int>& matrix, const int min, const int max);
void fill_matrix_with_random_double_values(BasicMatrix<double>& matrix, const double min, const double max);

template <typename T>
void print_matrix(const BasicMatrix<T>& matrix, const std::string format)
{
	for (long row = 0; row < matrix.rows(); row++)
	{
		printf("[");
		for (long col = 0; col < matrix.cols(); col++)
		{
			printf(format.c_str(), matrix(row, col));
		}
		printf("]");
		printf("\n");
	}
}
