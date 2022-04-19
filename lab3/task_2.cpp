#include <iostream>
#include <mpi.h>

using namespace std;

void multiplier(int **A, int **B, int **c, int row1, int col1, int col2) {
        for (int i = 0; i < row1; i++)
                for (int j = 0; j < col2; j++)
                        for (int x = 0; x < col1; x++)
                                c[i][j] +=  A[i][x] * B[x][j];
}
void multiplier(int *A, int **B, int **c, int row1, int col1, int col2) {
        for (int i = 0; i < row1; i++)
                for (int j = 0; j < col2; j++)
                        for (int x = 0; x < col1; x++)
                                c[i][j] +=  A[x] * B[x][j];
}


void enter(int** Matrix, int rows, int cols) {
        for (int i = 0; i < rows; i++) {
                for (int j = 0; j < cols; j++) {
                        cout << "enter element" << "[" << i << "][" << j << "]" << endl;
                        cin >> Matrix[i][j];
                }
        }
}

void output(int** Matrix, int rows, int cols) {
        for (int i = 0; i < rows; i++) {
                for (int j = 0; j < cols; j++) {
                        cout << Matrix[i][j] << " ";
                }
                cout << endl;
        }
}

void generator(int** Matrix, int rows, int cols) {
        for (int i = 0; i < rows; i++) {
                for (int j = 0; j < cols; j++) {
                         Matrix[i][j] = rand() % 100 - 50;
                }
        }
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

int main() {
        int row1, row2, col1, col2, type;
        int** A;
        int** A1;
        int** result;
        int** buffer;
        int** B;
        int *thread_from;
        int *thread_length;
        MPI_Status status;
        int nranks, rank;
        MPI_Init(NULL, NULL);
        MPI_Comm_size(MPI_COMM_WORLD, &nranks);
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);
        if (rank == 0){
                cout << "if you want to enter a matrix enter 1 \n" <<
                        "if you want to generate a random matrix enter 2" << endl;
                cin >> type;

                cout << "enter the number of rows of the first matrix" << endl;
                cin >> row1;
                cout << "enter the number of columns of the first matrix" << endl;
                cin >> col1;

                cout << "enter the number of rows of the second matrix" << endl;
                cin >> row2;
                cout << "enter the number of columns of the second matrix" << endl;
                cin >> col2;

                if (col1 != row2) {
                        cout << "such matrices cannot be multiplied" << endl;
                        MPI_Finalize();
                        return 0;
                }
        }
        MPI_Bcast(&row1,1, MPI_INT ,0 , MPI_COMM_WORLD);
        MPI_Barrier(MPI_COMM_WORLD);
        MPI_Bcast(&col1,1, MPI_INT ,0 , MPI_COMM_WORLD);
        MPI_Barrier(MPI_COMM_WORLD);
        MPI_Bcast(&col2,1, MPI_INT ,0 , MPI_COMM_WORLD);
        MPI_Barrier(MPI_COMM_WORLD);
        A = (int **)malloc(row1*sizeof(int*));
        for (int i = 0; i < row1; i++)
                A[i] = (int*)malloc(col1*sizeof(int));
        A1 = (int **)malloc(row1*sizeof(int*));
        for (int i = 0; i < row1; i++)
                A1[i] = (int*)malloc(col1*sizeof(int));
        B = (int **)malloc(col1*sizeof(int*));
        for (int i = 0; i < col1; i++)
                B[i] = (int*)malloc(col2*sizeof(int));
        result = (int **)malloc(row1*sizeof(int*));
        for (int i = 0; i < row1; i++)
                result[i] = (int*)malloc(col2*sizeof(int));
        buffer = (int **)malloc(row1*sizeof(int*));
        for (int i = 0; i < row1; i++)
                buffer[i] = (int*)malloc(col2*sizeof(int));
        thread_from =(int *)malloc(nranks * sizeof(int));
        thread_length = (int *)malloc(nranks * sizeof(int));

        if (rank == 0){
               if (type == 1) {
                       cout << "enter the first matrix" << endl;
                       enter(A, row1, col1);
               }
               else generator(A, row1, col1);
               cout << "the first matrix" << endl;
               output(A, row1, col1);
               if (type == 1) {
                       cout << "enter the second matrix" << endl;
                       enter(B, col1, col2);
               }
               else generator(B, col1, col2);
               cout << "the second matrix" << endl;
               output(B, col1, col2);
               make_parts(thread_from, thread_length, nranks, row1);
         }
         MPI_Barrier(MPI_COMM_WORLD);
         for (int i = 0; i < col1;i++){
         MPI_Bcast(B[i], col2, MPI_INT ,0 , MPI_COMM_WORLD);
         MPI_Barrier(MPI_COMM_WORLD);
         }
         MPI_Bcast(thread_from, nranks, MPI_INT ,0 , MPI_COMM_WORLD);
         MPI_Barrier(MPI_COMM_WORLD);
         MPI_Bcast(thread_length, nranks, MPI_INT ,0 , MPI_COMM_WORLD);
         MPI_Barrier(MPI_COMM_WORLD);
         cout <<  "rank "<< rank<<" row1 "<<row1<<" col1 "<<col1<<" col2 "<<col2<<endl;
         cout <<  "rank "<< rank << "\nB\n";
         output(B,col1,col2);

         cout << nranks<<" "<<row1<<endl;
         cout << " thread_from ";
         for ( int i = 0; i<nranks;i++)cout << thread_from[i];
         cout<< endl;
         cout<< " thread_length ";
         for ( int i = 0; i<nranks;i++)cout << thread_length[i];
         cout<<endl;

         for (int i = 0; i<nranks;i++){
                MPI_Scatterv(A, thread_length, thread_from, MPI_INT,A1[thread_from[rank]], row1*col1, MPI_INT, 0,MPI_COMM_WORLD);
         }
         for (int i = 0; i<row1; i++)
                free(A[i]);
         free(A);
         output(A1,thread_length[rank],col1);
          multiplier(A1[thread_from[rank]], B, result, thread_length[rank], col1, col2);
          MPI_Gatherv(result[thread_from[rank]],  thread_length[rank],  MPI_INT, buffer, thread_length, thread_from, MPI_INT, 0, MPI_COMM_WORLD);

          if (rank == 0){
                cout << "result:" << endl;
                output(buffer, row1, col2);
          }

          //for (int i = 0; i < row1; i++) delete[]A[i]; delete[]A;
          //for (int i = 0; i < row2; i++) delete[]B[i]; delete[]B;
          //for (int i = 0; i < row1; i++) delete[]result[i]; delete[]result;
          MPI_Finalize();
          return 0;
}
