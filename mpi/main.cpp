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
constexpr int MatrixDim = 4;
constexpr int MatrixSize = MatrixDim * MatrixDim;

void print_coords(const int *arr, const int threadId)
{
    //printf("Thread %i: A[%i;%i];B[%i;%i];A->%i;B->%i\n", threadId, arr[0], arr[1], arr[2], arr[3], arr[4], arr[5]);
    //    printf("Thread %i:\tA->%i;B->%i PREV[%i;%i]\n", threadId, arr[4], arr[5]);
    printf("Thread %i:\tPREV[%i;%i]\n", threadId, arr[2], arr[3]);
}

inline uint get_index(const uint &row, const uint &col)
{
    return ((row * MatrixDim) + col);
}

template <typename T>
void print_matrix(const std::vector<T> &mat, const uint dim)
{

    for (uint row = 0; row < dim; row++)
    {
        for (uint col = 0; col < dim; col++)
        {
            printf("%i ", mat.at((row * col) + col));
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

int main(int argc, char **argv)
{
    int threadCount;
    int currentThreadId;

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

    if (currentThreadId == MasterThreadId)
    {
        // Initialization and work distribution in master thread.
        std::vector<int> A = {1, 2, 3, 4,
                              5, 8, 6, 1,
                              2, 4, 2, 0,
                              0, 0, 0, 1};
        std::vector<int> B = {3, 2, 1, 0,
                              5, 0, 6, 7,
                              0, 1, 0, 0,
                              2, 3, 1, 4};
        assert(A.size() == MatrixSize);
        assert(B.size() == MatrixSize);
        //Result should be
        /*
        21 17 17 30
        57 19 54 60
        26  6 26 28
         2  3  1  4
        */
        std::vector<int> C = generate_matrix(MatrixSize, 0);
        printf("Generated matrices, distribution work...\n");

        int matrix1DOffset;
        int aRow = 0;
        int aCol = 3;
        int bRow = 3;
        int bCol = 0;
        int dimCounter = 0;
        const int coordsSize = 6;
        int coords[coordsSize];
        int aReceiver = 5;
        int bReceiver = 2;
        int taskRow = 0;
        int tmp;
        int aindex, bindex;
        // Initialize workers.
        for (int threadId = 1; threadId <= MatrixSize; threadId++)
        {
            ++dimCounter;
            matrix1DOffset = threadId - 1;
            aReceiver = (threadId + MatrixDim);
            aReceiver = (aReceiver >= threadCount) ? (aReceiver % threadCount) + 1 : aReceiver;

            int currentMaxB = (taskRow * MatrixDim) + MatrixDim;

            bReceiver = (taskRow * MatrixDim) + dimCounter + 1;
            bReceiver = (dimCounter == MatrixDim) ? bReceiver - MatrixDim : bReceiver;

            coords[0] = aReceiver;

            tmp = aReceiver + 4;
            coords[1] = tmp >= threadCount ? (tmp % threadCount) + 1 : tmp;
            tmp = coords[1] + 4;
            coords[2] = tmp >= threadCount ? (tmp % threadCount) + 1 : tmp;

            coords[3] = bReceiver;
            tmp = bReceiver + 1;
            coords[4] = tmp > currentMaxB ? tmp - MatrixDim : tmp;
            tmp = bReceiver + 2;
            coords[5] = tmp > currentMaxB ? tmp - MatrixDim : tmp;

            //printf("Thread %i\tA[%i;%i]\tB[%i;%i]\n", threadId, aRow, aCol, bRow, bCol);
            //printf("%i to %i,%i,%i and %i,%i,%i\n", threadId, coords[0], coords[1], coords[2], coords[3], coords[4], coords[5]);

            aindex = get_index(aRow, aCol);
            bindex = get_index(bRow, bCol);
            printf("T%i: A[r:%i;c:%i],B[r:%i;c:%i],A index:%i,B index:%i\n", threadId, aRow, aCol, bRow, bCol, aindex, bindex);
            MPI_Send(&A[aindex], 1, MPI_INT, threadId, MasterThreadTag, MPI_COMM_WORLD);
            MPI_Send(&B[bindex], 1, MPI_INT, threadId, MasterThreadTag, MPI_COMM_WORLD);
            MPI_Send(&coords[0], coordsSize, MPI_INT, threadId, MasterThreadTag, MPI_COMM_WORLD);

            if (dimCounter == MatrixDim)
            {
                dimCounter = 0;

                aCol = (aCol + 3) % MatrixDim;
                bCol = (bCol + 1) % MatrixDim;
                bRow = (bRow + 3) % MatrixDim;
                ++taskRow;
            }

            aRow = (aRow + 1) % MatrixDim;
            aCol = (aCol + (MatrixDim - 1)) % MatrixDim;
            bRow = (bRow + (MatrixDim - 1)) % MatrixDim;
        }

        // Wait for workers.
        for (int threadId = 1; threadId <= MatrixSize; threadId++)
        {
            int c;
            MPI_Recv(&c, 1, MPI_INT, threadId, WorkerThreadTag, MPI_COMM_WORLD, &status);

            matrix1DOffset = threadId - 1;
            C[matrix1DOffset] = c;
        }
        printf("Received result from all threads.\nResult:\n");
        print_matrix(C, 4);
    }

    if (currentThreadId != MasterThreadId)
    {
        const bool print = true;
        const int printThread = 2;
        // Worker thread job.
        int a, b, c;
        const int coordsSize = 6;
        int coords[coordsSize];

        MPI_Recv(&a, 1, MPI_INT, MasterThreadId, MasterThreadTag, MPI_COMM_WORLD, &status);
        MPI_Recv(&b, 1, MPI_INT, MasterThreadId, MasterThreadTag, MPI_COMM_WORLD, &status);
        MPI_Recv(&coords[0], coordsSize, MPI_INT, MasterThreadId, MasterThreadTag, MPI_COMM_WORLD, &status);

        if (print && currentThreadId == printThread)
            printf("Thread %i; Initially received: a=%i b=%i\n", currentThreadId, a, b);
        c = a * b;

        MPI_Send(&a, 1, MPI_INT, coords[0], WorkerThreadTag, MPI_COMM_WORLD);
        MPI_Send(&a, 1, MPI_INT, coords[1], WorkerThreadTag, MPI_COMM_WORLD);
        MPI_Send(&a, 1, MPI_INT, coords[2], WorkerThreadTag, MPI_COMM_WORLD);

        MPI_Send(&b, 1, MPI_INT, coords[3], WorkerThreadTag, MPI_COMM_WORLD);
        MPI_Send(&b, 1, MPI_INT, coords[4], WorkerThreadTag, MPI_COMM_WORLD);
        MPI_Send(&b, 1, MPI_INT, coords[5], WorkerThreadTag, MPI_COMM_WORLD);

        int a1, a2, a3, b1, b2, b3;
        MPI_Recv(&a1, 1, MPI_INT, coords[2], WorkerThreadTag, MPI_COMM_WORLD, &status);
        MPI_Recv(&b1, 1, MPI_INT, coords[5], WorkerThreadTag, MPI_COMM_WORLD, &status);

        if (print && currentThreadId == printThread)
            printf("Thread %i; received: a1=%i b1=%i\n", currentThreadId, a1, b1);
        c += (a1 * b1);

        MPI_Recv(&a2, 1, MPI_INT, coords[1], WorkerThreadTag, MPI_COMM_WORLD, &status);
        MPI_Recv(&b2, 1, MPI_INT, coords[4], WorkerThreadTag, MPI_COMM_WORLD, &status);

        if (print && currentThreadId == printThread)
            printf("Thread %i; received: a2=%i b2=%i\n", currentThreadId, a2, b2);
        c += (a2 * b2);

        MPI_Recv(&a3, 1, MPI_INT, coords[0], WorkerThreadTag, MPI_COMM_WORLD, &status);
        MPI_Recv(&b3, 1, MPI_INT, coords[3], WorkerThreadTag, MPI_COMM_WORLD, &status);

        if (print && currentThreadId == printThread)
            printf("Thread %i; received: a3=%i b3=%i\n", currentThreadId, a3, b3);
        c += (a3 * b3);

        //printf("Thread %i, received all data.\n", currentThreadId);

        MPI_Send(&c, 1, MPI_INT, MasterThreadId, WorkerThreadTag, MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;
}