#include <omp.h>
#include <iostream>
#include <chrono>
#include "matrix.h"
using namespace std;
using namespace std::chrono;

// Экспериментировал -- Ошлаков Данил, ИВТ-22
int main() {
    //Установка числа используемых потоков
    omp_set_num_threads(8);
    vector<vector<int>> matrix1 = MatrixOperations<int>::generateRandomMatrix(1000, 1000, -10, 10);
    vector<vector<int>> matrix2 = MatrixOperations<int>::generateRandomMatrix(1000, 1000, -10, 10);


    auto start = high_resolution_clock::now();
    vector<vector<int>> matrix_prod1 = MatrixOperations<int>::multiplyMatrices(matrix1, matrix2);
    auto end = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(end - start);
    cout << "Время вычисления одним потоком: " << duration.count() << " миллисекунд." << endl;
    start = high_resolution_clock::now();
    vector<vector<int>> matrix_prod2 = MatrixOperations<int>::multiplyMatricesParallel(matrix1, matrix2);
    end = high_resolution_clock::now();
    duration = duration_cast<milliseconds>(end - start);
    cout << "Время вычисления при помощи потоков: " << duration.count() << " миллисекунд." << endl;
    return 0;
}