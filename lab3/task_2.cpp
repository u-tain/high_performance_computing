#include <iostream>
#include <mpi.h>

using namespace std;

void multiplier(int **A, int **B, int **c, int row1, int col1, int col2) {
        MPI_Bcast(&row1,1,MPI_INT,0,MPI_COMM_WORLD); 
        for (int i = 0; i < row1; i++)
                for (int j = 0; j < col2; j++)
                        for (int x = 0; x < col1; x++)
                                c[i][j] +=  A[i][x] * B[x][j];
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

int main() {
        int row1, row2, col1, col2, type;
        int** A;
        int** B;
        int nranks, rank
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
        }
        if (col1 != row2) {
                cout << "such matrices cannot be multiplied" << endl;
                MPI_Finalize();
                return 0;
        }
        else {
                A = new int* [row1];
                for (int i = 0; i < row1; i++)
                        A[i] = new int[col1];
                if (type == 1) {
                        cout << "enter the first matrix" << endl;
                        enter(A, row1, col1);
                }
                else generator(A, row1, col1);
                cout << "the first matrix" << endl;
                output(A, row1, col1);

                B = new int* [row2];
                for (int i = 0; i < row2; i++)
                        B[i] = new int[col2];
                if (type == 1) {
                        cout << "enter the second matrix" << endl;
                        enter(B, row2, col2);
                }
                else generator(B, row2, col2);
                cout << "the second matrix" << endl;
                output(B, row2, col2);

                int** result = new int* [row1];
                for (int i = 0; i < row1; i++)
                        result[i] = new int[col2];

                multiplier(A, B, result, row1, col1, col2);
                
                if (rank == 0){
                        cout << "result:" << endl;
                        output(result, row1, col2);
                }
                
                for (int i = 0; i < row1; i++) delete[]A[i]; delete[]A;
                for (int i = 0; i < row2; i++) delete[]B[i]; delete[]B;
                for (int i = 0; i < row1; i++) delete[]result[i]; delete[]result;
                MPI_Finalize();
                return 0;
        }
}
