#include <chrono>
#include <fstream>
#include <iostream>
#include <random>
#include <vector>

#include "lab5.cpp"

// Функция для выполнения бенчмаркинга сортировки
void benchmark(void (*sortFunc)(std::vector<int>&), const std::string& sortName, int maxSize, int step,
               std::ofstream& outFile) {
    // Создаем генератор случайных чисел
    std::random_device rd; // Источник истинной случайности (если доступен)
    std::mt19937 gen(rd()); // Генератор Mersenne Twister
    std::uniform_int_distribution<> dis(1, 100000); // Распределение от 1 до 100000

    // Цикл по размерам массива от 5000 до maxSize с заданным шагом
    for (int size = 5000; size <= maxSize; size += step) {
        std::vector<int> arr(size); // Создаем вектор целых чисел заданного размера

        // Заполняем вектор случайными числами
        for (int& x : arr) {
            x = dis(gen); // Генерируем случайное число и присваиваем элементу вектора
        }

        // Замеряем время начала сортировки
        auto start = std::chrono::high_resolution_clock::now();
        sortFunc(arr); // Вызываем функцию сортировки
        // Замеряем время окончания сортировки
        auto end = std::chrono::high_resolution_clock::now();

        // Вычисляем продолжительность сортировки
        std::chrono::duration<double> duration = end - start;
        
        // Записываем результаты в файл и выводим на консоль
        outFile << sortName << "," << size << "," << duration.count() << "\n";
        std::cout << sortName << " - Size: " << size << " - Time: " << duration.count() << "s\n";
    }
}

// Главная функция программы
int main() {
    int maxSize = 100000; // Максимальный размер массива для тестирования
    int step = 5000;      // Шаг увеличения размера массива

    std::ofstream outFile("benchmark.csv"); // Открываем файл для записи результатов
    outFile << "Algorithm,Size,Time\n"; // Записываем заголовки в файл

    // Запускаем бенчмаркинг для различных алгоритмов сортировки
    benchmark(pancakeSort, "Pancake Sort", maxSize, step, outFile);
    benchmark([](std::vector<int>& arr) { quickSort(arr, 0, arr.size() - 1); }, "Quick Sort", maxSize, step,
               outFile);
    benchmark(flashSort, "Flash Sort", maxSize, step, outFile);

    outFile.close(); // Закрываем файл после завершения всех бенчмарков
    return 0; // Завершаем программу
}