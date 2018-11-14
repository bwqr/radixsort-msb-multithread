cmake_minimum_required(VERSION 3.12)
project(radixsort_msb_multithread)

set(CMAKE_CXX_STANDARD 14)

add_executable(radixsort_msb_multithread main.cpp radixmsb.cpp)