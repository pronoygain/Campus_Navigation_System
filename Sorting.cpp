#include "Sorting.h"

#include <string>

using namespace std;

static void merge(
    Location arr[],
    int left,
    int mid,
    int right
)
{
    int n1 = mid - left + 1;
    int n2 = right - mid;

    Location* L = new Location[n1];
    Location* R = new Location[n2];

    for (int i = 0; i < n1; i++)
    {
        L[i] = arr[left + i];
    }

    for (int j = 0; j < n2; j++)
    {
        R[j] = arr[mid + 1 + j];
    }

    int i = 0;
    int j = 0;
    int k = left;

    while (i < n1 && j < n2)
    {
        if (L[i].name <= R[j].name)
        {
            arr[k] = L[i];

            i++;
        }
        else
        {
            arr[k] = R[j];

            j++;
        }

        k++;
    }

    while (i < n1)
    {
        arr[k] = L[i];

        i++;
        k++;
    }

    while (j < n2)
    {
        arr[k] = R[j];

        j++;
        k++;
    }

    delete[] L;
    delete[] R;
}

void mergeSortByName(
    Location arr[],
    int left,
    int right
)
{
    if (left >= right)
    {
        return;
    }

    int mid = left + (right - left) / 2;

    mergeSortByName(arr, left, mid);

    mergeSortByName(arr, mid + 1, right);

    merge(arr, left, mid, right);
}