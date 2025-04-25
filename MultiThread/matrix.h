#pragma once
#include <vector>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <cassert>

using namespace std;

template <typename T>
class MatrixOperations {
public:
    // Функция для генерации случайной матрицы
    static vector<vector<T>> generateRandomMatrix(size_t rows, size_t cols, T minVal, T maxVal) {
        vector<vector<T>> matrix(rows, vector<T>(cols));
        srand(static_cast<unsigned int>(time(0))); // Инициализация генератора случайных чисел

        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < cols; ++j) {
                matrix[i][j] = minVal + static_cast<T>(rand()) / (static_cast<T>(RAND_MAX / (maxVal - minVal))); // Генерация случайного числа в диапазоне [minVal, maxVal]
            }
        }
        return matrix;
    }

    // Функция для создания матрицы вручную
    static vector<vector<T>> createMatrix(size_t rows, size_t cols) {
        vector<vector<T>> matrix(rows, vector<T>(cols));
        cout << "Введите элементы матрицы " << rows << "x" << cols << ":" << endl;
        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < cols; ++j) {
                cin >> matrix[i][j];
            }
        }
        return matrix;
    }
    //Перемножение матриц. Бросает исключение, если матрица не найдена
    static vector<vector<T>> multiplyMatrices(const vector<vector<T>>& matrix1, const vector<vector<T>>& matrix2) {
        size_t rows1 = matrix1.size();
        size_t cols1 = matrix1[0].size();
        size_t rows2 = matrix2.size();
        size_t cols2 = matrix2[0].size();

        if (cols1 != rows2) {
            throw invalid_argument("Размерности матриц не совпадают");
        }

        vector<vector<T>> result(rows1, vector<T>(cols2, 0));

        for (size_t i = 0; i < rows1; ++i) {
            for (size_t j = 0; j < cols2; ++j) {
                T sum = 0;
                for (size_t k = 0; k < cols1; ++k) {
                    sum += matrix1[i][k] * matrix2[k][j];
                }
                result[i][j] = sum;
            }
        }

        return result;
    }

    //Перемножение матриц несколькими потоками. Бросает исключение, если матрица не найдена
    static vector<vector<T>> multiplyMatricesParallel(const vector<vector<T>>& matrix1, const vector<vector<T>>& matrix2) {
        size_t rows1 = matrix1.size();
        size_t cols1 = matrix1[0].size();
        size_t rows2 = matrix2.size();
        size_t cols2 = matrix2[0].size();
        if (cols1 != rows2) {
            throw invalid_argument("Размерности матриц не совпадают");
        }

        vector<vector<T>> result(rows1, vector<T>(cols2, 0));
        //https://learn.microsoft.com/ru-ru/cpp/parallel/openmp/reference/openmp-directives?view=msvc-170. 
        // Приводит к тому, что работа в цикле for внутри параллельного региона будет разделена между потоками.
#pragma omp parallel for
//і: переменная индекса в операторе For директивы ОрепМР должна иметь тип целого со знаком
        for (long long int i = 0; i < rows1; ++i) {
            for (long long int j = 0; j < cols2; ++j) {
                T sum = 0;
                for (long long int k = 0; k < cols1; ++k) {
                    sum += matrix1[i][k] * matrix2[k][j];
                }
                result[i][j] = sum;
            }
        }

        return result;
    }

    // Функция для вывода матрицы
    static void printMatrix(const vector<vector<T>>& matrix) {
        for (const auto& row : matrix) {
            for (const T& elem : row) {
                cout << elem << " ";
            }
            cout << endl;
        }
    }

};

// Функция тестирования
void testMultiplication() {
    // Тест 1: Маленькие матрицы
    vector<vector<int>> m1 = { {1, 2}, {3, 4} };
    vector<vector<int>> m2 = { {5, 6}, {7, 8} };
    vector<vector<int>> expected = { {19, 22}, {43, 50} };

    auto seq_result = MatrixOperations<int>::multiplyMatrices(m1, m2);
    auto par_result = MatrixOperations<int>::multiplyMatricesParallel(m1, m2);

    assert(seq_result == expected && "Sequential multiplication failed");
    assert(par_result == expected && "Parallel multiplication failed");

    // Тест 2: Неквадратные матрицы
    vector<vector<double>> m3 = { {1.0, 2.0, 3.0}, {4.0, 5.0, 6.0} };
    vector<vector<double>> m4 = { {7.0, 8.0}, {9.0, 10.0}, {11.0, 12.0} };
    vector<vector<double>> expected2 = { {58.0, 64.0}, {139.0, 154.0} };

    auto seq_result2 = MatrixOperations<double>::multiplyMatrices(m3, m4);
    auto par_result2 = MatrixOperations<double>::multiplyMatricesParallel(m3, m4);

    assert(seq_result2.size() == expected2.size() && "Size mismatch");
    for (size_t i = 0; i < seq_result2.size(); ++i) {
        for (size_t j = 0; j < seq_result2[0].size(); ++j) {
            assert(abs(seq_result2[i][j] - expected2[i][j]) < 1e-9 && "Sequential multiplication failed");
            assert(abs(par_result2[i][j] - expected2[i][j]) < 1e-9 && "Parallel multiplication failed");
        }
    }

    cout << "All tests passed successfully!\n";
}