#include <mpi.h>
#include <iostream>

using namespace std;

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size != 4) {
        if (rank == 0) {
            cout << "Ошибка: программа должна запускаться на 4 процессах!" << endl;
        }
        MPI_Finalize();
        return 1;
    }

    int ai = rank * 2 + 1;  // a0 = 1, a1 = 3, a2 = 5, a3 = 7
    int bi = rank * 3 + 2;  // b0 = 2, b1 = 5, b2 = 8, b3 = 11
    int received_a = -1, received_b = -1;

    cout << "Процесс " << rank << ": a" << rank << " = " << ai << ", b" << rank << " = " << bi << endl;

    int send_to[] = {2, 0, 3, 1};    // Куда отправляем ai
    int recv_from_a[] = {1, 3, 0, 2}; // Откуда получаем ai
    int send_to_b[] = {1, 3, 0, 2};   // Куда отправляем bi
    int recv_from_b[] = {2, 0, 3, 1}; // Откуда получаем bi

    MPI_Status status;

    // --- Обмен ai ---
    if (rank % 2 == 0) {
        MPI_Recv(&received_a, 1, MPI_INT, recv_from_a[rank], 0, MPI_COMM_WORLD, &status);
        MPI_Send(&ai, 1, MPI_INT, send_to[rank], 0, MPI_COMM_WORLD);
    } else {
        MPI_Send(&ai, 1, MPI_INT, send_to[rank], 0, MPI_COMM_WORLD);
        MPI_Recv(&received_a, 1, MPI_INT, recv_from_a[rank], 0, MPI_COMM_WORLD, &status);
    }

    if (rank % 2 == 0) {
        MPI_Recv(&received_b, 1, MPI_INT, recv_from_b[rank], 1, MPI_COMM_WORLD, &status);
        MPI_Send(&bi, 1, MPI_INT, send_to_b[rank], 1, MPI_COMM_WORLD);
    } else {
        MPI_Send(&bi, 1, MPI_INT, send_to_b[rank], 1, MPI_COMM_WORLD);
        MPI_Recv(&received_b, 1, MPI_INT, recv_from_b[rank], 1, MPI_COMM_WORLD, &status);
    }

    int ci = received_a + received_b;
    cout << "Процесс " << rank << ": Исходные данные: a = " << ai << ", b = " << bi
    << "; Полученные данные: a = " << received_a << ", b = " << received_b << "; c" << rank << " = " << ci << endl;

    MPI_Finalize();
    return 0;
}