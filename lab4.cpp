#include <iostream>
#include <vector>

using namespace std;

void findSubarraysWithZeroSum(const vector<int>& arr, int K) {
    int N = arr.size();
    if (K > N) return; // Невозможно найти подмассив длиной K

    vector<int> indices(K);
    for (int i = 0; i < K; ++i) {
        indices[i] = i;
    }

    while (true) {
        // Вычисляем сумму текущего подмассива
        int sum = 0;
        for (int idx : indices) {
            sum += arr[idx];
        }

        // Если сумма равна нулю, выводим индексы
        if (sum == 0) {
            cout << "[";
            for (size_t i = 0; i < indices.size(); ++i) {
                cout << indices[i];
                if (i < indices.size() - 1) cout << ",";
            }
            cout << "]" << endl;
        }

        // Генерируем следующую комбинацию индексов
        int i;
        for (i = K - 1; i >= 0; --i) {
            if (indices[i] != N - K + i) {
                ++indices[i];
                for (int j = i + 1; j < K; ++j) {
                    indices[j] = indices[j - 1] + 1;
                }
                break;
            }
        }
        if (i < 0) break; // Все комбинации перебраны
    }
}

int main() {
    vector<int> array = {1, -1, -1, 2, 0, -2, 1, -1, 3, -3, 4, -4, 5, -5, 0, 6, -6, 7, -7, 8, -8, 9, -9, 10, -10};
    int K = 10;
    findSubarraysWithZeroSum(array, K);
    return 0;
}