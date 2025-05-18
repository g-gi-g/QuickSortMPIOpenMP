#include "QuickSortOpenMP.h"
#include "QuickSort.h"

std::vector<int> QuickSortOpenMP(std::vector<int>& array, int numThreads)
{
	int n = array.size();
	int chunkSize = n / numThreads;
	std::vector<std::vector<int>> chunks(numThreads);

	// Тут масив ділиться на підмасиви
	for (int i = 0; i < numThreads; ++i)
	{
		int start = i * chunkSize; 
		int end = (i == numThreads - 1) ? n : (i + 1) * chunkSize; // n-1 ?

		chunks[i] = std::vector<int>(array.begin() + start, array.begin() + end);
	}

	// Паралельно сортуємо кожну частину
	#pragma omp parallel for num_threads(numThreads)
	for (int i = 0; i < numThreads; ++i)
	{
		QuickSort(chunks[i], 0, chunks[i].size() - 1);
	}

	// Попарне злиття відсортованих частин
	while (chunks.size() > 1) // Зливаємо, поки більше одного шматку
	{
		std::vector<std::vector<int>> newChunks; // кладемо сюди злиті шматки

		#pragma omp parallel for schedule(dynamic) num_threads(numThreads) // schedule(dynamic) - omp може перекидати ітерації поміж потоків
		for (int i = 0; i < chunks.size() / 2; ++i) // запускаємо цикл для кожної пари шматків
		{
			std::vector<int> merged;
			std::merge(chunks[2 * i].begin(), chunks[2 * i].end(), // з'єднуємо два відсортовані шматки у один відсортований. std::merge гарантує порядок
				chunks[2 * i + 1].begin(), chunks[2 * i + 1].end(),
				std::back_inserter(merged));

			// щоб не було перегонів, коли декілька потоків одночасно додають у newChunks, використовуємо цю директиву, щоб в кожен момент часу тільки один потік
			// міг робити push_back
			#pragma omp critical 
			newChunks.push_back(std::move(merged));
		}

		if (chunks.size() % 2 == 1) // випадок, коли кількість шматків непарна. Просто переносимо останній у newChunks
			newChunks.push_back(std::move(chunks.back()));

		// тепер newChunks стає chunks та нова ітерація
		chunks = std::move(newChunks);
	}

	return chunks[0]; // Відсортований масив
}