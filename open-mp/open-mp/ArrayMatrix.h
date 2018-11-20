#pragma once
#include "pch.h"

template <typename T>
class ArrayMatrix : public BasicMatrix<T>
{
private:
	T* data;
public:
	ArrayMatrix(long rows, long cols) : BasicMatrix<T>(rows, cols)
	{
		data = new T[rows * cols];
	}

	~ArrayMatrix()
	{
		delete[] data;
	}

	T& operator()(long row, long col)
	{
#if CHECK_MATRIX_ACCESS
		if (row >= this->rowCount || col >= this->colCount)
			throw std::exception("BAD ACCESS!");
#endif
		return data[((row * this->colCount) + col)];
	}

	const T& operator()(long row,long col) const
	{
#if CHECK_MATRIX_ACCESS
		if (row >= this->rowCount || col >= this->colCount)
			throw std::exception("BAD ACCESS!");
#endif
		return data[((row * this->colCount) + col)];
	}



};
