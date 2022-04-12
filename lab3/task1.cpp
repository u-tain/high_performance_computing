#include <mpi.h>
#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <cmath>

using namespace std;

int main(int argc, char** argv) {
    int nranks, rank, name_len, N, z;
    double sum = 0;
    double pi = 0;
    double x = 0;
    int n = 0;

    char processor_name[MPI_MAX_PROCESSOR_NAME];
    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &nranks);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0){
cout << "введите точность" << endl;
        cin >> N;
    }
    z = N;
    while(z > 0) {
        n++;
        z /= 10;
    }
    MPI_Bcast(&N, 1, MPI_INT, 0, MPI_COMM_WORLD);
    for (int i = rank; i < N; i += nranks) {
        x = (i+0.5)/N;
        sum += 4/(1 + x*x);
    }
    sum = sum / N;
    MPI_Reduce(&sum, &pi, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    MPI_Get_processor_name(processor_name, &name_len);
    if(rank ==0){
       cout << "результат: " << fixed << setprecision(n-1)<< pi << endl;
    }
    MPI_Finalize();
    return 0;
