#include "QuickSort.h"

std::vector<int> QuickSort(std::vector<int> array)
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

	std::vector<int> sortedLeft = QuickSort(left);
	std::vector<int> sortedRight = QuickSort(right);

	sortedLeft.push_back(pivot);
	sortedLeft.insert(sortedLeft.end(), sortedRight.begin(), sortedRight.end());
	return sortedLeft;
}