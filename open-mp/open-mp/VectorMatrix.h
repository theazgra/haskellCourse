#pragma once
#include "pch.h"

template <typename T>
class VectorMatrix : public BasicMatrix<T>
{
private:
	std::vector<T> data;
public:
	VectorMatrix(long rows, long cols) : BasicMatrix<T>(rows, cols)
	{
		data.resize(rows * cols);
	}

	~VectorMatrix()
	{
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
