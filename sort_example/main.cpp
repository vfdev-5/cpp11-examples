#include <iostream>
#include <algorithm>

using namespace std;

template<typename T>
void print_array(const vector<T> & array) {
    cout << "array : ";
    for_each(begin(array), end(array), [](int v){
        cout << v << " ";
    });
    cout << endl;
}

template <typename T>
void merge_sort(vector<T> & array)
{
    if (array.size() > 0)
    {
        // divide
        int n = array.size()/2;
        vector<T> left(array.begin(), array.begin() + n);
        vector<T> right(array.begin() + n, array.end());

        print_array(left);
        print_array(right);
        // merge
    }

}

template <typename T>
void merge(vector<T> & array)
{
}



int main()
{

    vector<int> array1 = {6,4,7,9,3,45,76,8,9,5,34,23,4,6,7,5,4,3,4,6};
    vector<int> array2 = {6,4,7,9,3,45,76,8,9,5,34,23,4,6,7,5,4,3,4,6};


    sort(begin(array1), end(array1), [](auto a, auto b) -> bool { return a < b; });
    merge_sort(array2);



    print_array(array1);
    print_array(array2);



    return 0;
}

