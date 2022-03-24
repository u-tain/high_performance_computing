// в изначальной версии процессы запускаются не по порядку т.к. они независимые и мы не прописали не какой логики
#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {
    int nranks;
    int rank;
    int name_len;
    char processor_name[MPI_MAX_PROCESSOR_NAME];
    MPI_Init(NULL, NULL); // логи интепретируются как открывающиеся скобки
    // те процессы которые запускаются интерпретируются номером т.е. rank ( = от 0 до кол-ва процессов)
    // их всего: nranks
    // процессы запускаются в рамках какой-то группы (коммутатора)
    MPI_Comm_size(MPI_COMM_WORLD, &nranks); //узнаем сколько всего процессов в конкретной группе
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); // знаем какой у меня rank  в конкретной группе
    MPI_Get_processor_name(processor_name, &name_len); // узнаем где прогррамма физически запущена 
    printf("Hello world from node %s, rank [%d/%d]\n", processor_name, rank, nr>
    MPI_Finalize(); // логи интепретируются как  закрывающиеся  скобки
        return 0;
}
