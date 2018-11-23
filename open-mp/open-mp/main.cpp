#include "pch.h"

inline bool is_in(float x, float y)
{
    float distance = sqrt(pow(x, 2) + pow(y, 2));
    return (distance <= 1);
}

void approx_pi(void)
{
    const int threadCount = 12;
    const int numberOfTries = 30000000;
    omp_set_num_threads(threadCount);
    int totalInCount = 0;

    StopwatchData s = {};
    start_stopwatch(s);

#pragma omp parallel
    {
        int threadInCount = 0;
        float x, y;

        std::random_device rd;
        std::mt19937 mt(rd());
        std::uniform_real_distribution<double> dist(0.0, 1.0);

        for (int i = 0; i < (numberOfTries / threadCount); i++)
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
    stop_stopwatch(s);
    printf("Approximed pi at: %f\n", approxPi);
    printf("Took: %f ms\n", elapsed_milliseconds(s));
}

double sum(const std::vector<double> &vec)
{
    double result = 0;

    for (int v : vec)
        result += v;

    return result;
}

bool test_mul_if_correct()
{
    bool result = true;
    const long matrixSize = 1000;
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

void basic_mul_benchmark(const BasicMatrix<int> &A, const BasicMatrix<int> &B, unsigned int sampleCount)
{
    std::vector<double> mul_1_times;
    std::vector<double> mul_1_tmp_times;
    std::vector<double> mul_2_times;
    std::vector<double> mul_3_times;
    std::vector<double> mul_3_tmp_times;
    std::vector<double> mul_4_times;
    std::vector<double> mul_5_times;
    std::vector<double> mul_6_times;
    mul_1_times.reserve(sampleCount);
    mul_1_tmp_times.reserve(sampleCount);
    mul_2_times.reserve(sampleCount);
    mul_3_times.reserve(sampleCount);
    mul_3_tmp_times.reserve(sampleCount);
    mul_4_times.reserve(sampleCount);
    mul_5_times.reserve(sampleCount);
    mul_6_times.reserve(sampleCount);

    ArrayMatrix<int> result = ArrayMatrix<int>(A.rows(), B.cols());
    double time;
    for (unsigned int tryId = 0; tryId < sampleCount; tryId++)
    {
        basic_multiplication_1(A, B, result, time);
        mul_1_times.push_back(time);
        fill_matrix_with_value(result, 0);

        basic_multiplication_1_with_tmp(A, B, result, time);
        mul_1_tmp_times.push_back(time);
        fill_matrix_with_value(result, 0);

        basic_multiplication_2(A, B, result, time);
        mul_2_times.push_back(time);
        fill_matrix_with_value(result, 0);

        basic_multiplication_3(A, B, result, time);
        mul_3_times.push_back(time);
        fill_matrix_with_value(result, 0);

        basic_multiplication_3_with_tmp(A, B, result, time);
        mul_3_tmp_times.push_back(time);
        fill_matrix_with_value(result, 0);

        basic_multiplication_4(A, B, result, time);
        mul_4_times.push_back(time);

        fill_matrix_with_value(result, 0);
        basic_multiplication_5(A, B, result, time);
        mul_5_times.push_back(time);
        fill_matrix_with_value(result, 0);

        basic_multiplication_6(A, B, result, time);
        mul_6_times.push_back(time);
        fill_matrix_with_value(result, 0);
        printf("Finished sample %i\n", tryId + 1);
    }

    double mul_1_avg_time = sum(mul_1_times) / (double)sampleCount;
    double mul_1_tmp_avg_time = sum(mul_1_tmp_times) / (double)sampleCount;
    double mul_2_avg_time = sum(mul_2_times) / (double)sampleCount;
    double mul_3_avg_time = sum(mul_3_times) / (double)sampleCount;
    double mul_3_tmp_avg_time = sum(mul_3_tmp_times) / (double)sampleCount;
    double mul_4_avg_time = sum(mul_4_times) / (double)sampleCount;
    double mul_5_avg_time = sum(mul_5_times) / (double)sampleCount;
    double mul_6_avg_time = sum(mul_6_times) / (double)sampleCount;

    printf("===========================================SINGLE-THREAD==================================\n");
    printf("[basic_multiplication_1], average time for %i samples: %f ms.\n", sampleCount, mul_1_avg_time);
    printf("[basic_mult_1_with_tmp],  average time for %i samples: %f ms.\n", sampleCount, mul_1_tmp_avg_time);
    printf("[basic_multiplication_2], average time for %i samples: %f ms.\n", sampleCount, mul_2_avg_time);
    printf("[basic_multiplication_3], average time for %i samples: %f ms.\n", sampleCount, mul_3_avg_time);
    printf("[basic_mult_3_with_tmp],  average time for %i samples: %f ms.\n", sampleCount, mul_3_tmp_avg_time);
    printf("[basic_multiplication_4], average time for %i samples: %f ms.\n", sampleCount, mul_4_avg_time);
    printf("[basic_multiplication_5], average time for %i samples: %f ms.\n", sampleCount, mul_5_avg_time);
    printf("[basic_multiplication_6], average time for %i samples: %f ms.\n", sampleCount, mul_6_avg_time);
    printf("==========================================================================================\n");
}

void openmp_mul_benchmark(const BasicMatrix<int> &A, const BasicMatrix<int> &B, unsigned int sampleCount, unsigned short threadCount)
{
    std::vector<double> mul_1_times;
    std::vector<double> mul_1_atomic_times;
    std::vector<double> mul_1_2_times;
    std::vector<double> mul_1_tmp_times;
    std::vector<double> mul_2_times;
    std::vector<double> mul_3_times;
    std::vector<double> mul_3_tmp_times;
    std::vector<double> mul_4_times;
    std::vector<double> mul_5_times;
    std::vector<double> mul_6_times;
    mul_1_times.reserve(sampleCount);
    mul_1_atomic_times.reserve(sampleCount);
    mul_1_2_times.reserve(sampleCount);
    mul_1_tmp_times.reserve(sampleCount);
    mul_2_times.reserve(sampleCount);
    mul_3_times.reserve(sampleCount);
    mul_3_tmp_times.reserve(sampleCount);
    mul_4_times.reserve(sampleCount);
    mul_5_times.reserve(sampleCount);
    mul_6_times.reserve(sampleCount);

    ArrayMatrix<int> result = ArrayMatrix<int>(A.rows(), B.cols());
    double time;
    for (unsigned int tryId = 0; tryId < sampleCount; tryId++)
    {
        openmp_multiplication_1_1(A, B, result, time, threadCount);
        mul_1_times.push_back(time);
        fill_matrix_with_value(result, 0);

        openmp_multiplication_1_atomic(A, B, result, time, threadCount);
        mul_1_atomic_times.push_back(time);
        fill_matrix_with_value(result, 0);

        openmp_multiplication_1_2(A, B, result, time, threadCount);
        mul_1_2_times.push_back(time);
        fill_matrix_with_value(result, 0);

        openmp_multiplication_1_with_tmp(A, B, result, time, threadCount);
        mul_1_tmp_times.push_back(time);
        fill_matrix_with_value(result, 0);

        openmp_multiplication_2(A, B, result, time, threadCount);
        mul_2_times.push_back(time);
        fill_matrix_with_value(result, 0);

        openmp_multiplication_3(A, B, result, time, threadCount);
        mul_3_times.push_back(time);
        fill_matrix_with_value(result, 0);

        openmp_multiplication_4(A, B, result, time, threadCount);
        mul_4_times.push_back(time);
        fill_matrix_with_value(result, 0);

        openmp_multiplication_5(A, B, result, time, threadCount);
        mul_5_times.push_back(time);
        fill_matrix_with_value(result, 0);

        openmp_multiplication_6(A, B, result, time, threadCount);
        mul_6_times.push_back(time);
        fill_matrix_with_value(result, 0);

        printf("Finished sample %i\n", tryId + 1);
    }

    double mul_1_avg_time = sum(mul_1_times) / (double)sampleCount;
    double mul_1_atomic_avg_time = sum(mul_1_atomic_times) / (double)sampleCount;
    double mul_1_2_avg_time = sum(mul_1_2_times) / (double)sampleCount;
    double mul_1_tmp_avg_time = sum(mul_1_tmp_times) / (double)sampleCount;
    double mul_2_avg_time = sum(mul_2_times) / (double)sampleCount;
    double mul_3_avg_time = sum(mul_3_times) / (double)sampleCount;
    double mul_4_avg_time = sum(mul_4_times) / (double)sampleCount;
    double mul_5_avg_time = sum(mul_5_times) / (double)sampleCount;
    double mul_6_avg_time = sum(mul_6_times) / (double)sampleCount;

    printf("========================OPEN-MP-%i-THREADS===============================\n", threadCount);
    printf("[openmp_multiplication_1], average time for %i samples: %f ms.\n", sampleCount, mul_1_avg_time);
    printf("[openmp_multipl_1_atomic], average time for %i samples: %f ms.\n", sampleCount, mul_1_atomic_avg_time);
    printf("[openmp_multiplicati_1_2], average time for %i samples: %f ms.\n", sampleCount, mul_1_2_avg_time);
    printf("[openmp_multiplica_1_tmp], average time for %i samples: %f ms.\n", sampleCount, mul_1_tmp_avg_time);
    printf("[openmp_multiplication_2], average time for %i samples: %f ms.\n", sampleCount, mul_2_avg_time);
    printf("[openmp_multiplication_3], average time for %i samples: %f ms.\n", sampleCount, mul_3_avg_time);
    printf("[openmp_multiplication_4], average time for %i samples: %f ms.\n", sampleCount, mul_4_avg_time);
    printf("[openmp_multiplication_5], average time for %i samples: %f ms.\n", sampleCount, mul_5_avg_time);
    printf("[openmp_multiplication_6], average time for %i samples: %f ms.\n", sampleCount, mul_6_avg_time);
    printf("===========================================================================\n");
}

int main(int, char **)
{
    //approx_pi();
    //return test_mul_if_correct() ? 0 : 1;
    const long matrixSize = 200;
    const int sampleCount = 10;

    ArrayMatrix<int> arrA = ArrayMatrix<int>(matrixSize, matrixSize);
    ArrayMatrix<int> arrB = ArrayMatrix<int>(matrixSize, matrixSize);

    fill_matrix_with_random_int_values(arrA);
    printf("Filled matrix A.\n");
    fill_matrix_with_random_int_values(arrB);
    printf("Filled matrix B.\n");

    printf("BENCHMARK for Array Matrices, dimensions %i x %i\n", (int)matrixSize, (int)matrixSize);
    printf("Starting single-threaded, no open-mp multiplication.\n");
    basic_mul_benchmark(arrA, arrB, sampleCount);
    for (unsigned short threadCount = 1; threadCount <= 12; threadCount++)
    {
        printf("Starting Open-MP benchmark for %i threads.\n", threadCount);
        openmp_mul_benchmark(arrA, arrB, sampleCount, threadCount);
    }

    {
        Array2DMatrix<int> arr2D_A = Array2DMatrix<int>(matrixSize, matrixSize);
        Array2DMatrix<int> arr2D_B = Array2DMatrix<int>(matrixSize, matrixSize);

        copy_matrix(arrA, arr2D_A);
        copy_matrix(arrB, arr2D_B);

        printf("BENCHMARK for Array2D Matrices, dimensions %i x %i\n", (int)matrixSize, (int)matrixSize);
        printf("Starting single-threaded, no open-mp multiplication.\n");
        basic_mul_benchmark(arr2D_A, arr2D_B, sampleCount);
        for (unsigned short threadCount = 1; threadCount <= 12; threadCount++)
        {
            printf("Starting Open-MP benchmark for %i threads.\n", threadCount);
            openmp_mul_benchmark(arr2D_A, arr2D_B, sampleCount, threadCount);
        }
    }

    {
        VectorMatrix<int> vecA = VectorMatrix<int>(matrixSize, matrixSize);
        VectorMatrix<int> vecB = VectorMatrix<int>(matrixSize, matrixSize);

        copy_matrix(arrA, vecA);
        copy_matrix(arrB, vecB);

        printf("BENCHMARK for Vector Matrices, dimensions %i x %i\n", (int)matrixSize, (int)matrixSize);
        printf("Starting single-threaded, no open-mp multiplication.\n");
        basic_mul_benchmark(vecA, vecB, sampleCount);
        for (unsigned short threadCount = 1; threadCount <= 12; threadCount++)
        {
            printf("Starting Open-MP benchmark for %i threads.\n", threadCount);
            openmp_mul_benchmark(vecA, vecB, sampleCount, threadCount);
        }
    }

    {
        Vector2DMatrix<int> vec2D_A = Vector2DMatrix<int>(matrixSize, matrixSize);
        Vector2DMatrix<int> vec2D_B = Vector2DMatrix<int>(matrixSize, matrixSize);
        copy_matrix(arrA, vec2D_A);
        copy_matrix(arrB, vec2D_B);

        printf("BENCHMARK for Vector2D Matrices, dimensions %i x %i\n", (int)matrixSize, (int)matrixSize);
        printf("Starting single-threaded, no open-mp multiplication.\n");
        basic_mul_benchmark(vec2D_A, vec2D_B, sampleCount);
        for (unsigned short threadCount = 1; threadCount <= 12; threadCount++)
        {
            printf("Starting Open-MP benchmark for %i threads.\n", threadCount);
            openmp_mul_benchmark(vec2D_A, vec2D_B, sampleCount, threadCount);
        }
    }

    return 0;
}