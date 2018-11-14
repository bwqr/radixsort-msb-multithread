#include <iostream>
#include <pthread.h>

#define MAX_THREAD 4

int getMax(int arr[], int n) {

    int mx = arr[0];
    for (int i = 1; i < n; i++)
        if (arr[i] > mx)
            mx = arr[i];
    return mx;
}

void countSort(int arr[], int start, int end, int exp) {

    int output[end - start]; // output array
    int i, count[10] = {0};

    // Store count of occurrences in count[]
    for (i = start; i < end; i++)
        count[ (arr[i]/exp)%10 ]++;

    // Change count[i] so that count[i] now contains actual
    //  position of this digit in output[]
    for (i = 1; i < 10; i++)
        count[i] += count[i - 1];

    // Build the output array
    for (i = end - 1; i >= start; i--)
    {
        output[count[ (arr[i]/exp)%10 ] - 1] = arr[i];
        count[ (arr[i]/exp)%10 ]--;
    }

    // Copy the output array to arr[], so that arr[] now
    // contains sorted numbers according to current digit
    for (i = start; i < end; i++)
        arr[i] = output[i];
}

void radixSort(int arr[], int n) {
    int m = getMax(arr, n);

    int exp = 1;

    while(exp <= m) {
        exp *= 10;
    }

    for (; exp > 1; exp /= 10)
        countSort(arr, 2, 4, exp);
}

int main(int argc, char* argv[]) {
    int arr[] = {19, 22, 35, 84, 12, 23, 54};
    int n = sizeof(arr) / sizeof(arr[0]);

    radixSort(arr, n);
}