#include <iostream>
#include <pthread.h>

#define MAX_THREAD 4
using namespace std;

struct thread_data {
    int *arr;
    int start;
    int end;
    int exp;
};

int getMax(int arr[], int n) {

    int mx = arr[0];
    for (int i = 1; i < n; i++)
        if (arr[i] > mx)
            mx = arr[i];
    return mx;
}

int *countSort(int arr[], int n, int exp) {

    int output[n]; // output array
    int i, count[10] = {0};
    int *count_copy = new int[10]();

    // Store count of occurrences in count[]
    for (i = 0; i < n; i++) {
        int b= arr[i];
        int k = (b / exp) % 10;
        count_copy[(arr[i] / exp) % 10]++;
        count[(arr[i] / exp) % 10]++;
    }



    // Change count[i] so that count[i] now contains actual
    //  position of this digit in output[]
    for (i = 1; i < 10; i++)
        count[i] += count[i - 1];

    // Build the output array
    for (i = n - 1; i >= 0; i--) {
        output[count[(arr[i] / exp) % 10] - 1] = arr[i];
        count[(arr[i] / exp) % 10]--;
    }

    // Copy the output array to arr[], so that arr[] now
    // contains sorted numbers according to current digit
    for (i = 0; i < n; i++)
        arr[i] = output[i];

    return count_copy;
}

void *countSortThread(void *thread_arg) {

    struct thread_data *my_data;

    my_data = (struct thread_data *) thread_arg;

    int output[my_data->end - my_data->start]; // output array
    int i, count[10] = {0};

    // Store count of occurrences in count[]
    for (i = my_data->start; i < my_data->end; i++)
        count[(my_data->arr[i] / my_data->exp) % 10]++;

    // Change count[i] so that count[i] now contains actual
    //  position of this digit in output[]
    for (i = 1; i < 10; i++)
        count[i] += count[i - 1];

    // Build the output array
    for (i = my_data->end - 1; i >= my_data->start; i--) {
        output[count[(my_data->arr[i] / my_data->exp) % 10] - 1] = my_data->arr[i];
        count[(my_data->arr[i] / my_data->exp) % 10]--;
    }

    // Copy the output array to arr[], so that arr[] now
    // contains sorted numbers according to current digit
    for (i = my_data->start; i < my_data->end; i++)
        my_data->arr[i] = output[i];

    pthread_exit(NULL);
}

void radixSort(int arr[], int n) {
    int m = getMax(arr, n);
    int exp = 1;
    while (exp <= m / 10) {
        exp *= 10;
    }

    int *thread_divide = countSort(arr, n, exp);

//    for (int i = 0; i < 10; i++)
//        cout << thread_divide[i] << endl;

    pthread_t threads[10];
    thread_data data[10];

    pthread_attr_t attr;
    void * status;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);



    int rc;
    int last = 0;
    exp /= 10;
    for (int i = 0; exp >= 1; exp /= 10) {
        cout << "exp " << exp;
        for (int i = 0; i < 10; i++) {
            data[i].arr = arr;
            data[i].start = last;
            data[i].end = thread_divide[i];
            data[i].exp = exp;
            last += thread_divide[i];

            rc = pthread_create(&threads[i], &attr, countSortThread, (void *) &data[i]);
            if(rc) {
                cout << "Error creating thread" << endl;
                exit(-1);
            }
        }
    }
    pthread_attr_destroy(&attr);

    for(int i = 0; i < 10; i++) {
        rc = pthread_join(threads[i], &status);

        if(rc) {
            cout << "Unable to join the thread " << i << " " << rc << endl;
            exit(-1);
        }

        cout << "completed thread " << i << endl;
        cout << "exiting with status " << status << endl;

    }

    for(int i = 0; i < n; i++) {
        cout << arr[i] << endl;
    }

    int k = 2;

}

int main(int argc, char *argv[]) {
    int arr[] = {19, 22, 35, 84, 12, 23, 54};
    int n = sizeof(arr) / sizeof(arr[0]);

    radixSort(arr, n);
}