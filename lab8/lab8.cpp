#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <map>
#include <set>
#include <cassert>
#include <limits>
#include <cmath>
#include <fstream>
#include <sstream>
#include <chrono>
#include <algorithm>

using namespace std;
using namespace chrono;

using Graph = map<int, vector<pair<int, double>>>; //O(V+E) memory; O(log(V)) для вставок, удаления и поиска по ключу
using Coordinates = map<int, pair<double, double>>; //O(V) память; O(log(V)) для вставок, удаления и поиска по ключу

void parseGraph(const string& filename, Graph& graph, Coordinates& coordinates) {
    ifstream file(filename); //O(1) memory; O(1) difficulty
    if (!file.is_open()) { //diff: O(1)
        cerr << "Error opening file: " << filename << endl;//O(1) both
        return; //O(1) difficulty
    }

    map<pair<double, double>, int> coordinateToId; memory: //создание строки O(k), k = len(str); difficulty O(n*k) - the worst(n - number of str in file)
    int nodeId = 0; //O(1) both

    auto getId = [&](double lon, double lat) { //O(1) both
        pair<double, double> coord = {lon, lat}; //O(1) both (fix value for both of double)
        if (!coordinateToId.count(coord)) { difficulty: //O(log(V))
            coordinateToId[coord] = nodeId; //mem: O(1), diff: O(log(V))
            coordinates[nodeId] = coord; //mem: O(1), diff: O(log(V))
            return nodeId++; //time: O(1)
        }
        return coordinateToId[coord]; //time: O(log(V))
    };

    string line; //both O(1)
    while (getline(file, line)) { //diff: O(V)
        size_t colon = line.find(":"); //mem: O(1); diff: O(k)
        if (colon == string::npos) continue; //diff: O(1)

        string parentNode = line.substr(0, colon); //both O(m), m - len of included str
        string children = line.substr(colon + 1); //same as previous

        double lon1, lat1; //both O(1)
        sscanf(parentNode.c_str(), "%lf,%lf", &lon1, &lat1); //time O(1)
        int parentId = getId(lon1, lat1); //mem: O(1), diff: O(n*N) the worst and O(log(V)) median

        stringstream ss(children); //mem: O(V), diff: O(1)
        string child; // both O(1)
        while (getline(ss, child, ';')) { //mem: O(k); diif: O(V), where's k = len(str), n = len of included str
            double lon2, lat2, weight; //both O(1)
            if (sscanf(child.c_str(), "%lf,%lf,%lf", &lon2, &lat2, &weight) == 3) { //time: O(1)
                int childId = getId(lon2, lat2); //mem: O(1); diff: O(log(V)+Log(V))
                graph[parentId].emplace_back(childId, weight); //both O(1)
                graph[childId].emplace_back(parentId, weight); //same
            }
        }
    }
}


int findNearestNode(const Coordinates& coordinates, double lon, double lat) {
    int nearestNode = -1; // both O(1)
    double minDistance = numeric_limits<double>::infinity(); //same

    for (const auto& [node, coord] : coordinates) { //O(V)
        double distance = sqrt(pow(coord.first - lon, 2) + pow(coord.second - lat, 2)); //Both O(1)
        if (distance < minDistance) { //time: O(1)
            minDistance = distance; //same
            nearestNode = node; //same
        }
    }
    return nearestNode; //same
}

double printPath(const map<int, int>& parent, int start, int goal, const Graph& graph) {
    vector<int> path; //both O(1)
    double weight = 0; //same
    for (int cur = goal; cur != start; cur = parent.at(cur)) { //time: O(V)
        path.push_back(cur); //diff: O(V) worst
        for (const auto& [neighbor, w] : graph.at(parent.at(cur))) //diff: O(Ev), where's Ev - number of edges from one node
            if (neighbor == cur) weight += w; //diff: O(1)
    }
    path.push_back(start); //diff: O(1)

    reverse(path.begin(), path.end()); //diff: O(p), wheres p - length of path
    cout << "\nTotal weight: " << weight << endl; //time: O(1)
    return weight; //same
}

double DFS(const Graph& graph, int start, int goal) {
    // Запоминаем время начала выполнения функции
    auto start_time = high_resolution_clock::now();

    stack<int> s; // Стек для хранения вершин (O(1))
    set<int> visited; // Множество для отслеживания посещенных вершин (O(1))
    map<int, int> parent; // Словарь для хранения родительских вершин (O(1))
    
    s.push(start); // Добавляем начальную вершину в стек (O(1))

    while (!s.empty()) { // Пока стек не пустой (O(V) в худшем случае)
        int current = s.top(); // Получаем верхнюю вершину стека (O(1))
        s.pop(); // Удаляем верхнюю вершину из стека (O(1))

        // Если текущая вершина уже посещена, пропускаем ее
        if (visited.count(current)) continue; // Проверка на наличие (O(log(V)))
        
        visited.insert(current); // Помечаем текущую вершину как посещенную (O(log(V)))

        // Проверяем, достигли ли мы целевой вершины
        if (current == goal) { // Проверка на равенство (O(1))
            auto end_time = high_resolution_clock::now(); // Запоминаем время окончания выполнения (O(1))
            printPath(parent, start, goal, graph); // Печатаем путь от start до goal (O(V), где N - длина пути)
            cout << "DFS Time: " << duration_cast<milliseconds>(end_time - start_time).count() << " ms\n"; // Вывод времени выполнения (O(1))
            return printPath(parent, start, goal, graph); // Возвращаем путь (O(V), где N - длина пути)
        }

        // Перебираем соседей текущей вершины
        for (const auto& [neighbor, _] : graph.at(current)) { // Проходим по всем соседям текущей вершины (O(V*Ev), где Ev - количество соседей)
            if (!visited.count(neighbor)) { // Проверяем, не посещен ли сосед (O(log(V)))
                s.push(neighbor); // Добавляем соседа в стек (O(1))
                parent[neighbor] = current; // Устанавливаем текущую вершину как родителя соседа (O(1), но вставка в map O(log(V)))
            }
        }
    }
    
    cout << "DFS: No path found.\n"; // Если путь не найден, выводим сообщение (O(1))
    return -1; // Возвращаем -1, если путь не найден
}

double BFS(const Graph& graph, int start, int goal) {
    // Запоминаем время начала выполнения функции
    auto start_time = high_resolution_clock::now();

    queue<int> q; // Очередь для хранения вершин (O(1))
    set<int> visited; // Множество для отслеживания посещенных вершин (O(1))
    map<int, int> parent; // Словарь для хранения родительских вершин (O(1))
    
    q.push(start); // Добавляем начальную вершину в очередь (O(1))

    while (!q.empty()) { // Пока очередь не пуста (O(V) в худшем случае)
        int current = q.front(); // Получаем первую вершину очереди (O(1))
        q.pop(); // Удаляем первую вершину из очереди (O(1))

        // Если текущая вершина уже посещена, пропускаем ее
        if (visited.count(current)) continue; // Проверка на наличие (O(log(V)))
        
        visited.insert(current); // Помечаем текущую вершину как посещенную (O(log(V)))

        // Проверяем, достигли ли мы целевой вершины
        if (current == goal) { // Проверка на равенство (O(1))
            auto end_time = high_resolution_clock::now(); // Запоминаем время окончания выполнения (O(1))
            printPath(parent, start, goal, graph); // Печатаем путь от start до goal (O(V), где N - длина пути)
            cout << "BFS Time: " << duration_cast<milliseconds>(end_time - start_time).count() << " ms\n"; // Вывод времени выполнения (O(1))
            return printPath(parent, start, goal, graph); // Возвращаем путь (O(V), где N - длина пути)
        }

        // Перебираем соседей текущей вершины
        for (const auto& [neighbor, _] : graph.at(current)) { // Проходим по всем соседям текущей вершины (O(V*Ev), где Ev - количество соседей)
            if (!visited.count(neighbor)) { // Проверяем, не посещен ли сосед (O(log(V)))
                q.push(neighbor); // Добавляем соседа в очередь (O(1))
                parent[neighbor] = current; // Устанавливаем текущую вершину как родителя соседа (O(1), но вставка в map O(log(V)))
            }
        }
    }
    
    cout << "BFS: No path found.\n"; // Если путь не найден, выводим сообщение (O(1))
    return -1; // Возвращаем -1, если путь не найден
}

double Dijkstra(const Graph& graph, int start, int goal) {
    auto start_time = high_resolution_clock::now(); // Запоминаем время начала выполнения функции (O(1))

    // Приоритетная очередь для хранения пар (дистанция, вершина), сортированная по дистанции
    priority_queue<pair<double, int>, vector<pair<double, int>>, greater<>> pq; // (O(1) для инициализации)
    
    map<int, double> distances; // Словарь для хранения минимальных расстояний до каждой вершины (O(1) для инициализации)
    map<int, int> parent; // Словарь для хранения родительских вершин (O(1) для инициализации)

    // Инициализируем все расстояния до вершин бесконечностью
    for (const auto& node : graph) {
        distances[node.first] = numeric_limits<double>::infinity(); // Установка расстояния до всех вершин в бесконечность (O(V) * O(log(V)) = O(V log V))
    }
    distances[start] = 0; // Расстояние до стартовой вершины равно 0 (O(log(V)))

    pq.emplace(0, start); // Добавляем стартовую вершину с расстоянием 0 в приоритетную очередь (O(1) для добавления, O(log(V)) для поддержания порядка)

    while (!pq.empty()) { // Пока очередь не пуста (O(N^2) в худшем случае)
        auto [current_distance, current_node] = pq.top(); // Получаем вершину с наименьшим расстоянием (O(1))
        pq.pop(); // Удаляем вершину из очереди (O(log(V)))

        // Если достигли целевой вершины, завершаем алгоритм
        if (current_node == goal) { // Проверка на равенство (O(1))
            auto end_time = high_resolution_clock::now(); // Запоминаем время окончания выполнения (O(1))
            printPath(parent, start, goal, graph); // Печатаем путь от start до goal (O(V), где N - длина пути)
            cout << "Dijkstra Time: " << duration_cast<milliseconds>(end_time - start_time).count() << " ms\n"; // Вывод времени выполнения (O(1))
            return printPath(parent, start, goal, graph); // Возвращаем путь (O(V), где N - длина пути)
        }

        // Перебираем соседей текущей вершины
        for (const auto& [neighbor, weight] : graph.at(current_node)) { // Проходим по всем соседям текущей вершины (O(Ev), где Ev - количество соседей)
            double new_distance = current_distance + weight; // Рассчитываем новое расстояние до соседа (O(1))

            // Если новое расстояние меньше уже известного, обновляем данные
            if (new_distance < distances[neighbor]) { // Проверяем, меньше ли новое расстояние (O(1))
                distances[neighbor] = new_distance; // Обновляем расстояние до соседа (O(log(V)))
                parent[neighbor] = current_node; // Устанавливаем текущую вершину как родителя соседа (O(1))
                pq.emplace(new_distance, neighbor); // Добавляем соседа с обновленным расстоянием в приоритетную очередь (O(1) для добавления, O(log(V)) для поддержания порядка)
            }
        }
    }
    
    cout << "Dijkstra: No path found.\n"; // Если путь не найден, выводим сообщение (O(1))
    return -1; // Возвращаем -1, если путь не найден
}

double AStar(const Graph& graph, int start, int goal, const Coordinates& coordinates) {
    auto start_time = high_resolution_clock::now(); // Запоминаем время начала выполнения функции (O(1))

    // Функция эвристики, которая вычисляет расстояние между двумя узлами
    auto heuristic = [&](int node) {
        auto [lon1, lat1] = coordinates.at(node); // Получаем координаты текущего узла (O(log(V)))
        auto [lon2, lat2] = coordinates.at(goal); // Получаем координаты целевого узла (O(log(V)))
        // Вычисляем евклидово расстояние между узлами
        return sqrt((lon1 - lon2) * (lon1 - lon2) + (lat1 - lat2) * (lat1 - lat2)); // O(1)
    };

    // Приоритетная очередь для хранения пар (f_cost, вершина), сортированная по f_cost
    priority_queue<pair<double, int>, vector<pair<double, int>>, greater<>> pq; // O(1) для инициализации
    map<int, double> g_costs; // Словарь для хранения g_cost (стоимость пути от стартовой до текущей вершины) (O(1) для инициализации)
    map<int, double> f_costs; // Словарь для хранения f_cost (g_cost + эвристика) (O(1) для инициализации)
    map<int, int> parent; // Словарь для хранения родительских вершин (O(1) для инициализации)

    // Инициализируем все g_cost и f_cost бесконечностью
    for (const auto& node : graph) { // Проходим по всем узлам графа (O(V))
        g_costs[node.first] = numeric_limits<double>::infinity(); // Установка g_cost в бесконечность (O(log(V)))
        f_costs[node.first] = numeric_limits<double>::infinity(); // Установка f_cost в бесконечность (O(log(V)))
    } // Общее время: O(V log V)

    g_costs[start] = 0; // Расстояние до стартовой вершины равно 0 (O(log(V)))
    f_costs[start] = heuristic(start); // Вычисляем f_cost для стартовой вершины (O(1))

    pq.emplace(f_costs[start], start); // Добавляем стартовую вершину в приоритетную очередь (O(1) для добавления, O(log(V)) для поддержания порядка)

    while (!pq.empty()) { // Пока очередь не пуста (в худшем случае O(N^2))
        auto [current_f_cost, current_node] = pq.top(); // Получаем вершину с наименьшим f_cost (O(1))
        pq.pop(); // Удаляем вершину из очереди (O(log(V)))

        // Если достигли целевой вершины, завершаем алгоритм
        if (current_node == goal) { // Проверка на равенство (O(1))
            auto end_time = high_resolution_clock::now(); // Запоминаем время окончания выполнения (O(1))
            printPath(parent, start, goal, graph); // Печатаем путь от start до goal (O(V), где N - длина пути)
            cout << "A* Time: " << duration_cast<milliseconds>(end_time - start_time).count() << " ms\n"; // Вывод времени выполнения (O(1))
            return printPath(parent, start, goal, graph); // Возвращаем путь (O(V), где N - длина пути)
        }

        // Перебираем соседей текущей вершины
        for (const auto& [neighbor, weight] : graph.at(current_node)) { // Проходим по всем соседям текущей вершины (O(Ev), где Ev - количество соседей)
            double tentative_g_cost = g_costs[current_node] + weight; // Рассчитываем новое g_cost до соседа (O(1))

            // Если новое g_cost меньше уже известного, обновляем данные
            if (tentative_g_cost < g_costs[neighbor]) { // Проверяем, меньше ли новое g_cost (O(1))
                g_costs[neighbor] = tentative_g_cost; // Обновляем g_cost до соседа (O(log(V)))
                f_costs[neighbor] = tentative_g_cost + heuristic(neighbor); // Обновляем f_cost для соседа (O(1))
                parent[neighbor] = current_node; // Устанавливаем текущую вершину как родителя соседа (O(1))
                pq.emplace(f_costs[neighbor], neighbor); // Добавляем соседа с обновленным f_cost в приоритетную очередь (O(1) для добавления, O(log(V))для поддержания порядка)
            }
        }
    }

    cout << "A*: No path found.\n"; // Если путь не найден, выводим сообщение (O(1))
    return -1; // Возвращаем -1, если путь не найден
}

void test_dfs() {
    Graph graph;
    Coordinates coordinates;
    parseGraph("test_graph.txt", graph, coordinates);
    double startLon = 1.0, startLat = 1.0;
    double goalLon= 2.0, goalLat = 2.0;
    int start = findNearestNode(coordinates, startLon, startLat);
    int goal = findNearestNode(coordinates, goalLon, goalLat);
    double weight_dfs = DFS(graph, start, goal);
    assert(weight_dfs == 74.0);
}

void test_bfs() {
    Graph graph;
    Coordinates coordinates;
    parseGraph("test_graph.txt", graph, coordinates);
    double startLon = 1.0, startLat = 1.0;
    double goalLon= 2.0, goalLat = 2.0;
    int start = findNearestNode(coordinates, startLon, startLat);
    int goal = findNearestNode(coordinates, goalLon, goalLat);
    double weight_bfs = BFS(graph, start, goal);
    assert(weight_bfs == 22.0);
}

void test_dijkstra() {
    Graph graph;
    Coordinates coordinates;
    parseGraph("test_graph.txt", graph, coordinates);
    double startLon = 1.0, startLat = 1.0;
    double goalLon= 2.0, goalLat = 2.0;
    int start = findNearestNode(coordinates, startLon, startLat);
    int goal = findNearestNode(coordinates, goalLon, goalLat);
    double weight_deijkstra = Dijkstra(graph, start, goal);
    assert(weight_deijkstra == 18.0);
}

void test_Astar() {
    Graph graph;
    Coordinates coordinates;
    parseGraph("test_graph.txt", graph, coordinates);
    double startLon = 1.0, startLat = 1.0;
    double goalLon= 2.0, goalLat = 2.0;
    int start = findNearestNode(coordinates, startLon, startLat);
    int goal = findNearestNode(coordinates, goalLon, goalLat);
    double weight_deijkstra = Dijkstra(graph, start, goal);
    assert(weight_deijkstra == 18.0);
}

int main() {
    string filename = "spb_graph.txt";
    test_dfs();
    test_bfs();
    test_dijkstra();
    test_Astar();
    cout << "All tests passed.\n";

    Graph graph;
    Coordinates coordinates;
    parseGraph(filename, graph, coordinates);

    double startLon = 30.499879, startLat = 59.936492 ;
    double goalLon= 30.307961, goalLat = 59.957246;


    int start = findNearestNode(coordinates, startLon, startLat);
    int goal = findNearestNode(coordinates, goalLon, goalLat);

    if (start == -1 || goal == -1) {
        cerr << "Error: Start or goal node not found by coordinates." << endl;
        return 1;
    }

    cout << "\nRunning DFS:\n";
    DFS(graph, start, goal);

    cout << "\nRunning BFS:\n";
    DFS(graph, start, goal);

    cout << "\nRunning Dijkstra:\n";
    Dijkstra(graph, start, goal);

    cout << "\nRunning A*:\n";
    AStar(graph, start, goal, coordinates);

    return 0;
}