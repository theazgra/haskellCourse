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