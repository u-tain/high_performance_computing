// в изначальной версии процессы запускаются не по порядку т.к. они независимые и мы не прописали не какой логики
#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {
    int nranks;
    int rank;
    int name_len;
    double buf[10]; //буфер
    MPI_Status status;
    char processor_name[MPI_MAX_PROCESSOR_NAME];
    MPI_Init(NULL, NULL); // логи интепретируются как открывающиеся скобки
    // те процессы которые запускаются интерпретируются номером т.е. rank ( = от 0 до кол-ва процессов)
    // их всего: nranks
    // процессы запускаются в рамках какой-то группы (коммутатора)
    MPI_Comm_size(MPI_COMM_WORLD, &nranks); //узнаем сколько всего процессов в конкретной группе
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); // знаем какой у меня rank  в конкретной группе
    MPI_Get_processor_name(processor_name, &name_len); // узнаем где прогррамма физически запущена 
    if (rank==0) { // привязка к конкретному процессу
        printf('root\n");
               // отправка сообщения MPI_Send(const void *buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm)
               // const void *buf - буфер
               // int count - количество отправляемых элементвом, < размера буфера
               // MPI_Datatype datatype - обертка над традиционными типами данных, бепем обертку на тем который исп 
               // int dest - получатель, какой-то конкретный процесс
               // int tag - позволяет отличить одно открытое соединение от другого. Адрес подписки
               // MPI_Comm comm - группа с которой взаимодействуем
               MPI_Send(buf, 3, MPI_DOUBLE, 1, 1, MPI_COMM_WORLD);
    }
    // барьер, зная общее кол-во процессов в группе, работает как счетчик. 
    // процесссы пирходят по одному к барьеру, он сравнивает их кол-во с общим кол-вом процессов и они проходят дальше, когла соберутся все.
    // MPI_Barrier(MPI_COMM_WORLD);
    else {
        // получение сообщения
        MPI_Recv(buf, 3,  MPI_DOUBLEб 0, 1, MPI_COMM_WORLD, &status)
    }
    printf("Hello world from node %s, rank [%d/%d]\n", processor_name, rank, nr>
    MPI_Finalize(); // логи интепретируются как  закрывающиеся  скобки
        return 0;
}
