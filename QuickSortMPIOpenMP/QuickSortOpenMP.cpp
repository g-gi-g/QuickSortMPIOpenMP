#include "QuickSortOpenMP.h"
#include "QuickSort.h"

/*std::vector<int> QuickSortTasksOpenMP(std::vector<int> array)
{
	if (array.size() <= 1)
		return array;

	int pivot = array[array.size() - 1];
	std::vector<int> left, right;
	for (int i = 0; i < array.size() - 1; ++i)
	{
		if (array[i] < pivot)
			left.push_back(array[i]);
		else if (array[i] >= pivot)
			right.push_back(array[i]);
	}

	std::vector<int> sortedLeft, sortedRight;

	if (array.size() >= PARALLEL_THRESHOLD)
	{
		#pragma omp task shared(sortedLeft)
			sortedLeft = QuickSortTasksOpenMP(left);
		#pragma omp task shared(sortedRight)
			sortedRight = QuickSortTasksOpenMP(right);
	}
	else
	{
		sortedLeft = QuickSort(left);
		sortedRight = QuickSort(right);
	}

	sortedLeft.push_back(pivot);
	sortedLeft.insert(sortedLeft.end(), sortedRight.begin(), sortedRight.end());
	return sortedLeft;
}

std::vector<int> QuickSortOpenMP(std::vector<int> array)
{
	std::vector<int> result;

	#pragma omp parallel
	{
		#pragma omp single
			result = QuickSortTasksOpenMP(array);
		#pragma omp taskwait
	}

	return result;
}

//std::vector<int> QuickSortOpenMP(std::vector<int> array)
//{
//	if (array.size() <= 1)
//		return array;
//
//	int pivot = array[array.size() - 1];
//	std::vector<int> left, right;
//	for (int i = 0; i < array.size() - 1; ++i)
//	{
//		if (array[i] < pivot)
//			left.push_back(array[i]);
//		else if (array[i] >= pivot)
//			right.push_back(array[i]);
//	}
//
//	std::vector<int> sortedLeft, sortedRight;
//
//	if (array.size() > PARALLEL_THRESHOLD)
//	{
//		// ������� ��� ������
//		#pragma omp parallel
//		{
//			// ҳ���� ���� ���� � ���� �� �������� ��� ����, ����� ����� ���� ���� �������� ������ 
//			// (nowait ��� ���� ������ �� ������ �� ���������� ����� single �����)
//			#pragma omp single nowait
//			{
//				// ������� ������, ��� ���� �������� ����-���� ���� � ���� 
//				// shared(sortedLeft) �����, �� ����� sortedLeft ������ ��� ��� ������ 
//				// �� ���� ���� ������ � ������� ������
//				#pragma omp task shared(sortedLeft)
//				{
//					sortedLeft = QuickSortOpenMP(left);
//				}
//
//				// ��� ����
//				#pragma omp task shared(sortedRight)
//				{
//					sortedRight = QuickSortOpenMP(right);
//				}
//
//				// ������, ���� �� ������� ������ � ��� ������ �����������
//				// �������, �� ��� ����� ������ �� ������� ����� ���� ����������
//				#pragma omp taskwait
//			}
//		}
//	}
//	else
//	{
//		sortedLeft = QuickSortOpenMP(left);
//		sortedRight = QuickSortOpenMP(right);
//	}
//
//	sortedLeft.push_back(pivot);
//	sortedLeft.insert(sortedLeft.end(), sortedRight.begin(), sortedRight.end());
//	return sortedLeft;
//}*/

std::vector<int> QuickSortOpenMP(std::vector<int>& array, int numThreads)
{
	int n = array.size();
	int chunkSize = n / numThreads;
	std::vector<std::vector<int>> chunks(numThreads);

	// ��� ����� ������� �� ��������
	for (int i = 0; i < numThreads; ++i)
	{
		int start = i * chunkSize; 
		int end = (i == numThreads - 1) ? n : (i + 1) * chunkSize; // n-1 ?

		chunks[i] = std::vector<int>(array.begin() + start, array.begin() + end);
	}

	// ���������� ������� ����� �������
	#pragma omp parallel for num_threads(numThreads)
	for (int i = 0; i < numThreads; ++i)
	{
		QuickSort(chunks[i], 0, chunks[i].size() - 1);
	}

	// ������� ������ ������������ ������
	while (chunks.size() > 1)
	{
		std::vector<std::vector<int>> newChunks;

		#pragma omp parallel for schedule(dynamic) num_threads(numThreads)
		for (int i = 0; i < chunks.size() / 2; ++i)
		{
			std::vector<int> merged;
			std::merge(chunks[2 * i].begin(), chunks[2 * i].end(),
				chunks[2 * i + 1].begin(), chunks[2 * i + 1].end(),
				std::back_inserter(merged));

			#pragma omp critical
			newChunks.push_back(std::move(merged));
		}

		if (chunks.size() % 2 == 1)
			newChunks.push_back(std::move(chunks.back()));

		chunks = std::move(newChunks);
	}

	return chunks[0]; // ³����������� �����
}