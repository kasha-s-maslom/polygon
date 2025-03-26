#include <iostream>
#include <string>

using namespace std;

struct Node {
    bool is_number;  // Флаг: true если узел содержит число
    double value;    // Числовое значение (если is_number == true)
    char op;         // Оператор (если is_number == false)
    Node* left;      // Левый потомок
    Node* right;     // Правый потомок
    
    // Конструктор для числового узла
    Node(double val) : is_number(true), value(val), left(nullptr), right(nullptr) {}
    
    // Конструктор для операторного узла
    Node(char operation, Node* l, Node* r) 
        : is_number(false), op(operation), left(l), right(r) {}
};

class Calculator {
    string expr;  // Строка с выражением
    int pos;      // Текущая позиция в строке
    
    // Вспомогательные функции для разбора
    bool isDigit(char c) {return c >= '0' && c <= '9';}
    bool isSpace(char c) {return c == ' ' || c == '\t';}
    void skipSpaces() {while(pos < expr.size() && isSpace(expr[pos])) pos++;}
    
    // Парсинг числа из строки
    double parseNumber() {
        int start = pos;
        while(pos < expr.size() && (isDigit(expr[pos]) || expr[pos] == '.')) pos++;
        string numStr = expr.substr(start, pos - start);
        return stod(numStr);  // Преобразование строки в double
    }
    
    // Объявления методов парсинга
    Node* parseAddAndSub();
    Node* parseMulAndDiv();
    Node* parseNumberOrBrackets();
    
    // Рекурсивное удаление дерева
    void deleteTree(Node* node) {
        if(!node) return;
        deleteTree(node->left);
        deleteTree(node->right);
        delete node;  // Освобождение памяти
    }
    
public:
    // Основной метод вычисления выражения
    double calculate(const string& expression) {
        expr = expression;
        pos = 0;
        
        // Построение AST
        Node* root = parseAddAndSub();
        if(!root) throw "Invalid expression";
        
        // Вычисление и очистка
        double result = evaluate(root);
        deleteTree(root);
        return result;
    }

    // Рекурсивное вычисление выражения
    double evaluate(Node* node) {
        if(node->is_number) {
            return node->value;  // Базовый случай - число
        }
        
        // Рекурсивное вычисление подвыражений
        double left = evaluate(node->left);
        double right = evaluate(node->right);
        
        // Применение операции
        switch(node->op) {
            case '+': return left + right;
            case '-': return left - right;
            case '*': return left * right;
            case '/':
                if(right == 0) throw "Division by zero";
                return left / right;
            default: throw "Unknown operator";
        }
    }
};

// Реализация методов парсинга

// Парсинг наибольшего приоритета
Node* Calculator::parseNumberOrBrackets() {
    skipSpaces();
    
    if(pos >= expr.size()) throw "Unexpected end";
    
    // Обработка выражений в скобках
    if(expr[pos] == '(') {
        pos++;
        Node* node = parseAddAndSub();  // Рекурсивный парсинг
        skipSpaces();
        if(pos >= expr.size() || expr[pos] != ')') {
            deleteTree(node);  // Удаление частично разобранного дерева при ошибке
            throw "Missing parenthesis";
        }
        pos++;
        return node;
    }
    
    // Обработка чисел
    if(isDigit(expr[pos])) {
        return new Node(parseNumber());
    }
    
    throw "Invalid character";
}

// Парсинг * и /
Node* Calculator::parseMulAndDiv() {
    Node* left = parseNumberOrBrackets();
    
    // Обработка цепочки умножений/делений
    while(true) {
        skipSpaces();
        if(pos >= expr.size()) break;
        
        char c = expr[pos];
        if(c != '*' && c != '/') break;
        
        pos++;
        Node* right = parseNumberOrBrackets();
        left = new Node(c, left, right);  // Построение дерева операций
    }
    
    return left;
}

// Парсинг + и -
Node* Calculator::parseAddAndSub() {
    Node* left = parseMulAndDiv();
    
    // Обработка цепочки сложений/вычитаний
    while(true) {
        skipSpaces();
        if(pos >= expr.size()) break;
        
        char c = expr[pos];
        if(c != '+' && c != '-') break;
        
        pos++;
        Node* right = parseMulAndDiv();
        left = new Node(c, left, right);  // Построение дерева операций
    }
    
    return left;
}

int main() {
    Calculator calc;
    string input;
    
    cout << "Калькулятор поддерживает целые и дробные числа, операции +, -, *, / и скобки" << endl;
    cout << "Введите 'exit' для выхода" << endl;
    
    // Основной цикл чтения выражений
    while(true) {
        cout << "> ";
        getline(cin, input);
        
        if(input == "exit") break;
        if(input.empty()) continue;
        
        try {
            double result = calc.calculate(input);
            cout << "Результат: " << result << endl;
        } catch(const char* msg) {
            cerr << "Ошибка: " << msg << endl;
        }
    }
    
    cout << "Работа завершена." << endl;
    return 0;
}

// сложность по времени O(n), где n - количество символов в полученном выражении
// пространственная сложность O(n), где n - количество узлов в графе( кол-во операторов и чисел)