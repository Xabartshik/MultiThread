#include <omp.h>
//Для активации omp.h необходимо добавить галочку в настройках проекта, тем самым подключив еще и библиотеку
#include <iostream>
#include <chrono>
#include "matrix.h"
using namespace std;
using namespace std::chrono;

// Экспериментировал -- Ошлаков Данил, ИВТ-22
int main() {
    vector<long long> sequentialTimes;
    vector<long long> parallelTimes;

    omp_set_num_threads(8); // Установка числа потоков
    vector<int> matrixSizes = {500, 600, 700, 800, 900, 1000, 1100, 1200, 1300, 1400, 1500, 1750, 2000 };
    for (int size : matrixSizes) {
        cout << "Testing matrix size: " << size << "x" << size << endl;

        // Генерация случайных матриц
        auto matrix1 = MatrixOperations<int>::generateRandomMatrix(size, size, -10, 10);
        auto matrix2 = MatrixOperations<int>::generateRandomMatrix(size, size, -10, 10);

        // Последовательное умножение
        auto start = high_resolution_clock::now();
        auto matrix_prod1 = MatrixOperations<int>::multiplyMatrices(matrix1, matrix2);
        auto end = high_resolution_clock::now();
        auto duration = duration_cast<milliseconds>(end - start);
        sequentialTimes.push_back(duration.count());

        // Параллельное умножение
        start = high_resolution_clock::now();
        auto matrix_prod2 = MatrixOperations<int>::multiplyMatricesParallel(matrix1, matrix2);
        end = high_resolution_clock::now();
        duration = duration_cast<milliseconds>(end - start);
        parallelTimes.push_back(duration.count());

        cout << "Sequential time: " << sequentialTimes.back() << " ms" << endl;
        cout << "Parallel time: " << parallelTimes.back() << " ms" << endl;
        cout << "----------------------------" << endl;
    }

    // Вывод результатов
    cout << "\nResults:" << endl;
    cout << "Sizes: [";
    for (int size : matrixSizes) cout << size << (size != matrixSizes.back() ? ", " : "");
    cout << "]" << endl;

    cout << "Sequential times: [";
    for (long long time : sequentialTimes) cout << time << (time != sequentialTimes.back() ? ", " : "");
    cout << "]" << endl;

    cout << "Parallel times: [";
    for (long long time : parallelTimes) cout << time << (time != parallelTimes.back() ? ", " : "");
    cout << "]" << endl;
}