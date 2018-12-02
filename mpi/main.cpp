#include <iostream>
#include <vector>
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
    printf("Thread %i: A[%i;%i];B[%i;%i]\n", threadId, arr[0], arr[1], arr[2], arr[3]);
}

inline uint get_index(const uint &row, const uint &col)
{
    return ((row * col) + col);
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
        std::vector<int> A = generate_matrix(MatrixSize, 2);
        std::vector<int> B = generate_matrix(MatrixSize, 3);
        std::vector<int> C = generate_matrix(MatrixSize, 0);
        printf("Generated matrices, distribution work...\n");

        int matrix1DOffset;
        int Ax = 0;
        int Ay = 3;
        int Bx = 3;
        int By = 0;
        int dimCounter = 0;
        int coords[4];
        // Initialize workers.
        for (int threadId = 1; threadId < threadCount; threadId++)
        {
            //printf("A[%i;%i];B[%i;%i]\n", Ax, Ay, Bx, By);
            matrix1DOffset = threadId - 1;

            coords[0] = Ax;
            coords[1] = Ay;
            coords[2] = Bx;
            coords[3] = By;
            //MPI_Send(&coords, 4, MPI_INT, threadId, MasterThreadTag, MPI_COMM_WORLD);
            MPI_Send(&A[matrix1DOffset], 1, MPI_INT, threadId, MasterThreadTag, MPI_COMM_WORLD);
            MPI_Send(&B[matrix1DOffset], 1, MPI_INT, threadId, MasterThreadTag, MPI_COMM_WORLD);
            MPI_Send(&coords[0], 4, MPI_INT, threadId, MasterThreadTag, MPI_COMM_WORLD);

            if (++dimCounter == MatrixDim)
            {
                dimCounter = 0;
                Ay = (Ay + 3) % MatrixDim;
                By = (By + 1) % MatrixDim;

                Bx = (Bx + 3) % MatrixDim;
            }
            Ax = (Ax + 1) % MatrixDim;
            Ay = (Ay + (MatrixDim - 1)) % MatrixDim;
            Bx = (Bx + (MatrixDim - 1)) % MatrixDim;
        }

        // Wait for workers.
        for (int threadId = 1; threadId < threadCount; threadId++)
        {
            matrix1DOffset = threadId - 1;
            MPI_Recv(&C[matrix1DOffset], 1, MPI_INT, threadId, WorkerThreadTag, MPI_COMM_WORLD, &status);
        }
        printf("Received result from all threads.\nResult:\n");
        print_matrix(C, 4);
    }

    if (currentThreadId != MasterThreadId)
    {
        // Worker thread job.
        int a, b, c, ax;
        int coords[4];

        MPI_Recv(&a, 1, MPI_INT, MasterThreadId, MasterThreadTag, MPI_COMM_WORLD, &status);
        MPI_Recv(&b, 1, MPI_INT, MasterThreadId, MasterThreadTag, MPI_COMM_WORLD, &status);
        MPI_Recv(&coords[0], 4, MPI_INT, MasterThreadId, MasterThreadTag, MPI_COMM_WORLD, &status);
        print_coords(coords, currentThreadId);
        c = a * b;

        MPI_Send(&c, 1, MPI_INT, MasterThreadId, WorkerThreadTag, MPI_COMM_WORLD);

        // What to do now?
        // Move result down by Ay like  A03-->A02-->A01-->A00
        //                              A12-->A11-->A10-->A13
        // Move result right by Bx like B30-->B20-->B10-->B00
        //                              B21-->B11-->B01-->B31
        // Finish when THE first (marked somehow reveceive result from the last one)
        //  Or rather mark THE last one to MPI_Send result to master thread.
    }

    MPI_Finalize();
    return 0;
}