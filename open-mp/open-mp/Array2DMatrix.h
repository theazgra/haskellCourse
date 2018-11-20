#pragma once
#include "pch.h"

template <typename T>
class Array2DMatrix : public BasicMatrix<T>
{
private:
	T** data;
public:
	Array2DMatrix(long rows, long cols) : BasicMatrix<T>(rows, cols)
	{
		data = new T*[rows];
		for (long row = 0; row < rows; row++)
			data[row] = new T[cols];
	}

	~Array2DMatrix()
	{
		for (long row = 0; row < this->rowCount; row++)
		{
			delete[] data[row];
		}
		delete[] data;
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
