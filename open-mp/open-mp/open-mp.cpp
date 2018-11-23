#include "pch.h"
#define PI_APPROX_USING_OMP_FOR 1

inline bool is_in(float x, float y)
{
	float distance = sqrt(pow(x, 2) + pow(y, 2));
	//float distance = sqrt((x*x) + (y*y));
	return (distance <= 1);
}


void approx_pi(void)
{
	//	omp_set_num_threads(8);
	//	const int numberOfSteps = 1000;
	//	long sum = 0;
	//
	//#pragma omp parallel
	//	{
	//		int local = 0;
	//		for (int i = 0; i < numberOfSteps; i++)
	//		{
	//			local += i;
	//		}
	//#pragma omp atomic
	//		sum += local;
	//	}


	const int threadCount = 12;
	const int numberOfTries = 30000000;
	omp_set_num_threads(threadCount);
	int totalInCount = 0;


	Stopwatch s;
	s.start();

#pragma omp parallel
	{
		int threadInCount = 0;
		float x, y;

		std::random_device rd;
		std::mt19937 mt(rd());
		std::uniform_real_distribution<double> dist(0.0, 1.0);

#if PI_APPROX_USING_OMP_FOR
#pragma omp for
		for (int i = 0; i < numberOfTries; i++)
#else
		for (int i = 0; i < (numberOfTries / threadCount); i++)
#endif
		{
			x = dist(mt);
			y = dist(mt);

			threadInCount += is_in(x, y) ? 1 : 0;
		}


#pragma omp critical
		{
			totalInCount += threadInCount;
		}
	}
	double approxPi = ((double)totalInCount / (double)numberOfTries) * (double)4.0;
	s.stop();
	printf("Approximed pi at: %f\n", approxPi);
	printf("Took: %f ms\n", s.elapsedMilliseconds());

	// 10mil threaded	- 311.255600 ms
	// 10mil single		- 1204.019200 ms

	// 30mil threaded	- 746.867600 ms
	// 30mil single		- 3061.572400 ms



}

bool test_mul_if_correct()
{
	bool result = true;
	const long matrixSize = 250;
	double x;

	VectorMatrix<int> test = get_test_vector_matrix(matrixSize);
	VectorMatrix<int> expected = VectorMatrix<int>(matrixSize, matrixSize);
	VectorMatrix<int> resultMat = VectorMatrix<int>(matrixSize, matrixSize);
	fill_matrix_with_value(expected, (int)matrixSize);

#pragma region BASIC



	basic_multiplication_1(test, test, resultMat, x);
	if (is_correct_result(resultMat, expected))
		printf("basic_multiplication_1: \t\tCORRECT\n");
	else
		printf("basic_multiplication_1: \t\tERROR\n");
	fill_matrix_with_value(resultMat, 0);

	basic_multiplication_2(test, test, resultMat, x);
	if (is_correct_result(resultMat, expected))
		printf("basic_multiplication_2: \t\tCORRECT\n");
	else
		printf("basic_multiplication_2: \t\tERROR\n");
	fill_matrix_with_value(resultMat, 0);

	basic_multiplication_3(test, test, resultMat, x);
	if (is_correct_result(resultMat, expected))
		printf("basic_multiplication_3: \t\tCORRECT\n");
	else
		printf("basic_multiplication_3: \t\tERROR\n");
	fill_matrix_with_value(resultMat, 0);

	basic_multiplication_4(test, test, resultMat, x);
	if (is_correct_result(resultMat, expected))
		printf("basic_multiplication_4: \t\tCORRECT\n");
	else
		printf("basic_multiplication_4: \t\tERROR\n");
	fill_matrix_with_value(resultMat, 0);

	basic_multiplication_5(test, test, resultMat, x);
	if (is_correct_result(resultMat, expected))
		printf("basic_multiplication_5: \t\tCORRECT\n");
	else
		printf("basic_multiplication_5: \t\tERROR\n");
	fill_matrix_with_value(resultMat, 0);

	basic_multiplication_6(test, test, resultMat, x);
	if (is_correct_result(resultMat, expected))
		printf("basic_multiplication_6: \t\tCORRECT\n");
	else
		printf("basic_multiplication_6: \t\tERROR\n");
	fill_matrix_with_value(resultMat, 0);

	basic_multiplication_1_with_tmp(test, test, resultMat, x);
	if (is_correct_result(resultMat, expected))
		printf("basic_multiplication_1_with_tmp: \tCORRECT\n");
	else
		printf("basic_multiplication_1_with_tmp: \tERROR\n");
	fill_matrix_with_value(resultMat, 0);

	basic_multiplication_3_with_tmp(test, test, resultMat, x);
	if (is_correct_result(resultMat, expected))
		printf("basic_multiplication_3_with_tmp: \tCORRECT\n");
	else
		printf("basic_multiplication_3_with_tmp: \tERROR\n");
	fill_matrix_with_value(resultMat, 0);

#pragma endregion

	// OPEN MP
	openmp_multiplication_1_1(test, test, resultMat, x, 12);
	if (is_correct_result(resultMat, expected))
		printf("openmp_multiplication_1_1: \t\tCORRECT\n");
	else
		printf("openmp_multiplication_1_1: \t\tERROR\n");
	fill_matrix_with_value(resultMat, 0);

	openmp_multiplication_1_2(test, test, resultMat, x, 12);
	if (is_correct_result(resultMat, expected))
		printf("openmp_multiplication_1_2: \t\tCORRECT\n");
	else
		printf("openmp_multiplication_1_2: \t\tERROR\n");
	fill_matrix_with_value(resultMat, 0);

	openmp_multiplication_1_3(test, test, resultMat, x, 12);
	if (is_correct_result(resultMat, expected))
		printf("openmp_multiplication_1_3: \t\tCORRECT\n");
	else
		printf("openmp_multiplication_1_3: \t\tERROR\n");
	fill_matrix_with_value(resultMat, 0);

	openmp_multiplication_1_with_tmp(test, test, resultMat, x, 12);
	if (is_correct_result(resultMat, expected))
		printf("openmp_multiplication_1_with_tmp: \tCORRECT\n");
	else
		printf("openmp_multiplication_1_with_tmp: \tERROR\n");
	fill_matrix_with_value(resultMat, 0);

	openmp_multiplication_1_atomic(test, test, resultMat, x, 12);
	if (is_correct_result(resultMat, expected))
		printf("openmp_multiplication_1_atomic: \tCORRECT\n");
	else
		printf("openmp_multiplication_1_atomic: \tERROR\n");
	fill_matrix_with_value(resultMat, 0);

	openmp_multiplication_2(test, test, resultMat, x, 12);
	if (is_correct_result(resultMat, expected))
		printf("openmp_multiplication_2: \t\tCORRECT\n");
	else
		printf("openmp_multiplication_2: \t\tERROR\n");
	fill_matrix_with_value(resultMat, 0);

	openmp_multiplication_2_atomic(test, test, resultMat, x, 12);
	if (is_correct_result(resultMat, expected))
		printf("openmp_multiplication_2_atomic: \tCORRECT\n");
	else
		printf("openmp_multiplication_2_atomic: \tERROR\n");
	fill_matrix_with_value(resultMat, 0);

	openmp_multiplication_3(test, test, resultMat, x, 12);
	if (is_correct_result(resultMat, expected))
		printf("openmp_multiplication_3: \t\tCORRECT\n");
	else
		printf("openmp_multiplication_3: \t\tERROR\n");
	fill_matrix_with_value(resultMat, 0);

	openmp_multiplication_4(test, test, resultMat, x, 12);
	if (is_correct_result(resultMat, expected))
		printf("openmp_multiplication_4: \t\tCORRECT\n");
	else
		printf("openmp_multiplication_4: \t\tERROR\n");
	fill_matrix_with_value(resultMat, 0);

	openmp_multiplication_5(test, test, resultMat, x, 12);
	if (is_correct_result(resultMat, expected))
		printf("openmp_multiplication_5: \t\tCORRECT\n");
	else
		printf("openmp_multiplication_5: \t\tERROR\n");
	fill_matrix_with_value(resultMat, 0);

	openmp_multiplication_6(test, test, resultMat, x, 12);
	if (is_correct_result(resultMat, expected))
		printf("openmp_multiplication_6: \t\tCORRECT\n");
	else
		printf("openmp_multiplication_6: \t\tERROR\n");
	fill_matrix_with_value(resultMat, 0);

	



	return result;
}

int main()
{

	if (!test_mul_if_correct())
		throw new std::exception("ERROR IN SOME MULTIPLICATION");





	//approx_pi();

	//const long matrixSize = 1000;
	//VectorMatrix<int> vmA(matrixSize, matrixSize);
	//VectorMatrix<int> vmB(matrixSize, matrixSize);
	//VectorMatrix<int> vmResult(vmA.rows(), vmB.cols());

	//printf("Filling matrices\n");
	//fill_matrix_with_random_int_values(vmA);
	//printf("Filled matrix A.\n");
	//fill_matrix_with_random_int_values(vmB);
	//printf("Filled matrix B.\n");
	//fill_matrix_with_value(vmResult, 0);
	//printf("Filled matrix RESULT.\n");

	//double vmTime = -1;
	//basic_multiplication_1(vmA, vmB, vmResult, vmTime);
	//printf("basic_multiplication_1;VectorMatrix;Time: %f ms.\n", vmTime);
	//openmp_multiplication_1(vmA, vmB, vmResult, vmTime, 12);
	//printf("openmp_multiplication_1;VectorMatrix;Time: %f ms.\n", vmTime);

	return 0;
}
