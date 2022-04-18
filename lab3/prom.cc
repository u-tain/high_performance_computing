#include <iostream>
#include <vector>
#include <mpi.h>

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
int merge_sorted_after_multithreading(std::vector<int>& arr, std::vector<int>& buffer,
        vector<int>& thread_from, vector<int>& thread_length,
        unsigned int threads_count, int i_thread)
{
        if (threads_count == 1)
                return thread_length[i_thread];

        int count_left = threads_count / 2;
        int i_thread_left = i_thread;
        int count_right = threads_count - count_left;
        int i_thread_right = i_thread + count_left;

        // вызываем рекурсию для левой и правой части
        int length_left_merged = merge_sorted_after_multithreading(arr, buffer, thread_from, thread_length, count_left, i_thread_left);
        int length_rigth_merged = merge_sorted_after_multithreading(arr, buffer, thread_from, thread_length, count_right, i_thread_right);

        // слияние упорядоченных частей
        merge_arrays(arr, buffer, length_left_merged, length_rigth_merged, thread_from[i_thread_left], thread_from[i_thread_right]);
        return length_left_merged + length_rigth_merged;
}

void make_parts(std::vector<int>& thread_from, std::vector<int>& thread_length, unsigned int threads_count, size_t length)
{
        int dlength = (length / threads_count);
        int odd_length = length % threads_count;
        int offset = 0;
        for (int i = 0; i < threads_count; ++i)
        {
                if (odd_length > 0)
                {
                        thread_length.push_back(dlength + 1);
                        --odd_length;
                }
                else
                        thread_length.push_back(dlength);

                thread_from.push_back(offset);
                offset += thread_length[i];
        }
}

void merge_sort(vector<int>& arr, vector<int>& buffer, size_t length, int from)
{
    if (length == 1) return;

    int length_left = length / 2;
    int length_right = length - length_left;

    merge_sort(arr, buffer, length_left, from);
    merge_sort(arr, buffer, length_right, from + length_left);

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

int main(int argc, char** argv){
    //инициализация
    int length, type, nranks, rank, name_len;
    vector<int> arr;
    vector<int> buffer;
    char processor_name[MPI_MAX_PROCESSOR_NAME];
    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &nranks);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0){
      cout << "if you want to enter an array enter 1 \nif you want to generate a random array enter 2" << endl;
      cin >> type;
      cout << "enter the size of the array"<<endl;
      cin >> length;
      if (type == 1) {
        int elem = 0;
        for (int i = 0; i < length; ++i){
            cout << "enter " << i <<"-element"<<endl;
            cin >> elem;
                    arr.push_back(elem);
       }
     }
     else generate_int_array(arr, length);
     output(arr);
     }
     MPI_Bcast(&arr[0], length, MPI_INT , 0, MPI_COMM_WORLD);

     for (int i = 0; i < arr.size(); ++i)
        buffer.push_back(0);
     vector<int> thread_from;
     vector<int> thread_length;
     make_parts(thread_from, thread_length, nranks, length);
  //   cout << "indices: "<<endl;
  //   output(thread_from);
     int length_length = thread_length.size();
     int threads_length[length_length];
     int threads_from[length_length];
     for (int i = 0; i< length_length; i++){
        threads_length[i]= thread_length[i];
        threads_from[i] = thread_from[i];}
    if (rank !=0){
        output(arr);}
    merge_sort(arr, buffer, thread_length[rank], thread_from[rank]);
    MPI_Get_processor_name(processor_name, &name_len);
    cout<< "node: " << processor_name << " rank: "<< rank <<" indices: "<< thread_from[rank]<<endl;
        // ожидаем пока все выполнятся
//    MPI_Barrier(MPI_COMM_WORLD);
    //MPI_Gatherv(&arr[0], length,  MPI_INT, &buffer[0], threads_length, threads_from, MPI_INT, 0, MPI_COMM_WORLD);
        cout<<"buffer: "<<endl;
        output(buffer);
   if (rank == 0){
        //for (int i = 0; i < arr.size(); ++i)
        //   buffer1.push_back(0);
        //merge_sorted_after_multithreading(buffer, buffer1, thread_from, thread_length, nranks, 0);

    //if (rank == 0){
      cout << "result: " << endl;
      output(arr);
    }
    MPI_Finalize();
    return 0;
}
