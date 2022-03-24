// запуск программы:
// в изначальной версии процессы запускаются не по порядку т.к. они независимые и мы не прописали не какой логики
#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {
    int nranks;
    int rank;
    int name_len;
    const int TAG_DOUBLE = 1;
    const int TAG_FLOAT = 2;
    double buf[10]; //буфер
    buf[0] = 1;
    float fbuf[10];
    MPI_Status status;
    char processor_name[MPI_MAX_PROCESSOR_NAME];
    MPI_Init(NULL, NULL); // логи интепретируются как открывающиеся скобки
    // те процессы которые запускаются интерпретируются номером т.е. rank ( = от 0 до кол-ва процессов)
    // их всего: nranks
    // процессы запускаются в рамках какой-то группы (коммутатора)
    // MPI_Comm_size(MPI_COMM_WORLD, &nranks); //узнаем сколько всего процессов в конкретной группе
    // MPI_Comm_rank(MPI_COMM_WORLD, &rank); // знаем какой у меня rank  в конкретной группе
    // MPI_Get_processor_name(processor_name, &name_len); // узнаем где прогррамма физически запущена 
    if (rank==0) { // привязка к конкретному процессу
        buf[0] = 123.45;
        printf('root\n");
               // отправка сообщения MPI_Send(const void *buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm)
               // const void *buf - буфер
               // int count - количество отправляемых элементвом, < размера буфера
               // MPI_Datatype datatype - обертка над традиционными типами данных, бепем обертку на тем который исп 
               // int dest - получатель, какой-то конкретный процесс
               // int tag - позволяет отличить одно открытое соединение от другого. Адрес подписки
               // MPI_Comm comm - группа с которой взаимодействуем
               // MPI_Send(buf, 3, MPI_DOUBLE, 1, TAG_DOUBLE, MPI_COMM_WORLD);
               // MPI_Send(fbuf, 4, MPI_FLOAT, 1, TAG_FLOAT, MPI_COMM_WORLD);
    }
    // барьер, зная общее кол-во процессов в группе, работает как счетчик. 
    // процесссы пирходят по одному к барьеру, он сравнивает их кол-во с общим кол-вом процессов и они проходят дальше, когла соберутся все.
    // MPI_Barrier(MPI_COMM_WORLD);
    else {
        // получение сообщения
        MPI_Recv(fbuf, 4,  MPI_FLOAT, 0, TAG_FLOAT, MPI_COMM_WORLD, &status);
        printf(buf[0]);
        MPI_Recv(buf, 3,  MPI_DOUBLE, 0, TAG_DOUBLE, MPI_COMM_WORLD, &status);
        printf(buf[0]);
    }
    // передаем информацию всем процессам, кроме отправителя
    MPI_Bcast(buf, 3,MPI_DOUBLE, 0, MPI_COMM_WORLD);        
    printf("Hello world from node %f, rank [%d/%d]\n", processor_name, rank, nr, buf[0]>
    MPI_Finalize(); // логи интепретируются как  закрывающиеся  скобки
    return 0;
}
