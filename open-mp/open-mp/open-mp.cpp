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


int main()
{
	//approx_pi();
	//Vector2DMatrix<int> x(10, 10);
	VectorMatrix<double> mat(10, 10);
	
	//fill_matrix_with_value(mat, 5);
	//fill_matrix_with_random_int_values(mat, (const int)0, (const int)1000);
	fill_matrix_with_random_double_values(mat, 2.0, 100.0);
	print_matrix(mat,"%f ");


	return 0;
}
