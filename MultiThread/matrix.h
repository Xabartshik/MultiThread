#pragma once
#include <vector>
#include <cstdlib>
#include <ctime>
#include <iostream>

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
            for (long long int  j = 0; j < cols2; ++j) {
                T sum = 0;
                for (long long int  k = 0; k < cols1; ++k) {
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