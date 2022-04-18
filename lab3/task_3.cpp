#include <mpi.h>
#include <iostream>

using namespace std;

void generate_int_array(int *arr, int length){
  for (int i = 0; i < length; ++i)
    arr[i] = rand() % 1000- 500;
}

void output(int *arr, int length ){
    for (int i = 0; i < length; ++i){
                cout << arr[i] << " ";
    }
    cout << endl;
}

void make_parts(int *thread_from, int *thread_length, unsigned int threads_count, size_t length)
{
  int dlength = (length / threads_count);
        int odd_length = length % threads_count;
        int offset = 0;
        for (int i = 0; i < threads_count; ++i){
                if (odd_length > 0)
                {
                        thread_length[i] = dlength + 1;
                        --odd_length;
                }
                else
                        thread_length[i]=dlength;

                thread_from[i] = offset;
                offset += thread_length[i];
        }
}


void merge_arrays(int *arr, int *buffer, size_t length_left, size_t length_right, int start_left, int start_right)
{
        int idx_left, idx_right, idx_buffer;
        idx_left = idx_right = idx_buffer = 0;

        // слияние массивов, пока не дойдем до конца одного из них
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

        // слияние остатка не законченного массива с итоговым
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

        // копирование итогового упорядоченного массива в исходный
        for (int i = 0; i < idx_buffer; ++i)
        {
                arr[start_left + i] = buffer[start_left + i];
        }
}

void merge_sort(int *arr, int *buffer, size_t length, int from)
{
        // массив длины 1 упорядочен
        if (length == 1) return;
        // делим массив пополам
        int length_left = length / 2;
        int length_right = length - length_left;

        // вызываем рекурсию для левой и правой части
        merge_sort(arr, buffer, length_left, from);
        merge_sort(arr, buffer, length_right, from + length_left);

        // слияние упорядоченных частей
        merge_arrays(arr, buffer, length_left, length - length_left, from, from + length_left);
}


int merge_sorted_after_multithreading(int *arr, int *buffer,
        int *thread_from, int *thread_length,
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
int main(){
    int length, type, nranks, rank, name_len;
    int *arr, *thread_from, *thread_length, *buffer, *arr1, *buffer1;
    MPI_Status status;
    char processor_name[MPI_MAX_PROCESSOR_NAME];
    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &nranks);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0){
      cout << "if you want to enter an array enter 1 \nif you want to generate a random array enter 2" << endl;
      cin >> type;
      if (type != 1 and type !=2){
        cout << "invalid identifier. please press ctrl+c " << endl;
         MPI_Finalize();
         return 0;
      }
      cout << "enter the size of the array"<<endl;
      cin >> length;
      if ( length<= nranks){
         cout << "array length <= number of processes. too short for multi-threaded counting. please press ctrl+c"<<endl;
         MPI_Finalize();
         return 0;
      }
      arr = (int *)malloc(length * sizeof(int));
      if (type == 1) {
        int elem = 0;
        for (int i = 0; i < length; ++i){
            cout << "enter " << i <<"-element"<<endl;
            cin >> elem;
                    arr[i] = elem;
        }
      }
    else if (type == 2) generate_int_array(arr, length);
    //  output(arr, length);
    }
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Bcast(&length, 1, MPI_INT ,0 , MPI_COMM_WORLD);

    thread_from =(int *)malloc(nranks * sizeof(int));
    thread_length = (int *)malloc(nranks * sizeof(int));
    make_parts(thread_from, thread_length, nranks, length);

    int max = thread_length[0];
    for (int i = 0; i < nranks; ++i) {
        if (thread_length[i] > max) {
            max = thread_length[i];
         }
    }
    arr1 = (int *)malloc(max * sizeof(int));

    MPI_Scatterv(arr, thread_length, thread_from, MPI_INT,arr1 , thread_length[rank], MPI_INT, 0,MPI_COMM_WORLD);
    buffer = (int *)malloc(length * sizeof(int));
    free(arr);
    arr = NULL;
    merge_sort(arr1, buffer, thread_length[rank], 0);

    MPI_Gatherv(arr1,  thread_length[rank],  MPI_INT, buffer, thread_length, thread_from, MPI_INT, 0, MPI_COMM_WORLD);
    free(arr1);
    arr1 = NULL;

    if (rank == 0){
       buffer1 = (int *)malloc(length * sizeof(int));
       merge_sorted_after_multithreading(buffer, buffer1, thread_from, thread_length, nranks, 0);
       cout<< "result:"<< endl;
       if  (length <10000) output(buffer1,length);
       else cout << "the array is well sorted but too big to display"<< endl;
    }
   MPI_Finalize();
   return 0;
}
