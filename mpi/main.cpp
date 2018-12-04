#include <iostream>
#include <vector>
#include <assert.h>
// Hardcoded include for vscode. #include <mpi.h> can be used.
#include "/usr/include/openmpi/mpi.h"

// Test case    A matrix all values = 2
//              B matrix all values = 3
//              Result matrix all values = 24

// Defines
constexpr int MasterThreadId = 0;
constexpr int MasterThreadTag = 1;
constexpr int WorkerThreadTag = 2;
static uint MatrixDim = 4;
static uint ExpectedResult = 4;

inline uint get_index(const uint &row, const uint &col)
{
    uint result = ((row * MatrixDim) + col);
    return result;
}

void check_result(const std::vector<int> &mat)
{
    bool failed = false;
    for (uint row = 0; row < MatrixDim; row++)
    {
        for (uint col = 0; col < MatrixDim; col++)
        {
            if (mat[get_index(row, col)] != ExpectedResult)
            {
                printf("!!!MULTIPLICATION FAILED!!!\n");
                return;
            }
        }
    }
    printf("MULTIPLICATION OK.\n");
}

inline uint get_matrix_size()
{
    return (MatrixDim * MatrixDim);
}

template <typename T>
void print_matrix(const std::vector<T> &mat, const uint dim)
{

    for (uint row = 0; row < dim; row++)
    {
        for (uint col = 0; col < dim; col++)
        {
            printf("%i ", mat.at((row * MatrixDim) + col));
        }
        printf("\n");
    }
}

template <typename T>
std::vector<T> generate_matrix(const uint size, const T value)
{
    std::vector<T> result;
    result.reserve(size);
    for (size_t i = 0; i < size; i++)
    {
        result.push_back(value);
    }
    return result;
}

template <typename T>
std::vector<T> get_partition(const std::vector<T> &mat, const uint fromRow, const uint fromCol, const uint partitionDim)
{
    std::vector<T> result;

    result.reserve((partitionDim * partitionDim));
    for (uint row = fromRow; row < (fromRow + partitionDim); row++)
    {
        for (uint col = fromCol; col < (fromCol + partitionDim); col++)
        {
            result.push_back(mat[get_index(row, col)]);
        }
    }
    return result;
}

int main(int argc, char **argv)
{
    int threadCount;
    int currentThreadId;

    MatrixDim = 4;
    ExpectedResult = 24;

    // MPI initializations
    MPI_Status status;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &threadCount);
    MPI_Comm_rank(MPI_COMM_WORLD, &currentThreadId);

    if (threadCount != 17)
    {
        // For testing we require 16 workers.
        printf("We require 17 threads for testing!\n");
        MPI_Abort(MPI_COMM_WORLD, 1);
    }
    const bool print = true;
    const int printThread = 5;
    if (currentThreadId == MasterThreadId)
    {
        /*
        std::vector<int> A = {1, 2, 3, 4,
                              5, 8, 6, 1,
                              2, 4, 2, 0,
                              0, 0, 0, 1};
        std::vector<int> B = {3, 2, 1, 0,
                              5, 0, 6, 7,
                              0, 1, 0, 0,
                              2, 3, 1, 4};
        */
        /*
        21 17 17 30
        57 19 54 60
        26  6 26 28
         2  3  1  4
        */

        // Initialization and work distribution in master thread.
        std::vector<int> A = generate_matrix(get_matrix_size(), 2);
        std::vector<int> B = generate_matrix(get_matrix_size(), 3);
        std::vector<int> C = generate_matrix(get_matrix_size(), 0);

        assert(A.size() == get_matrix_size());
        assert(B.size() == get_matrix_size());
        assert(C.size() == get_matrix_size());

        printf("Generated matrices, distributing work...\n");

        int matrix1DOffset;
        int aRow = 0;
        int aCol = 3;
        int bRow = 3;
        int bCol = 0;
        int dimCounter = 0;
        const int jobInfoArraySize = 6;
        int jobInfo[jobInfoArraySize];
        int aReceiver = 2;
        int bReceiver = 5;
        int taskRow = 0;
        int tmp;
        int aindex, bindex;

        // Initialize workers.
        for (int threadId = 1; threadId <= get_matrix_size(); threadId++)
        {
            matrix1DOffset = threadId - 1;

            aRow = taskRow;
            aCol = (MatrixDim - 1) - dimCounter - taskRow;
            aCol = aCol >= 0 ? aCol : aCol + MatrixDim;

            bRow = aCol;
            bCol = dimCounter;

            aReceiver = (taskRow * MatrixDim) + dimCounter + 2;
            aReceiver = ((dimCounter + 1) == MatrixDim) ? aReceiver - MatrixDim : aReceiver;
            int currentMaxA = (taskRow * MatrixDim) + MatrixDim;

            bReceiver = (threadId + MatrixDim);
            bReceiver = (bReceiver >= threadCount) ? (bReceiver % threadCount) + 1 : bReceiver;

            jobInfo[0] = aReceiver;
            tmp = aReceiver + 1;
            jobInfo[1] = tmp > currentMaxA ? tmp - MatrixDim : tmp;
            tmp = aReceiver + 2;
            jobInfo[2] = tmp > currentMaxA ? tmp - MatrixDim : tmp;

            jobInfo[3] = bReceiver;
            tmp = bReceiver + 4;
            jobInfo[4] = tmp >= threadCount ? (tmp % threadCount) + 1 : tmp;
            tmp = jobInfo[4] + 4;
            jobInfo[5] = tmp >= threadCount ? (tmp % threadCount) + 1 : tmp;

            if (++dimCounter == MatrixDim)
            {
                dimCounter = 0;
                ++taskRow;
            }

            aindex = get_index(aRow, aCol);
            bindex = get_index(bRow, bCol);

            // if (print && threadId == printThread)
            // {
            //     printf("%i to %i,%i,%i and %i,%i,%i\n", threadId, jobInfo[0], jobInfo[1], jobInfo[2], jobInfo[3], jobInfo[4], jobInfo[5]);
            //     printf("T%i: A[r:%i;c:%i],B[r:%i;c:%i],A index:%i,B index:%i\n", threadId, aRow, aCol, bRow, bCol, aindex, bindex);
            // }

            MPI_Send(&A[aindex], 1, MPI_INT, threadId, MasterThreadTag, MPI_COMM_WORLD);
            MPI_Send(&B[bindex], 1, MPI_INT, threadId, MasterThreadTag, MPI_COMM_WORLD);
            MPI_Send(&jobInfo[0], jobInfoArraySize, MPI_INT, threadId, MasterThreadTag, MPI_COMM_WORLD);
        }

        // Wait for workers.
        for (int threadId = 1; threadId <= get_matrix_size(); threadId++)
        {
            int c;
            MPI_Recv(&c, 1, MPI_INT, threadId, WorkerThreadTag, MPI_COMM_WORLD, &status);

            matrix1DOffset = threadId - 1;
            if (c == 57)
            {
                printf("Receiver 57 from thread %i.Saving at index %i\n", threadId, matrix1DOffset);
            }
            C.at(matrix1DOffset) = c;
        }
        printf("Received result from all threads.\nResult:\n");
        check_result(C);
        print_matrix(C, 4);
    }

    if (currentThreadId != MasterThreadId)
    {

        // Worker thread job.
        int a, b, c;
        c = 0;
        const int jobInfoArraySize = 6;
        int jobInfo[jobInfoArraySize];

        MPI_Recv(&a, 1, MPI_INT, MasterThreadId, MasterThreadTag, MPI_COMM_WORLD, &status);
        MPI_Recv(&b, 1, MPI_INT, MasterThreadId, MasterThreadTag, MPI_COMM_WORLD, &status);
        MPI_Recv(&jobInfo[0], jobInfoArraySize, MPI_INT, MasterThreadId, MasterThreadTag, MPI_COMM_WORLD, &status);

        if (print && currentThreadId == printThread)
        {
            printf("Thread %i; Initially received: a=%i b=%i, from master.\n", currentThreadId, a, b);
            printf("c = %i\n", c);
        }
        c = a * b;
        if (print && currentThreadId == printThread)
            printf("c = %i\n", c);

        MPI_Send(&a, 1, MPI_INT, jobInfo[0], WorkerThreadTag, MPI_COMM_WORLD);
        MPI_Send(&a, 1, MPI_INT, jobInfo[1], WorkerThreadTag, MPI_COMM_WORLD);
        MPI_Send(&a, 1, MPI_INT, jobInfo[2], WorkerThreadTag, MPI_COMM_WORLD);

        MPI_Send(&b, 1, MPI_INT, jobInfo[3], WorkerThreadTag, MPI_COMM_WORLD);
        MPI_Send(&b, 1, MPI_INT, jobInfo[4], WorkerThreadTag, MPI_COMM_WORLD);
        MPI_Send(&b, 1, MPI_INT, jobInfo[5], WorkerThreadTag, MPI_COMM_WORLD);

        MPI_Recv(&a, 1, MPI_INT, jobInfo[0], WorkerThreadTag, MPI_COMM_WORLD, &status);
        MPI_Recv(&b, 1, MPI_INT, jobInfo[3], WorkerThreadTag, MPI_COMM_WORLD, &status);

        if (print && currentThreadId == printThread)
        {
            printf("Thread %i; received: a1=%i b1=%i, from %i and %i\n", currentThreadId, a, b, jobInfo[5], jobInfo[2]);
            printf("c = %i\n", c);
        }
        c += (a * b);
        if (print && currentThreadId == printThread)
            printf("c = %i\n", c);

        MPI_Recv(&a, 1, MPI_INT, jobInfo[1], WorkerThreadTag, MPI_COMM_WORLD, &status);
        MPI_Recv(&b, 1, MPI_INT, jobInfo[4], WorkerThreadTag, MPI_COMM_WORLD, &status);

        if (print && currentThreadId == printThread)
            printf("Thread %i; received: a2=%i b2=%i, from %i and %i\n", currentThreadId, a, b, jobInfo[4], jobInfo[1]);
        c += (a * b);

        MPI_Recv(&a, 1, MPI_INT, jobInfo[2], WorkerThreadTag, MPI_COMM_WORLD, &status);
        MPI_Recv(&b, 1, MPI_INT, jobInfo[5], WorkerThreadTag, MPI_COMM_WORLD, &status);

        if (print && currentThreadId == printThread)
        {
            printf("Thread %i; received: a3=%i b3=%i, from %i and %i\n", currentThreadId, a, b, jobInfo[3], jobInfo[0]);
            printf("c = %i\n", c);
        }
        c += (a * b);
        if (print && currentThreadId == printThread)
            printf("c = %i\n", c);

        //printf("Thread %i, received all data.\n", currentThreadId);

        MPI_Send(&c, 1, MPI_INT, MasterThreadId, WorkerThreadTag, MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;
}