#include <vector>
#include <iostream>
#include <chrono>
#include "QuickSort.h"
#include "QuickSortOpenMP.h"

int main()
{
	// Тестування
	/*std::vector<int> test = {5, 3, 4, 1, 2, 3};
	
	QuickSort(test, 0, test.size() - 1);

	for (int i : test)
		std::cout << i;

	std::cout << std::endl;*/

	std::vector<int> array;
	int testSize = 600000;

	for (int i = 0; i < testSize; ++i)
		array.push_back(testSize - i);

	auto start = std::chrono::high_resolution_clock::now();

	//QuickSort(array, 0, array.size() - 1);
	QuickSortOpenMP(array, 8);

	// MPI



	auto end = std::chrono::high_resolution_clock::now();

	std::chrono::duration<double> duration = end - start;
	std::cout << "Час виконання: " << duration.count() << " секунд" << std::endl;

	return 0;
}