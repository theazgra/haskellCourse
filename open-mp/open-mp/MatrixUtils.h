#pragma once
#include "pch.h"


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

bool is_correct_result(const BasicMatrix<int> &actual, const BasicMatrix<int> &expected)
{
	if (actual.rows() != expected.rows() || actual.cols() != expected.cols())
		return false;

	for (long row = 0; row < expected.rows(); row++)
	{
		for (long col = 0; col < expected.cols(); col++)
		{
			if (actual(row, col) != expected(row, col))
				return false;
		}
	}
	return true;
}

ArrayMatrix<int> get_fixed_test_array_matrix(long dimension)
{
	ArrayMatrix<int> result = ArrayMatrix<int>(dimension, dimension);
	
	
	for(long row = 0; row < result.rows(); row++)
	{
		for(long col = 0; col < result.cols(); col++)
		{
			result(row, col) = (int)row + (int)col;
		}
	}
	return result;
}

ArrayMatrix<int> get_test_array_matrix(long dimension)
{
	ArrayMatrix<int> result = ArrayMatrix<int>(dimension, dimension);
	fill_matrix_with_value(result, 1);
	return result;
}

Array2DMatrix<int> get_test_array2d_matrix(long dimension)
{
	Array2DMatrix<int> result = Array2DMatrix<int>(dimension, dimension);
	fill_matrix_with_value(result, 1);
	return result;
}

VectorMatrix<int> get_test_vector_matrix(long dimension)
{
	VectorMatrix<int> result = VectorMatrix<int>(dimension, dimension);
	fill_matrix_with_value(result, 1);
	return result;
}

Vector2DMatrix<int> get_test_vector2d_matrix(long dimension)
{
	Vector2DMatrix<int> result = Vector2DMatrix<int>(dimension, dimension);
	fill_matrix_with_value(result, 1);
	return result;
}

void fill_matrix_with_random_int_values(BasicMatrix<int> &matrix, const int min = MY_INT_MIN, const int max = MY_INT_MAX)
{
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_int_distribution<int> dist(min, max);

	for (long row = 0; row < matrix.rows(); row++)
	{
		for (long col = 0; col < matrix.cols(); col++)
		{
			matrix(row, col) = dist(mt);
		}
	}
}

void fill_matrix_with_random_double_values(BasicMatrix<double> &matrix, const double min = MY_INT_MIN, const double max = MY_INT_MAX)
{
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_real_distribution<double> dist(min, max);

	for (long row = 0; row < matrix.rows(); row++)
	{
		for (long col = 0; col < matrix.cols(); col++)
		{
			matrix(row, col) = dist(mt);
		}
	}
}
