#include "pch.h"

//1 2 3 for result.rows { for result.cols { for A.cols } }, calculate point by point in the result matrix.
template <typename T>
void basic_multiplication_1(const BasicMatrix<T>& A, const BasicMatrix<T>& B, BasicMatrix<T>& result, double& elapsedMilliseconds)
{
	if (!(result.rows() == A.rows() && result.cols() == B.cols()))
		throw new std::exception("Result matrix has wrong dimensions.");

	Stopwatch s;


	s.start();
	for (long resRow = 0; resRow < result.rows(); resRow++)
	{
		for (long resCol = 0; resCol < result.cols(); resCol++)
		{
			for (long aCol = 0; aCol < A.cols(); aCol++)
			{
				result(resRow, resCol) += (A(resRow, aCol) * B(aCol, resCol));
			}
		}
	}
	s.stop();
	elapsedMilliseconds = s.elapsedMilliseconds();
}

//1 3 2 for result.rows { for A.cols { for result.cols } }, calculate 'whole row by increment'
template <typename T>
void basic_multiplication_2(const BasicMatrix<T>& A, const BasicMatrix<T>& B, BasicMatrix<T>& result, double& elapsedMilliseconds)
{
	if (!(result.rows() == A.rows() && result.cols() == B.cols()))
		throw new std::exception("Result matrix has wrong dimensions.");

	Stopwatch s;


	s.start();

	for (long resRow = 0; resRow < result.rows(); resRow++)
	{
		for (long aCol = 0; aCol < A.cols(); aCol++)
		{
			for (long resCol = 0; resCol < result.cols(); resCol++)
			{
				result(resRow, resCol) += (A(resRow, aCol) * B(aCol, resCol));
			}
		}
	}
	s.stop();
	elapsedMilliseconds = s.elapsedMilliseconds();
}

//2 1 3 for res.cols { for result.rows { for A.cols } }
template <typename T>
void basic_multiplication_3(const BasicMatrix<T>& A, const BasicMatrix<T>& B, BasicMatrix<T>& result, double& elapsedMilliseconds)
{
	if (!(result.rows() == A.rows() && result.cols() == B.cols()))
		throw new std::exception("Result matrix has wrong dimensions.");

	Stopwatch s;


	s.start();

	for (long resCol = 0; resCol < result.cols(); resCol++)
	{
		for (long resRow = 0; resRow < result.rows(); resRow++)
		{
			for (long aCol = 0; aCol < A.cols(); aCol++)
			{
				result(resRow, resCol) += (A(resRow, aCol) * B(aCol, resCol));
			}
		}
	}
	s.stop();
	elapsedMilliseconds = s.elapsedMilliseconds();
}

//2 3 1 for res.cols { for A.cols { for result.rows } }
template <typename T>
void basic_multiplication_4(const BasicMatrix<T>& A, const BasicMatrix<T>& B, BasicMatrix<T>& result, double& elapsedMilliseconds)
{
	if (!(result.rows() == A.rows() && result.cols() == B.cols()))
		throw new std::exception("Result matrix has wrong dimensions.");

	Stopwatch s;


	s.start();

	for (long resCol = 0; resCol < result.cols(); resCol++)
	{
		for (long aCol = 0; aCol < A.cols(); aCol++)
		{
			for (long resRow = 0; resRow < result.rows(); resRow++)
			{
				result(resRow, resCol) += (A(resRow, aCol) * B(aCol, resCol));
			}
		}
	}
	s.stop();
	elapsedMilliseconds = s.elapsedMilliseconds();
}

//3 1 2 for A.cols { for A.rows { for result.cols } }
template <typename T>
void basic_multiplication_5(const BasicMatrix<T>& A, const BasicMatrix<T>& B, BasicMatrix<T>& result, double& elapsedMilliseconds)
{
	if (!(result.rows() == A.rows() && result.cols() == B.cols()))
		throw new std::exception("Result matrix has wrong dimensions.");

	Stopwatch s;


	s.start();

	for (long aCol = 0; aCol < A.cols(); aCol++)
	{
		for (long resRow = 0; resRow < result.rows(); resRow++)
		{
			for (long resCol = 0; resCol < result.cols(); resCol++)
			{
				result(resRow, resCol) += (A(resRow, aCol) * B(aCol, resCol));
			}
		}
	}
	s.stop();
	elapsedMilliseconds = s.elapsedMilliseconds();
}

//3 2 1 for A.cols { for res.cols { for result.rows } }
template <typename T>
void basic_multiplication_6(const BasicMatrix<T>& A, const BasicMatrix<T>& B, BasicMatrix<T>& result, double& elapsedMilliseconds)
{
	if (!(result.rows() == A.rows() && result.cols() == B.cols()))
		throw new std::exception("Result matrix has wrong dimensions.");

	Stopwatch s;


	s.start();

	for (long aCol = 0; aCol < A.cols(); aCol++)
	{
		for (long resCol = 0; resCol < result.cols(); resCol++)
		{
			for (long resRow = 0; resRow < result.rows(); resRow++)
			{
				result(resRow, resCol) += (A(resRow, aCol) * B(aCol, resCol));
			}
		}
	}
	s.stop();
	elapsedMilliseconds = s.elapsedMilliseconds();
}