#include <iostream>
#include <pthread.h>
#include <fstream>
#include <sstream>
#include <random>
#include <mutex>
#include <thread>

#define MAX_THREAD 4
using namespace std;

int SIZE = 0;

struct thread_data {
    int *arr;
    int start;
    int end;
    int exp;
};

struct thread_data_p {
    int *arr;
    int size;
    int exp;
};

int getMax(int arr[], int n);

int *countSort(int arr[], int n, int exp);

void countSortThread(void *thread_arg);

void radixSort(int arr[], int n);

void write_file(char* path, int *arr, int n);

int* read_file(char* path);

int main(int argc, char *argv[]) {

    if (argc < 2) {
        cout << "wrong argument" << endl;
        return 1;
    }
    int* arr = read_file(argv[1]);

    int n = SIZE;

    radixSort(arr, n);

    //write_file(argv[2], arr, n);
//        constexpr unsigned num_threads = 6;
         // mutex ensures orderly access to std::cout from multiple threads.
//        std::mutex iomutex;
//        std::vector<std::thread> threads(num_threads);
//        for (unsigned i = 0; i < num_threads; ++i) {
//            threads[i] = std::thread([] {
//                while (1) {
//                    {
//                        // Use a lexical scope and lock_guard to safely lock the mutex only
//                        // for the duration of std::cout usage.
////                        std::lock_guard<std::mutex> iolock(iomutex);
////                        std::cout << "Thread #" << i << ": on CPU " << sched_getcpu() << "\n";
//                    }
//
//                    // Simulate important work done by the tread by sleeping for a bit...
//                    //std::this_thread::sleep_for(std::chrono::milliseconds(3000));
//                }
//            });
//        }
//
//        for (auto& t : threads) {
//            cout << "threads joining" << endl;
//            t.join();
//        }
//        return 0;
//    std::random_device rd; // obtain a random number from hardware
//    std::mt19937 eng(rd()); // seed the generator
//    std::uniform_int_distribution<> distr(100000000, 1000000000); // define the range
//
//    ofstream file;
//
//    file.open(argv[2]);
//
//    if(!file.is_open()) {
//        cout << "cannot write file, aborting...";
//        exit(1);
//    }
//
//    for(int i = 0; i < 10000000; i++) {
//        file << distr(eng)  << endl;
//    }
//
//    file.close();
}

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

void countSortThread(void *thread_arg) {

    struct thread_data_p *my_data;

    my_data = (struct thread_data_p *) thread_arg;

    int size = my_data->size;

    int output[my_data->size]; // output array
    int i, count[10] = {0};

    // Store count of occurrences in count[]
    for (i = 0; i < my_data->size; i++)
        count[(my_data->arr[i] / my_data->exp) % 10]++;

    // Change count[i] so that count[i] now contains actual
    //  position of this digit in output[]
    for (i = 1; i < 10; i++)
        count[i] += count[i - 1];

    // Build the output array
    for (i = my_data->size - 1; i >= 0; i--) {
        output[count[(my_data->arr[i] / my_data->exp) % 10] - 1] = my_data->arr[i];
        count[(my_data->arr[i] / my_data->exp) % 10]--;
    }

    // Copy the output array to arr[], so that arr[] now
    // contains sorted numbers according to current digit
    for (i = 0; i < my_data->size; i++)
        my_data->arr[i] = output[i];

}

void radixSort(int arr[], int n) {
    int m = getMax(arr, n);
    int exp = 1;
    while (exp <= m / 10) {
        exp *= 10;
    }

    int *thread_divide = countSort(arr, n, exp);

    exp /= 10;

    thread_data_p data[10];

    int last = 0;
    int fexp = 1;
    for (int i = 0; i < 10; i++) {
        data[i].arr = &arr[last];
        data[i].size = thread_divide[i];
        last += thread_divide[i];
        cout << thread_divide[i] << endl;
    }



    int rc;
    int thread_active[10] = {0};

    for (int fexp = 1; m/(10*fexp) > 0; fexp *= 10) {
        std::vector<std::thread> threads(10);

//        pthread_t threads[10];
//
//        pthread_attr_t attr;
//        void * status;
//        pthread_attr_init(&attr);
//        pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
        //cout << "exp " << exp;
        for (int i = 0; i < 10; i++) {
            data[i].exp = fexp;

            if(data[i].size != 0) {
                thread_active[i] = 1;
//                cout << "Creating thread " << i << endl;

                threads[i] = std::thread([data, i] {
                    //while (true) {

                    countSortThread((void *) &data[i]);
                        // Simulate important work done by the tread by sleeping for a bit...
//                        std::this_thread::sleep_for(std::chrono::milliseconds(900));
                    //}
                });
//                rc = pthread_create(&threads[i], &attr, countSortThread, (void *) &data[i]);
//                if(rc) {
//                    cout << "Error creating thread" << endl;
//                    exit(-1);
//                }
            }
        }

//        pthread_attr_destroy(&attr);

        for(int i = 0; i < 10; i++) {
            if(thread_active[i] == 0) {
                continue;
            }
            threads[i].join();
//            thread_active[i] = 0;
//
//            rc = pthread_join(threads[i], &status);
//            cout << "Status " << status;
//            if(rc) {
//                cout << "Unable to join the thread " << i << " " << rc << endl;
//                exit(-1);
//            }
        }
    }
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