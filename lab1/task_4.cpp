#include <iostream>
#include <vector>
#include <omp.h>
#include <chrono>

using namespace std;

void merge_arrays(vector<int>& arr, vector<int>& buffer, size_t length_left, size_t length_right, int start_left, int start_right)
{
    int idx_left, idx_right, idx_buffer;
    idx_left = idx_right = idx_buffer = 0;

    while ((idx_left < length_left) && (idx_right < length_right))
    {
        if (arr[start_left + idx_left] < arr[start_right + idx_right])
        {
                do {
                        buffer[start_left + idx_buffer] = arr[start_left + idx_left];
                        ++idx_buffer;
                        ++idx_left;
                } while ((idx_left < length_left) && (arr[start_left + idx_left] < arr[start_right + idx_right]));
        }
        else
        {
                do {
                        buffer[start_left + idx_buffer] = arr[start_right + idx_right];
                        ++idx_buffer;
                        ++idx_right;
                } while ((idx_right < length_right) && (arr[start_right + idx_right] < arr[start_left + idx_left]));
        }
    }

    if (idx_left == length_left)
    {
        for (; idx_right < length_right; ++idx_right)
        {
                buffer[start_left + idx_buffer] = arr[start_right + idx_right];
                ++idx_buffer;
        }
    }
    else
    {
        for (; idx_left < length_left; ++idx_left)
        {
                buffer[start_left + idx_buffer] = arr[start_left + idx_left];
                ++idx_buffer;
        }
    }

    for (int i = 0; i < idx_buffer; ++i)
    {
        arr[start_left + i] = buffer[start_left + i];
    }
}

void merge_sort(vector<int>& arr, vector<int>& buffer, size_t length, int from)
{
    if (length == 1) return;

    int length_left = length / 2;
    int length_right = length - length_left;

    #pragma omp task firstprivate (arr, length_left)
    merge_sort(arr, buffer, length_left, from);
    #pragma omp task firstprivate (arr, length_right)
    merge_sort(arr, buffer, length_right, from + length_left);

   #pragma omp taskwait
    merge_arrays(arr, buffer, length_left, length - length_left, from, from + length_left);

}

void generate_int_array(vector<int>& arr, int length){
    for (int i = 0; i < length; ++i)
        arr.push_back(rand() %100- 50);
}

void output(vector<int>& arr){
    for (int i = 0; i < arr.size(); ++i){
                cout << arr[i] << " ";
    }
    cout << endl;
}

int main(){
    int length, type;
    vector<int> arr1;
    vector<int> buffer;
    cout << "if you want to enter an array enter 1 \nif you want to generate a random array enter 2" << endl;
    cin >> type;
    cout << "enter the size of the array"<<endl;
    cin >> length;
    if (type == 1) {
        int elem = 0;
        for (int i = 0; i < length; ++i){
            cout << "enter " << i <<"-element"<<endl;
            cin >> elem;
                    arr1.push_back(elem);
        }
                output(arr1);
    }
    else generate_int_array(arr1, length);
    output(arr1);
    for (int i = 0; i < length; ++i)
        buffer.push_back(0);
    auto begin = chrono::steady_clock::now();
   #pragma omp parallel
   {
    #pragma omp single
    merge_sort(arr1, buffer, length, 0);
    }
    auto end = chrono::steady_clock::now();
    auto time = chrono::duration_cast<chrono::milliseconds>(end - begin);
    cout << "result: " << endl;
    output(arr1);
    cout << "time: "<<time.count()<<endl;
}
