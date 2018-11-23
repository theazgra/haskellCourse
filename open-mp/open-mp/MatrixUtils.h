#pragma once
#include "pch.h"

bool is_correct_result(const BasicMatrix<int> &actual, const BasicMatrix<int> &expected);
ArrayMatrix<int> get_test_array_matrix(long dimension);
Array2DMatrix<int> get_test_array2d_matrix(long dimension);
VectorMatrix<int> get_test_vector_matrix(long dimension);
Vector2DMatrix<int> get_test_vector2d_matrix(long dimension);

void fill_matrix_with_random_int_values(BasicMatrix<int> &matrix, const int min = INT_FAST16_MIN, const int max = INT_FAST16_MAX);
void fill_matrix_with_random_double_values(BasicMatrix<double> &matrix, const double min = INT_FAST16_MIN, const double max = INT_FAST16_MAX);

template <typename T>
void fill_matrix_with_value(BasicMatrix<T> &matrix, const T value)
{
#pragma omp parallel for
	for (long row = 0; row < matrix.rows(); row++)
	{
		for (long col = 0; col < matrix.cols(); col++)
		{
			matrix(row, col) = value;
		}
	}
}

template <typename T>
void print_matrix(const BasicMatrix<T> &matrix, const std::string format)
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

template <typename T>
void copy_matrix(const BasicMatrix<T> &source, BasicMatrix<T> &dest)
{
	for (long row = 0; row < source.rows(); row++)
	{
		for (long col = 0; col < source.cols(); col++)
		{
			dest(row, col) = source(row, col);
		}
	}
}
