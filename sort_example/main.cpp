#include <iostream>
#include <algorithm>
#include <cassert>


using namespace std;

template<typename T>
void print_array(const vector<T> & array) {
    cout << "array : ";
    for_each(begin(array), end(array), [](int v){
        cout << v << " ";
    });
    cout << endl;
}

template <typename Iterator>
struct vector_bounds
{
    vector_bounds(const Iterator & l, const Iterator & r) :
        begin(l), end(r)
    {}
    const Iterator & begin;
    const Iterator & end;
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


// ********************************************************************************************************
// ********************************************************************************************************

int main()
{

    vector<int> array1 = {6,4,7,9,3,45,76,8,9,5,34,23,4,6,7,5,4,3,4,6};
    vector<int> array2 = {6,4,7,9,3,45,76,8,9,5,34,23,4,6,7,5,4,3,4,6};

    sort(begin(array1), end(array1), [](auto a, auto b) -> bool { return a < b; });
    merge_sort(begin(array2), end(array2));

    print_array(array1);
    print_array(array2);

    return 0;
}

// ********************************************************************************************************
// ********************************************************************************************************


template<typename Iterator>
void merge(const vector_bounds<Iterator> & left, const vector_bounds<Iterator> & right, vector_bounds<Iterator> & array)
{
    cout << "Merge left + right" << endl;
    print_array(left);
    print_array(right);

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
    }

    auto it = (lit != left.end) ? lit : rit;
    auto end = (lit != left.end) ? left.end : right.end;

    for (;it != end && ait != array.end; ++it, ++ait)
    {
        *ait = *it;
    }

    print_array(array);
    cout << "END Merge left + right" << endl;
}

// ********************************************************************************************************

template<typename Iterator>
void merge_sort(Iterator leftIt, Iterator rightIt)
{

    cout << "Merge sort : size=" << (rightIt - leftIt) << endl;
    if ((rightIt - leftIt) > 1)
    {
        // divide
        int n = (rightIt - leftIt)/2;
        vector_bounds<Iterator> left(leftIt, leftIt + n);
        vector_bounds<Iterator> right(leftIt + n, rightIt);

        print_array(left);
        print_array(right);

        merge_sort(left.begin, left.end);
        merge_sort(right.begin, right.end);

        // copy left and right arrays
        vector<typename std::iterator_traits<Iterator>::value_type> left_copy(left.begin, left.end);
        vector<typename std::iterator_traits<Iterator>::value_type> right_copy(right.begin, right.end);
        vector_bounds<Iterator> left_copy_bounds(left_copy.begin(), left_copy.end());
        vector_bounds<Iterator> right_copy_bounds(right_copy.begin(), right_copy.end());

        // merge
        vector_bounds<Iterator> array_bounds(leftIt, rightIt);
        merge(left_copy_bounds, right_copy_bounds, array_bounds);
        // replace
    }
    cout << "END Merge sort" << endl;

}

// ********************************************************************************************************
/*




 */
template<typename Iterator>
void parallel_merge_sort(Iterator leftIt, Iterator rightIt)
{



}

// ********************************************************************************************************
