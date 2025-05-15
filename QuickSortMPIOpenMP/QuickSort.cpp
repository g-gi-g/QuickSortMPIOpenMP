#include "QuickSort.h"

/*std::vector<int> QuickSort(std::vector<int> array)
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
}*/

void QuickSort(std::vector<int>& array, int left, int right)
{
    if (left >= right)
        return;

    int pivot = array[right]; 
    int i = left - 1;

    for (int j = left; j < right; ++j)
    {
        if (array[j] < pivot)
        {
            ++i;
            std::swap(array[i], array[j]);
        }
    }

    std::swap(array[i + 1], array[right]);
    int pivotIndex = i + 1;

    QuickSort(array, left, pivotIndex - 1);
    QuickSort(array, pivotIndex + 1, right);
}