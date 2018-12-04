#include <iostream>
#include <vector>
#include <assert.h>
#include <math.h>
// Hardcoded include for vscode. #include <mpi.h> can be used.
#include "/usr/include/openmpi/mpi.h"

static std::vector<int> test_expected = {
    1380, 1416, 1452, 1488, 1524, 1560, 1596, 1632, 3236, 3336, 3436, 3536, 3636, 3736, 3836, 3936, 5092, 5256, 5420, 5584, 5748, 5912, 6076, 6240, 6948, 7176, 7404, 7632, 7860, 8088, 8316, 8544, 8804, 9096, 9388, 9680, 9972, 10264, 10556, 10848, 10660, 11016, 11372, 11728, 12084, 12440, 12796, 13152, 12516, 12936, 13356, 13776, 14196, 14616, 15036, 15456, 14372, 14856, 15340, 15824, 16308, 16792, 17276, 17760};

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

inline uint get_index(const uint &row, const uint &col, const uint partitionDim)
{
    return ((row * partitionDim) + col);
}

void check_result(const std::vector<int> &mat, const std::vector<int> &expected)
{
    for (size_t i = 0; i < mat.size(); i++)
    {
        if (mat[i] != expected[i])
        {
            printf("!!!MULTIPLICATION FAILED!!!\nExpected: %i, but got %i\n", expected[i], mat[i]);
            return;
        }
    }

    printf("MULTIPLICATION OK.\n");
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
                printf("!!!MULTIPLICATION FAILED!!!\nExpected: %i, but got %i\n", ExpectedResult, mat[get_index(row, col)]);
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
void print_vector(const std::vector<T> &vec)
{
    size_t vs = vec.size();
    for (size_t i = 0; i < vs; i++)
    {
        if (i == vs - 1)
            printf("%i \n", vec[i]);
        else
            printf("%i, ", vec[i]);
    }
}

template <typename T>
void print_matrix(const std::vector<T> &mat)
{
    uint dim = (uint)sqrt(mat.size());

    for (uint row = 0; row < dim; row++)
    {
        for (uint col = 0; col < dim; col++)
        {
            printf("%i\t", mat.at((row * MatrixDim) + col));
        }
        printf("\n");
    }
    printf("\n");
}

std::vector<int> generate_test_matrix(const uint size)
{
    std::vector<int> result;
    result.reserve(size);
    for (size_t i = 0; i < size; i++)
    {
        result.push_back((i + 1));
    }
    return result;
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

    uint actualFromRow = (fromRow * partitionDim);
    uint actualToRow = actualFromRow + partitionDim;

    uint actualFromCol = (fromCol * partitionDim);
    uint actualToCol = actualFromCol + partitionDim;

    result.reserve((partitionDim * partitionDim));
    uint index;

    for (uint row = actualFromRow; row < actualToRow; row++)
    {
        for (uint col = actualFromCol; col < actualToCol; col++)
        {
            index = get_index(row, col);
            result.push_back(mat[index]);
        }
    }
    return result;
}

template <typename T>
std::vector<std::vector<uint>> get_partition_indexes(const std::vector<T> &mat, const uint fromRow, const uint fromCol, const uint partitionDim)
{
    uint actualFromRow = (fromRow * partitionDim);
    uint actualToRow = actualFromRow + partitionDim;

    uint actualFromCol = (fromCol * partitionDim);
    uint actualToCol = actualFromCol + partitionDim;

    std::vector<std::vector<uint>> indexes;
    indexes.reserve(actualToRow - actualFromRow);

    for (uint row = actualFromRow; row < actualToRow; row++)
    {
        std::vector<uint> innerIndexes;
        innerIndexes.clear();
        innerIndexes.reserve(actualToCol - actualFromCol);
        for (uint col = actualFromCol; col < actualToCol; col++)
        {
            innerIndexes.push_back(get_index(row, col));
        }
        indexes.push_back(innerIndexes);
        //print_vector(innerIndexes);
    }
    //printf("\n\n");
    return indexes;
}

std::vector<uint> pack_indexes(const std::vector<std::vector<uint>> &indexes)
{
    std::vector<uint> packed;
    packed.reserve(indexes.size() * 2);
    uint first, last;
    for (const std::vector<uint> &innerIndexes : indexes)
    {
        first = innerIndexes[0];
        last = innerIndexes[innerIndexes.size() - 1];

        packed.push_back(first);
        packed.push_back(last);
    }
    return packed;
}

std::vector<std::vector<uint>> unpack_indexes(const std::vector<uint> &packed)
{
    assert((packed.size() % 2) == 0);
    std::vector<std::vector<uint>> unpacked;

    uint first, last;

    for (size_t i = 0; i < packed.size(); i += 2)
    {
        first = packed[i];
        last = packed[i + 1];

        std::vector<uint> inner;
        inner.reserve(last - first);

        for (uint index = first; index <= last; index++)
            inner.push_back(index);

        unpacked.push_back(inner);
    }

    return unpacked;
}

template <typename T>
void insert_to_matrix(std::vector<T> &C, const std::vector<T> &received, const uint fromRow, const uint fromCol, const uint partitionDim)
{
    uint actualFromRow = (fromRow * partitionDim);
    uint actualToRow = actualFromRow + partitionDim;

    uint actualFromCol = (fromCol * partitionDim);
    uint actualToCol = actualFromCol + partitionDim;

    int index = -1;
    for (uint row = actualFromRow; row < actualToRow; row++)
    {
        for (uint col = actualFromCol; col < actualToCol; col++)
        {
            C[get_index(row, col)] = received[++index];
        }
    }
}

int main(int argc, char **argv)
{
    int threadCount;
    int currentThreadId;

    MatrixDim = 8;
    ExpectedResult = MatrixDim * 6;
    const bool print = false;
    const int printThread = 1;

    // MPI initializations
    MPI_Status status;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &threadCount);
    MPI_Comm_rank(MPI_COMM_WORLD, &currentThreadId);

    int workerCount = threadCount - 1;
    uint partitionDim = (MatrixDim / (uint)sqrt(workerCount));
    uint partitionSize = (partitionDim * partitionDim);

    if (workerCount != 16)
    {
        // For testing we require 16 workers.
        printf("We require 16 worker threads for testing!\n");
        MPI_Abort(MPI_COMM_WORLD, 1);
    }
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
        std::vector<int> A = generate_test_matrix(get_matrix_size());
        std::vector<int> B = generate_test_matrix(get_matrix_size());
        // std::vector<int> A = generate_matrix(get_matrix_size(), 2);
        // std::vector<int> B = generate_matrix(get_matrix_size(), 3);
        // print_matrix(A);
        // print_matrix(B);
        std::vector<int> C = generate_matrix(get_matrix_size(), 0);

        assert(A.size() == get_matrix_size());
        assert(B.size() == get_matrix_size());
        assert(C.size() == get_matrix_size());

        printf("Generated matrices, distributing work...\n");

        int aRow = 0;
        int aCol = 3;
        int bRow = 3;
        int bCol = 0;
        int dimCounter = 0;
        const int jobInfoArraySize = 10;
        int jobInfo[jobInfoArraySize]; // 0 1 2 A threads, 3 4 5 B threads, 6 aRow, 7 aCol, 8 bRow, 9 bCol
        int aReceiver = 2;
        int bReceiver = 5;
        int taskRow = 0;
        int tmp;

        const int taskMeshDim = 4;

        // Initialize workers.
        for (int threadId = 1; threadId <= workerCount; threadId++)
        {
            aRow = taskRow;
            aCol = (taskMeshDim - 1) - dimCounter - taskRow;
            aCol = aCol >= 0 ? aCol : aCol + taskMeshDim;

            bRow = aCol;
            bCol = dimCounter;

            aReceiver = (taskRow * taskMeshDim) + dimCounter + 2;
            aReceiver = ((dimCounter + 1) == taskMeshDim) ? aReceiver - taskMeshDim : aReceiver;
            int currentMaxA = (taskRow * taskMeshDim) + taskMeshDim;

            bReceiver = (threadId + taskMeshDim);
            bReceiver = (bReceiver >= threadCount) ? (bReceiver % threadCount) + 1 : bReceiver;

            jobInfo[0] = aReceiver;
            tmp = aReceiver + 1;
            jobInfo[1] = tmp > currentMaxA ? tmp - taskMeshDim : tmp;
            tmp = aReceiver + 2;
            jobInfo[2] = tmp > currentMaxA ? tmp - taskMeshDim : tmp;

            jobInfo[3] = bReceiver;
            tmp = bReceiver + 4;
            jobInfo[4] = tmp >= threadCount ? (tmp % threadCount) + 1 : tmp;
            tmp = jobInfo[4] + 4;
            jobInfo[5] = tmp >= threadCount ? (tmp % threadCount) + 1 : tmp;

            if (++dimCounter == taskMeshDim)
            {
                dimCounter = 0;
                ++taskRow;
            }

            jobInfo[6] = aRow;
            jobInfo[7] = aCol;
            jobInfo[8] = bRow;
            jobInfo[9] = bCol;

            auto aPartition = get_partition(A, aRow, aCol, partitionDim);
            auto bPartition = get_partition(B, bRow, bCol, partitionDim);

            // printf("T%i: A[r:%i;c:%i],B[r:%i;c:%i]\n", threadId, aRow, aCol, bRow, bCol);
            // printf("T:%i;Partition A:\n", threadId);
            // print_vector(aPartition);
            // printf("T:%i;Partition B:\n", threadId);
            // print_vector(bPartition);

            // auto aPart = pack_indexes(get_partition_indexes(A, aRow, aCol, partitionDim));
            // auto bPart = pack_indexes(get_partition_indexes(B, bRow, bCol, partitionDim));

            // int aps = aPart.size();
            // int bps = bPart.size();

            // MPI_Send(&aps, 1, MPI_INT, threadId, MasterThreadTag, MPI_COMM_WORLD);
            // MPI_Send(&bps, 1, MPI_INT, threadId, MasterThreadTag, MPI_COMM_WORLD);

            // MPI_Send(&A[0], 1, MPI_INT, threadId, MasterThreadTag, MPI_COMM_WORLD);
            // MPI_Send(&B[0], 1, MPI_INT, threadId, MasterThreadTag, MPI_COMM_WORLD);

            // MPI_Send(&aPart[0], aPart.size(), MPI_INT, threadId, MasterThreadTag, MPI_COMM_WORLD);
            // MPI_Send(&aPart[0], aPart.size(), MPI_INT, threadId, MasterThreadTag, MPI_COMM_WORLD);

            MPI_Send(&aPartition[0], partitionSize, MPI_INT, threadId, MasterThreadTag, MPI_COMM_WORLD);
            MPI_Send(&bPartition[0], partitionSize, MPI_INT, threadId, MasterThreadTag, MPI_COMM_WORLD);
            MPI_Send(&jobInfo[0], jobInfoArraySize, MPI_INT, threadId, MasterThreadTag, MPI_COMM_WORLD);
        }

        // Wait for workers.
        for (int threadId = 1; threadId <= workerCount; threadId++)
        {
            std::vector<int> received;
            int info[2]; // 1 fromRow, 2 fromCol
            received.clear();
            received.resize(partitionSize);

            MPI_Recv(&received[0], partitionSize, MPI_INT, threadId, WorkerThreadTag, MPI_COMM_WORLD, &status);
            MPI_Recv(&info[0], 2, MPI_INT, threadId, WorkerThreadTag, MPI_COMM_WORLD, &status);

            insert_to_matrix(C, received, info[0], info[1], partitionDim);
        }
        printf("Received result from all threads.\nResult:\n");

        check_result(C, test_expected);
        //check_result(C);
        print_matrix(C);
    }

    if (currentThreadId != MasterThreadId)
    {
        // Worker thread job.
        const int jobInfoArraySize = 10;
        int jobInfo[jobInfoArraySize];
        uint aRow, aCol, bRow, bCol;

        std::vector<int> A;
        std::vector<int> B;
        std::vector<int> C;
        A.resize(partitionSize);
        B.resize(partitionSize);
        C.resize(partitionSize);

        // std::vector<uint> aIndexes;
        // std::vector<uint> bIndexes;

        // int aPointer, bPointer, aIndSize, bIndSize;

        // MPI_Recv(&aIndSize, 1, MPI_INT, MasterThreadId, MasterThreadTag, MPI_COMM_WORLD, &status);
        // MPI_Recv(&bIndSize, 1, MPI_INT, MasterThreadId, MasterThreadTag, MPI_COMM_WORLD, &status);

        // MPI_Recv(&aPointer, 1, MPI_INT, MasterThreadId, MasterThreadTag, MPI_COMM_WORLD, &status);
        // MPI_Recv(&bPointer, 1, MPI_INT, MasterThreadId, MasterThreadTag, MPI_COMM_WORLD, &status);

        // MPI_Recv(&bIndexes[0], aIndSize, MPI_INT, MasterThreadId, MasterThreadTag, MPI_COMM_WORLD, &status);
        // MPI_Recv(&bIndexes[0], bIndSize, MPI_INT, MasterThreadId, MasterThreadTag, MPI_COMM_WORLD, &status);

        // std::vector<std::vector<uint>> unpackedAIndexes = unpack_indexes(aIndexes);
        // std::vector<std::vector<uint>> unpackedBIndexes = unpack_indexes(bIndexes);

        //int value = *reinterpret_cast<int *>(aPointer + aIndexes[0]);

        MPI_Recv(&A[0], partitionSize, MPI_INT, MasterThreadId, MasterThreadTag, MPI_COMM_WORLD, &status);
        MPI_Recv(&B[0], partitionSize, MPI_INT, MasterThreadId, MasterThreadTag, MPI_COMM_WORLD, &status);
        MPI_Recv(&jobInfo[0], jobInfoArraySize, MPI_INT, MasterThreadId, MasterThreadTag, MPI_COMM_WORLD, &status);

        aRow = jobInfo[6];
        aCol = jobInfo[7];
        bRow = jobInfo[8];
        bCol = jobInfo[9];

        if (print && currentThreadId == printThread)
        {
            printf("Thread %i; Initially received:\n", currentThreadId);

            print_vector(A);
            print_vector(B);
        }

        for (uint resRow = 0; resRow < partitionDim; resRow++)
            for (uint resCol = 0; resCol < partitionDim; resCol++)
                for (uint aCol = 0; aCol < partitionDim; aCol++)
                    C[get_index(resRow, resCol, partitionDim)] += (A[get_index(resRow, aCol, partitionDim)] * B[get_index(aCol, resCol, partitionDim)]);

        if (print && currentThreadId == printThread)
        {
            printf("After first ff:\n");
            print_vector(C);
        }

        MPI_Send(&A[0], partitionSize, MPI_INT, jobInfo[0], WorkerThreadTag, MPI_COMM_WORLD);
        MPI_Send(&A[0], partitionSize, MPI_INT, jobInfo[1], WorkerThreadTag, MPI_COMM_WORLD);
        MPI_Send(&A[0], partitionSize, MPI_INT, jobInfo[2], WorkerThreadTag, MPI_COMM_WORLD);

        MPI_Send(&B[0], partitionSize, MPI_INT, jobInfo[3], WorkerThreadTag, MPI_COMM_WORLD);
        MPI_Send(&B[0], partitionSize, MPI_INT, jobInfo[4], WorkerThreadTag, MPI_COMM_WORLD);
        MPI_Send(&B[0], partitionSize, MPI_INT, jobInfo[5], WorkerThreadTag, MPI_COMM_WORLD);

        MPI_Recv(&A[0], partitionSize, MPI_INT, jobInfo[0], WorkerThreadTag, MPI_COMM_WORLD, &status);
        MPI_Recv(&B[0], partitionSize, MPI_INT, jobInfo[3], WorkerThreadTag, MPI_COMM_WORLD, &status);

        if (print && currentThreadId == printThread)
        {
            printf("Thread %i; received 1:\n", currentThreadId);

            print_vector(A);
            print_vector(B);
        }

        for (uint resRow = 0; resRow < partitionDim; resRow++)
            for (uint resCol = 0; resCol < partitionDim; resCol++)
                for (uint aCol = 0; aCol < partitionDim; aCol++)
                    C[get_index(resRow, resCol, partitionDim)] += (A[get_index(resRow, aCol, partitionDim)] * B[get_index(aCol, resCol, partitionDim)]);

        MPI_Recv(&A[0], partitionSize, MPI_INT, jobInfo[1], WorkerThreadTag, MPI_COMM_WORLD, &status);
        MPI_Recv(&B[0], partitionSize, MPI_INT, jobInfo[4], WorkerThreadTag, MPI_COMM_WORLD, &status);

        if (print && currentThreadId == printThread)
        {
            printf("Thread %i; received 2:\n", currentThreadId);

            print_vector(A);
            print_vector(B);
        }

        for (uint resRow = 0; resRow < partitionDim; resRow++)
            for (uint resCol = 0; resCol < partitionDim; resCol++)
                for (uint aCol = 0; aCol < partitionDim; aCol++)
                    C[get_index(resRow, resCol, partitionDim)] += (A[get_index(resRow, aCol, partitionDim)] * B[get_index(aCol, resCol, partitionDim)]);

        MPI_Recv(&A[0], partitionSize, MPI_INT, jobInfo[2], WorkerThreadTag, MPI_COMM_WORLD, &status);
        MPI_Recv(&B[0], partitionSize, MPI_INT, jobInfo[5], WorkerThreadTag, MPI_COMM_WORLD, &status);

        if (print && currentThreadId == printThread)
        {
            printf("Thread %i; received 3:\n", currentThreadId);

            print_vector(A);
            print_vector(B);
        }

        for (uint resRow = 0; resRow < partitionDim; resRow++)
            for (uint resCol = 0; resCol < partitionDim; resCol++)
                for (uint aCol = 0; aCol < partitionDim; aCol++)
                    C[get_index(resRow, resCol, partitionDim)] += (A[get_index(resRow, aCol, partitionDim)] * B[get_index(aCol, resCol, partitionDim)]);

        int info[2];
        info[0] = aRow;
        info[1] = bCol;

        if (print && currentThreadId == printThread)
        {
            printf("Returning from T:%i, Should insert at: %i;%i\n", currentThreadId, aRow, bCol);
            print_vector(C);
        }
        MPI_Send(&C[0], partitionSize, MPI_INT, MasterThreadId, WorkerThreadTag, MPI_COMM_WORLD);
        MPI_Send(&info[0], 2, MPI_INT, MasterThreadId, WorkerThreadTag, MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;
}