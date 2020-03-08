#include <iostream>
#include <vector>
using namespace std;

std::vector<int> prueba = {2, 5, 3, 6, 1, 4};

void insertionSort(int arr[], int n)
{
    int i, key, j;
    for (i = 1; i < n; i++)
    {
        key = arr[i];
        j = i - 1;

        /* Move elements of arr[0..i-1], that are
        greater than key, to one position ahead
        of their current position */
        while (j >= 0 && arr[j] > key)
        {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;
    }
}

int main() {
    for (int i = 0; i < prueba.size(); i++) cout << prueba[i];
    cout << '\n';
    insertionSort(&prueba[0], prueba.size());
    for (int i = 0 ; i < prueba.size(); i++)
        cout << prueba[i];
    cout << '\n';
	return 0;
}
