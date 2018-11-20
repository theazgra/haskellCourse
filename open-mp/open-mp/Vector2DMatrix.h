#pragma once
#include "pch.h"

template <typename T>
class Vector2DMatrix : public BasicMatrix<T>
{
private:
	std::vector<std::vector<T>> data;
public:
	Vector2DMatrix(long rows, long cols) : BasicMatrix<T>(rows, cols)
	{
		data.resize(rows);
		for (long row = 0; row < rows; row++)
		{
			data[row].resize(cols);
		}
	}

	~Vector2DMatrix()
	{
	}

	T& operator()(long row, long col)
	{
#if CHECK_MATRIX_ACCESS
		if (row >= this->rowCount || col >= this->colCount)
			throw std::exception("BAD ACCESS!");
#endif
		return data[row][col];
	}

	const T& operator()(long row,long col) const
	{
#if CHECK_MATRIX_ACCESS
		if (row >= this->rowCount || col >= this->colCount)
			throw std::exception("BAD ACCESS!");
#endif
		return data[row][col];
	}



};
