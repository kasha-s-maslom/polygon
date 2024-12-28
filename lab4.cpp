#include <iostream>
#include <vector>
#include <math.h>

std::vector<std::vector<char>> generate_all_masks(const int arr[], const char n_clusters = 2) {
    int size = sizeof(arr) / sizeof(arr[0]);  // 40 / 4 == 10

    if (size > 20) {
        throw "Max size is 20";
    }
    
    // Входные данные
    std::vector<std::vector<char>> all_masks;
    std::vector<char> mask(size, 0);

    for (int i = 0; i < std::pow(n_clusters, size); i++) {
        std::vector<char> mask_to_add(mask);
        all_masks.push_back(mask_to_add);

        for (int j = size - 1; j > -1; j--) {
            if (mask[j] == 1) {
                mask[j] = 0;
            } else if (mask[j] == 0) {
                mask[j] = 1;
                break;
            }
        }

    }

    return all_masks;
}

float calculate_cluster_metric(std::vector<int> cluster) {
    float mean = 0;
    // Тут сами посчитаете mean
    for(int i = 0; i < cluster.size(); i++) {
        mean += cluster[i];
    }
    mean = (float)mean/cluster.size();

    float rate = 0;
    for(int i = 0; i < cluster.size(); i++) {
        rate = rate + std::abs(cluster[i] - mean);
    }

    return rate;
}

float calculate_metric(std::vector<int> cluster_1, std::vector<int> cluster_2) {
    return calculate_cluster_metric(cluster_1) + calculate_cluster_metric(cluster_2);
}

int main() {

    // Входные данные
    int arr[] = {1, 2, 3, 9};

    // Разбить на кластеры
    std::vector<std::vector<char>> masks = generate_all_masks(arr);

    // Посчитать метрики для разных разбиений
    for (int i = 0; i < masks.size(); i++) {
        std::vector<char> mask = masks[i]; // 0,0,0

        std::vector<int> cluster1;
        std::vector<int> cluster2;

        for(int j = 0; j < mask.size(); j++) {
            if (mask[j] == 0) {
                cluster1.push_back(arr[j]);
            } else {
                cluster2.push_back(arr[j]);
            }
        }

        float rate = calculate_metric(cluster1, cluster2);

        //
    }

    // Выбрать лучшее разбиение

    return 0;
}