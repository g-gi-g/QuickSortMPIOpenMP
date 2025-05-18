#include "QuickSortOpenMP.h"
#include "QuickSort.h"

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
	while (chunks.size() > 1) // �������, ���� ����� ������ ������
	{
		std::vector<std::vector<int>> newChunks; // ������� ���� ���� ������

		#pragma omp parallel for schedule(dynamic) num_threads(numThreads) // schedule(dynamic) - omp ���� ���������� �������� ���� ������
		for (int i = 0; i < chunks.size() / 2; ++i) // ��������� ���� ��� ����� ���� ������
		{
			std::vector<int> merged;
			std::merge(chunks[2 * i].begin(), chunks[2 * i].end(), // �'������ ��� ���������� ������ � ���� ������������. std::merge ������� �������
				chunks[2 * i + 1].begin(), chunks[2 * i + 1].end(),
				std::back_inserter(merged));

			// ��� �� ���� ��������, ���� ������� ������ ��������� ������� � newChunks, ������������� �� ���������, ��� � ����� ������ ���� ����� ���� ����
			// �� ������ push_back
			#pragma omp critical 
			newChunks.push_back(std::move(merged));
		}

		if (chunks.size() % 2 == 1) // �������, ���� ������� ������ �������. ������ ���������� ������� � newChunks
			newChunks.push_back(std::move(chunks.back()));

		// ����� newChunks ��� chunks �� ���� ��������
		chunks = std::move(newChunks);
	}

	return chunks[0]; // ³����������� �����
}