#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <cassert>
#include <chrono>

using namespace std;

void pancakeSort(vector<int>& data) {
    if (data.size() > 1) {
        for (int size = data.size(); size > 1; --size) {
            // Позиция максимума в неотсортированной части
            auto maxindex = max_element(data.begin(), data.begin() + size) - data.begin(); //O(n)
            if (maxindex + 1 != size) {
                // Если максимум не на своем месте, то нужно развернуть
                if (maxindex != 0) {
                    // Переворачиваем так, чтобы максимум оказался слева
                    reverse(data.begin(), data.begin() + maxindex + 1); //O(n)
                }
                // Переворачиваем неотсортированную часть массива,
                // максимум становится на своё место
                reverse(data.begin(), data.begin() + size);
            }
        }
    }
} // O(n^2)



int partition(vector<int>& arr, int low, int high) {
    int pivot = arr[high]; // Выбираем последний элемент в качестве опорного
    int i = low - 1; // Индекс меньшего элемента

    for (int j = low; j < high; ++j) { // Проходим по всем элементам от low до high-1
        if (arr[j] < pivot) { // Если текущий элемент меньше опорного
            ++i; // Увеличиваем индекс меньшего элемента
            swap(arr[i], arr[j]); // Меняем местами элементы
        }
    }
    swap(arr[i + 1], arr[high]); // Перемещаем опорный элемент на правильную позицию
    return i + 1; // Возвращаем индекс опорного элемента
} //O(n)

void quickSort(vector<int>& arr, int low, int high) {
    if (low >= high) return; // Базовый случай: если диапазон не содержит элементов

    int pi = partition(arr, low, high); // Разделяем массив

    quickSort(arr, low, pi - 1); // Рекурсивно сортируем левую часть
    quickSort(arr, pi + 1, high); // Рекурсивно сортируем правую часть
} // худший случай O(n^2), средний случай O(n * log n)



void flashSort(vector<int> &arr) { // O(n)

    int n = arr.size(); // Размер массива, n - 4 байта
    if (n <= 1) return; // Если массив пуст или содержит один элемент, ничего не делаем.

    int m = int(0.45 * n); // Количество классов, m - около 45% от размера массива, m - 4 байта, количество классов
    vector<int> count(m, 0); // Вектор для хранения количества элементов в каждом классе, O(m)

    int min_el = arr[0], max_el = arr[0]; // Инициализируем минимальное и максимальное значения, min_el, max_el - 8 байт
    int max_idx = 0; // Индекс максимального элемента, max_idx - 4 байта

    for (int i = 1; i < n; i++) { // O(n), i - 4 байта
        if (arr[i] < min_el) min_el = arr[i]; // Находим минимум.
        if (arr[i] > max_el) {
            max_el = arr[i]; // Находим максимум.
            max_idx = i; // Запоминаем индекс максимального элемента.
        }
    }

    if (min_el == max_el) return; // Если все элементы одинаковы, массив уже отсортирован.

    double scale = double(m - 1) / (max_el - min_el); // Вычисляем масштаб для распределения элементов по классам, scale - 8 байт
    for (int i = 0; i < n; i++) { // O(n), i - 4 байта
        int idx = int((arr[i] - min_el) * scale); // Вычисляем индекс класса для текущего элемента, idx - 4 байта
        if (idx >= m) idx = m - 1; // Защита от выхода за границы
        count[idx]++; // Увеличиваем счетчик для соответствующего класса.
    }

    
    for (int i = 1; i < m; i++) count[i] += count[i - 1]; // Преобразуем массив счетчиков в массив индексов, O(m)

    vector<int> temp(n); // Временный массив для хранения отсортированных элементов, O(n)
    for (int i = n - 1; i >= 0; i--) { // Проходим с конца, чтобы сохранить стабильность при равных значениях, O(n)
        int idx = int((arr[i] - min_el) * scale);
        if (idx >= m) idx = m - 1;
        temp[--count[idx]] = arr[i]; // Ставим элемент на правильное место в временном массиве
    }

    for (int i = 0; i < n; i++) arr[i] = temp[i]; // Копируем отсортированные элементы обратно в оригинальный массив, O(n)
    
    int start = 0; // start - 4 байта, O(n log n) в худшем случае
    for (int i = 0; i < m; i++) { // i - 4 байта
        int end = count[i]; // Получаем индекс конца текущего класса, end - 4 байта
        if (end - start > 1) {
            sort(arr.begin() + start, arr.begin() + end); // Если в классе больше одного элемента, сортируем его стандартным методом, O(n log n)
        }
        start = end; // Обновляем начальный индекс для следующего класса.
    }

} // память O(n + m), сложность O(n) в среднем, O(n^2) в худшем случае

void testSortingAlgorithms() {
    {
        vector<int> original = {10, 7, -8, 9, -1, 5};
        vector<int> expected = original;
        sort(expected.begin(), expected.end());

        vector<int> cakeSorted = original;
        pancakeSort(cakeSorted);
        assert(cakeSorted == expected);

        vector<int> quickSorted = original;
        quickSort(quickSorted, 0, quickSorted.size() - 1);
        assert(quickSorted == expected);

        vector<int> flashSorted = original;
        flashSort(flashSorted);
        assert(flashSorted == expected);
    }
    {
        vector<int> original = {};
        vector<int> expected = original;

        vector<int> cakeSorted = original;
        pancakeSort(cakeSorted);
        assert(cakeSorted == expected);

        vector<int> quickSorted = original;
        quickSort(quickSorted, 0, quickSorted.size() - 1);
        assert(quickSorted == expected);

        vector<int> flashSorted = original;
        flashSort(flashSorted);
        assert(flashSorted == expected);
    }
    {
        vector<int> original = {42};
        vector<int> expected = original;

        vector<int> cakeSorted = original;
        pancakeSort(cakeSorted);
        assert(cakeSorted == expected);

        vector<int> quickSorted = original;
        quickSort(quickSorted, 0, quickSorted.size() - 1);
        assert(quickSorted == expected);

        vector<int> flashSorted = original;
        flashSort(flashSorted);
        assert(flashSorted == expected);
    }
    {
        vector<int> original = {5, 3, 8, 3, 9, 5, 3};
        vector<int> expected = original;
        sort(expected.begin(), expected.end());

        vector<int> cakeSorted = original;
        pancakeSort(cakeSorted);
        assert(cakeSorted== expected);

        vector<int> quickSorted = original;
        quickSort(quickSorted, 0, quickSorted.size() - 1);
        assert(quickSorted == expected);

        vector<int> flashSorted = original;
        flashSort(flashSorted);
        assert(flashSorted == expected);
    }
    {
        vector<int> original = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
        vector<int> expected = original;

        vector<int> cakeSorted = original;
        pancakeSort(cakeSorted);
        assert(cakeSorted== expected);

        vector<int> quickSorted = original;
        quickSort(quickSorted, 0, quickSorted.size() - 1);
        assert(quickSorted == expected);

        vector<int> flashSorted = original;
        flashSort(flashSorted);
        assert(flashSorted == expected);
    }
    {
        vector<int> original;

        for(int i = 10000; i > -1; i--){
            original.push_back(i);
        }

        vector<int> expected;

        for(int i = 0; i < 10001; i++){
            expected.push_back(i);
        }
        
        vector<int> cakeSorted = original;
        pancakeSort(cakeSorted);
        assert(cakeSorted == expected);

        vector<int> quickSorted = original;
        quickSort(quickSorted, 0, quickSorted.size() - 1);
        assert(quickSorted == expected);

        vector<int> flashSorted = original;
        flashSort(flashSorted);
        assert(flashSorted == expected);
    }
}

//int main() { testSortingAlgorithms();}