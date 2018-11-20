/*//https://www.geeksforgeeks.org/radix-sort/
// C++ implementation of Radix Sort
#include<iostream>
#include <fstream>
#include <sstream>

using namespace std;
int SIZE = 0;
// A utility function to get maximum value in arr[]
int getMax(int arr[], int n)
{
    int mx = arr[0];
    for (int i = 1; i < n; i++)
        if (arr[i] > mx)
            mx = arr[i];
    return mx;
}

// A function to do counting sort of arr[] according to
// the digit represented by exp.
void countSort(int arr[], int n, int exp)
{
    int output[n]; // output array
    int i, count[10] = {0};

    // Store count of occurrences in count[]
    for (i = 0; i < n; i++)
        count[ (arr[i]/exp)%10 ]++;

    // Change count[i] so that count[i] now contains actual
    //  position of this digit in output[]
    for (i = 1; i < 10; i++)
        count[i] += count[i - 1];

    // Build the output array
    for (i = n - 1; i >= 0; i--)
    {
        output[count[ (arr[i]/exp)%10 ] - 1] = arr[i];
        count[ (arr[i]/exp)%10 ]--;
    }

    // Copy the output array to arr[], so that arr[] now
    // contains sorted numbers according to current digit
    for (i = 0; i < n; i++)
        arr[i] = output[i];
}

// The main function to that sorts arr[] of size n using
// Radix Sort
void radixsort(int arr[], int n)
{
    // Find the maximum number to know number of digits
    int m = getMax(arr, n);

    // Do counting sort for every digit. Note that instead
    // of passing digit number, exp is passed. exp is 10^i
    // where i is current digit number
    for (int exp = 1; m/exp > 0; exp *= 10)
        countSort(arr, n, exp);
}

// A utility function to print an array
void print(int arr[], int n)
{
    for (int i = 0; i < n; i++)
        cout << arr[i] << " ";
}

int* read_file(char* path) {

    ifstream input_file(path);

    if(!input_file.is_open()) {
        cout << "cannot read file, aborting...";
        exit(1);
    }

    string line;
    getline(input_file, line);

    int size;

    istringstream first_line(line);

    first_line >> size;

    SIZE = size;

    int *arr = new int[size]();


    for( int i = 0; i < size; i++) {
        getline(input_file, line);

        istringstream iss(line);

        int a;
        iss >> a;
        arr[i] = a;
    }

    return arr;
}


void write_file(char* path, int* arr, int n) {

    ofstream file;

    file.open(path);

    if(!file.is_open()) {
        cout << "cannot write file, aborting...";
        exit(1);
    }

    for(int i = 0; i < n; i++) {
        file << arr[i] << endl;
    }

    file.close();
}

// Driver program to test above functions
int main(int argc, char *argv[])
{
    int* arr = read_file(argv[1]);

    radixsort(arr, SIZE);

//    write_file(argv[2], arr, SIZE);

    return 0;
}*/