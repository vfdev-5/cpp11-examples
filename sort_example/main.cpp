#include <iostream>
#include <vector>
#include <chrono>
#include <cstdlib>

using namespace std;

// Project
#include "merge_sort.h"

template<typename T>
void print_array(const vector<T> & array) {
    cout << "array : ";
    for_each(begin(array), end(array), [](int v){
        cout << v << " ";
    });
    cout << endl;
}

// ********************************************************************************************************
// ********************************************************************************************************

int main()
{
//    vector<int> array1 = {6,4,7,9,3,45,76,8,9,5,34,23,4,6,7,5,4,3,4,6};

    vector<int> array1(100, 0);
    for_each(begin(array1), end(array1), [](int & v){
        v = rand() % 500;
    });
    vector<int> array2(array1);
    vector<int> array3(array1);

    auto t0 = chrono::high_resolution_clock::now();

    sort(begin(array1), end(array1), [](auto a, auto b) -> bool { return a < b; });

    auto t1 = chrono::high_resolution_clock::now();

    // GLOBAL_OP_COUNTER reset
    GLOBAL_OP_COUNTER = 0;
    merge_sort(begin(array2), end(array2));

    auto t2 = chrono::high_resolution_clock::now();

    parallel_merge_sort(begin(array3), end(array3), 8);

    auto t3 = chrono::high_resolution_clock::now();

    double nlog2n = array2.size() * log2( array2.size() );
    cout << "Number of operations : " << GLOBAL_OP_COUNTER << ", n*log2(n) = " << nlog2n << endl;

    print_array(array1);
    print_array(array2);
    print_array(array3);

    assert (array1 == array2 && array1 == array3), "Results are not identical";


    cout << "std sort time (sec): " << chrono::duration<float>(t1 - t0).count() << endl;
    cout << "merge sort time (sec): " << chrono::duration<float>(t2 - t1).count() << endl;
    cout << "parallel merge sort time (sec): " << chrono::duration<float>(t3 - t2).count() << endl;

    return 0;
}

// ********************************************************************************************************
// ********************************************************************************************************



