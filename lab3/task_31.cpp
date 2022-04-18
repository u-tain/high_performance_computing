#include <mpi.h>
#include <iostream>
using namespace std;

void generate_int_array(int *arr, int length){
  for (int i = 0; i < length; ++i)
    arr[i] = rand() % 50;
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

int main(){
    int length, type, nranks, rank, name_len;
    char processor_name[MPI_MAX_PROCESSOR_NAME];
    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &nranks);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0){
      cout << "if you want to enter an array enter 1 \nif you want to generate a random array enter 2" << endl;
      cin >> type;
      cout << "enter the size of the array"<<endl;
      cin >> length;
      int arr[length];
      if (type == 1) {
        int elem = 0;
        for (int i = 0; i < length; ++i){
            cout << "enter " << i <<"-element"<<endl;
            cin >> elem;
                    arr[i] = elem;
       }
     }
     else generate_int_array(arr, length);
     output(arr);
     }
     MPI_Bcast(arr[0], length, MPI_INT , 0, MPI_COMM_WORLD);
     int thread_from[nranks];
     int thread_length[nranks];
     int buffer[length];
     make_parts(thread_from, thread_length, nranks, length);
  
     merge_sort(arr, buffer, thread_length[rank], thread_from[rank]);
     MPI_Get_processor_name(processor_name, &name_len);
     cout<< "node: " << processor_name << " rank: "<< rank <<" indices: "<< thread_from[rank]<<endl;
     MPI_Barrier(MPI_COMM_WORLD);
     MPI_Gatherv(arr[0], length,  MPI_INT, buffer[0], thread_length, thread_from, MPI_INT, 0, MPI_COMM_WORLD);
  
     
     if (rank == 0){
       int buffer1[length];
       merge_sorted_after_multithreading(buffer, buffer1, thread_from, thread_length, nranks, 0);
       cout<< "result:"<< endl;
       output(buffer1,length);
     }
   MPI_Finalize();
   return 0;
}
