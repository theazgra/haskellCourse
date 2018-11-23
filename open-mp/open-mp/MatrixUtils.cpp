#include "pch.h"

void fill_matrix_with_random_int_values(BasicMatrix<int>& matrix, const int min, const int max)
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

void fill_matrix_with_random_double_values(BasicMatrix<double>& matrix, const double min, const double max)
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

bool is_correct_result(const BasicMatrix<int>& actual, const BasicMatrix<int>& expected)
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