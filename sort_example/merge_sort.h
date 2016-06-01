#ifndef MERGE_SORT_H
#define MERGE_SORT_H

#include <algorithm>
#include <assert.h>
#include <math.h>
#include <thread>

using namespace std;

int GLOBAL_OP_COUNTER = 0;

template <typename Iterator>
struct vector_bounds
{
    vector_bounds(const Iterator & l, const Iterator & r) :
        begin(l), end(r)
    {}
    const Iterator begin; // copy
    const Iterator end; // copy
    int size() const
    { return end - begin; }
};

template<typename Iterator>
void print_array(const vector_bounds<Iterator> & array) {
    cout << "array : ";
    for_each(array.begin, array.end, [](int v){
        cout << v << " ";
    });
    cout << endl;
}

template<typename Iterator>
void merge(const vector_bounds<Iterator> & left, const vector_bounds<Iterator> & right, vector_bounds<Iterator> & array);

template<typename Iterator>
void merge_sort(Iterator leftIt, Iterator rightIt);

template<typename Iterator>
void parallel_merge_sort(Iterator leftIt, Iterator rightIt);


template<typename Iterator>
void merge(const vector_bounds<Iterator> & left, const vector_bounds<Iterator> & right, vector_bounds<Iterator> & array)
{
    // cout << "Merge left + right" << endl;
    // print_array(left);
    // print_array(right);

    assert(left.size() + right.size() == array.size());

    auto lit = left.begin;
    auto rit = right.begin;
    auto ait = array.begin;


    while ( lit != left.end && rit != right.end )
    {
        if (*lit < *rit)
        {
            *ait = *lit;
            lit++;
        }
        else
        {
            *ait = *rit;
            rit++;
        }
        ait++;

        // GLOBAL_OP_COUNTER
        GLOBAL_OP_COUNTER++;

    }

    auto it = (lit != left.end) ? lit : rit;
    auto end = (lit != left.end) ? left.end : right.end;

    for (;it != end && ait != array.end; ++it, ++ait)
    {
        *ait = *it;

        // GLOBAL_OP_COUNTER
        GLOBAL_OP_COUNTER++;
    }

    // print_array(array);
    // cout << "END Merge left + right" << endl;
}

// ********************************************************************************************************

template<typename Iterator>
void merge_sort(Iterator leftIt, Iterator rightIt)
{

    // cout << "Merge sort : size=" << (rightIt - leftIt) << endl;
    if ((rightIt - leftIt) > 1)
    {
        // divide
        int n = (rightIt - leftIt)/2;
        vector_bounds<Iterator> left(leftIt, leftIt + n);
        vector_bounds<Iterator> right(leftIt + n, rightIt);

        // print_array(left);
        // print_array(right);

        merge_sort(left.begin, left.end);
        merge_sort(right.begin, right.end);

        // copy left and right arrays
        vector<typename iterator_traits<Iterator>::value_type> left_copy(left.begin, left.end);
        vector<typename iterator_traits<Iterator>::value_type> right_copy(right.begin, right.end);
        vector_bounds<Iterator> left_copy_bounds(left_copy.begin(), left_copy.end());
        vector_bounds<Iterator> right_copy_bounds(right_copy.begin(), right_copy.end());

        // merge
        vector_bounds<Iterator> array_bounds(leftIt, rightIt);
        merge(left_copy_bounds, right_copy_bounds, array_bounds);
    }
    // cout << "END Merge sort" << endl;

}

// ********************************************************************************************************
/*
    Merge sort processing time is
        T(n) = a * T(n/b) + f(n)
    with a = 2, b = 2

    Parallelize some T(n/b) tasks
 */
template<typename Iterator>
void parallel_merge_sort_part(Iterator leftIt, Iterator rightIt)
{
//    cout << "Current thread : " << this_thread::get_id() << ", task : " << (rightIt - leftIt) << endl;
    if ((rightIt - leftIt) > 1)
    {
        // divide
        int n = (rightIt - leftIt)/2;
        vector_bounds<Iterator> left(leftIt, leftIt + n);
        vector_bounds<Iterator> right(leftIt + n, rightIt);

        // print_array(left);
        // print_array(right);

        parallel_merge_sort_part(left.begin, left.end);
        parallel_merge_sort_part(right.begin, right.end);

        // copy left and right arrays
        vector<typename iterator_traits<Iterator>::value_type> left_copy(left.begin, left.end);
        vector<typename iterator_traits<Iterator>::value_type> right_copy(right.begin, right.end);
        vector_bounds<Iterator> left_copy_bounds(left_copy.begin(), left_copy.end());
        vector_bounds<Iterator> right_copy_bounds(right_copy.begin(), right_copy.end());

        // merge
        vector_bounds<Iterator> array_bounds(leftIt, rightIt);
        merge(left_copy_bounds, right_copy_bounds, array_bounds);
    }
}


template<typename Iterator>
void parallel_merge_sort(Iterator leftIt, Iterator rightIt, const int nb_threads = 4)
{
//    cout << "Parallel merge sort : " << endl;
    // Divide into 2^log2(nb_threads) parts to execute merge sort in parallel
    int nb_parts = static_cast<int>( floor( pow(2.0, log2( nb_threads ) ) ) );
    // nb_parts <= nb_threads
//    cout << "- nb_parts : " << nb_parts << endl;

    vector<thread> threads;

    // std vector is thread safe in the following sense :
    // - can read a vector object from multiple threads simultaneously.
    // - if there is one thread changing a vector object, there shall be neither concurrent readers or writers.
    // - accesses to a vector object don't interfere with other vector objects.

    vector<vector_bounds<Iterator>> parts;
    // Start threads on the parts
    int part_size = static_cast<int>(ceil( (rightIt - leftIt) / nb_parts ) );
    int i = 0;
    for (;i<nb_parts-1; i++)
    {
        parts.emplace_back(leftIt + i*part_size, leftIt + (i+1)*part_size);
        threads.emplace_back(parallel_merge_sort_part<Iterator>, leftIt + i*part_size, leftIt + (i+1)*part_size);
    }
    parts.emplace_back(leftIt + i*part_size, rightIt);
    threads.emplace_back(parallel_merge_sort_part<Iterator>, leftIt + i*part_size, rightIt);

    // Wait until parts are sorted
    for (auto & trd : threads)
    {
        trd.join();
    }


    // Merge parts using one thread
    int counter = nb_parts - 1;
    auto it = parts.begin();
    int step = 1;
    while (counter > 0)
    {
//        cout << "Counter: " << counter << " | step: " << step << endl;

        // copy left and right arrays
        const Iterator & left_begin = it->begin;
        const Iterator & left_end = (it + step)->begin;
        const Iterator & right_begin = left_end;
        const Iterator & right_end = (it + 2*step != parts.end()) ? (it + 2*step)->begin : (parts.end() - 1)->end;
        vector<typename iterator_traits<Iterator>::value_type> left_copy(left_begin, left_end);
        vector<typename iterator_traits<Iterator>::value_type> right_copy(right_begin, right_end);
        vector_bounds<Iterator> left_copy_bounds(left_copy.begin(), left_copy.end());
        vector_bounds<Iterator> right_copy_bounds(right_copy.begin(), right_copy.end());

//        print_array(left_copy_bounds);
//        print_array(right_copy_bounds);

        // merge
        vector_bounds<Iterator> array_bounds(left_begin, right_end);
        merge(left_copy_bounds, right_copy_bounds, array_bounds);
//        print_array(array_bounds);

        // increment all
        if (it+2*step != parts.end())
            it += 2*step;
        else
        {
            it = parts.begin();
            step *= 2;
        }
        counter--;
    }

//    cout << endl << "Parts : size=" << parts.size() << endl;
//    for (const vector_bounds<Iterator> & part : parts)
//    {
//        print_array(part);
//    }
//    cout << "END Parallel merge sort : " << endl;
}

// ********************************************************************************************************

#endif // MERGE_SORT_H
