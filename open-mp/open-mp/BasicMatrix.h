#pragma once
#include "pch.h"

template <typename T>
class BasicMatrix
{
  protected:
	long rowCount;
	long colCount;

  public:
	BasicMatrix(long rows, long cols)
	{
		this->rowCount = rows;
		this->colCount = cols;
	}

	virtual ~BasicMatrix()
	{
	}

	long cols() const
	{
		return this->colCount;
	}

	long rows() const
	{
		return this->rowCount;
	}

	virtual T &operator()(long row, long col) = 0;
	virtual const T &operator()(long row, long col) const = 0;
};
