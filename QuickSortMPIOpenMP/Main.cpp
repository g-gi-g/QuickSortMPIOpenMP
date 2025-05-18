#include <vector>
#include <iostream>
#include <chrono>
#include "QuickSort.h"
#include "QuickSortOpenMP.h"
#include <mpi.h>

//int main(int argc, char** argv)
//{
//	std::vector<int> array;
//	int testSize = 600000;
//
//	for (int i = 0; i < testSize; ++i)
//		array.push_back(testSize - i);
//
//	auto start = std::chrono::high_resolution_clock::now();
//
//	//QuickSort(array, 0, array.size() - 1);
//	//QuickSortOpenMP(array, 8);
//
//	auto end = std::chrono::high_resolution_clock::now();
//
//	std::chrono::duration<double> duration = end - start;
//	std::cout << "Час виконання: " << duration.count() << " секунд" << std::endl;
//
//	return 0;
//}

// MPI

int main(int argc, char** argv)
{
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    const int testSize = 600000;
    std::vector<int> fullArray;

    // тільки rank 0 створює масив
    if (rank == 0) {
        fullArray.resize(testSize);
        for (int i = 0; i < testSize; ++i)
            fullArray[i] = testSize - i;
    }

    int chunkSize = testSize / size;

    std::vector<int> localArray(chunkSize);

    auto start = std::chrono::high_resolution_clock::now();

    // Розсилка по шматкам
    MPI_Scatter(fullArray.data(), chunkSize, MPI_INT,
        localArray.data(), chunkSize, MPI_INT,
        0, MPI_COMM_WORLD);

    // сортуємо локально
    QuickSort(localArray, 0, chunkSize - 1);

    // Збираємо результати
    MPI_Gather(localArray.data(), chunkSize, MPI_INT,
        fullArray.data(), chunkSize, MPI_INT,
        0, MPI_COMM_WORLD);

    if (rank == 0) 
    {
        // Зливаємо відсортовані шматки
        std::vector<int> result = fullArray;

        int mergeSize = chunkSize;
        while (mergeSize < testSize) {
            for (int i = 0; i + mergeSize < testSize; i += 2 * mergeSize) {
                std::inplace_merge(result.begin() + i,
                    result.begin() + i + mergeSize,
                    result.begin() + std::min(i + 2 * mergeSize, testSize));
            }
            mergeSize *= 2;
        }

        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duration = end - start;
        std::cout << "Час виконання: " << duration.count() << " секунд" << std::endl;
    }

    MPI_Finalize();
    return 0;
}