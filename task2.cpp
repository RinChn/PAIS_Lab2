#include <mpi.h>
#include <iostream>
#include <vector>
#include <cstdlib>

using namespace std;

// Функция для генерации случайного вектора
vector<int> generateVector(int size) {
    vector<int> vec(size);
    for (int &val : vec) {
        val = rand() % 21 - 10; // Генерация чисел от -10 до 10
    }
    return vec;
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    const int VECTOR_SIZE = 20;
    vector<int> vec;

    if (rank == 0) {
        vec = generateVector(VECTOR_SIZE);
        cout << "Исходный вектор: ";
        for (int v : vec) cout << v << " ";
        cout << endl;
    }

    // Рассылка данных
    vector<int> local_vec(VECTOR_SIZE / size);
    MPI_Scatter(vec.data(), VECTOR_SIZE / size, MPI_INT, local_vec.data(), VECTOR_SIZE / size, MPI_INT, 0, MPI_COMM_WORLD);

    // Сумма нечетных элементов кратных 3
    int local_sum_odd_3 = 0;
    for (int num : local_vec) {
        if (num % 2 != 0 && num % 3 == 0) {
            local_sum_odd_3 += num;
        }
    }

    int global_sum_odd_3;
    MPI_Reduce(&local_sum_odd_3, &global_sum_odd_3, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        cout << "Сумма нечетных элементов, кратных 3: " << global_sum_odd_3 << endl;
    }

    MPI_Finalize();
    return 0;
}