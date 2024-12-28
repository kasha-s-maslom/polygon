#include <iostream>
#include <map>
#include <string>
#include <cassert>

std::string reducer(std::string fullLine){
    std::map<char, int> chars; // O(1)
    std::string out; // O(1)
/*
реализация функции идёт через создание стандартного цикла,
который будет походиться по строке, добавлять значения в map,
а потом при смене символа занчение добавляется в vector и обнуляется
*/
    for(int i = 1; i <= fullLine.size(); i ++){  // O(N)
        if(fullLine[i] == fullLine[i-1]){ // O(1 + 1 + 1)
            chars[fullLine[i]] ++; // O(1 + 1)
        }
        else{
            out.push_back(fullLine[i-1]);
            out.push_back((chars[fullLine[i-1]] + 1) + '0');
            chars[fullLine[i-1]] = 0; // O(1 + 1)
        }       
    }
    return out;
}

/*bool compare_string(const std::string &fullLine, const std::string &out) {
    return fullLine == out;
}

void tests() {
    // Тест кейс 1
    const std::string input = "abcdefg";
    std::string result = reducer(input);
    const std::string out = "a1b1c1d1e1f1g1";

    assert(compare_string(result, out));

    // Кейс 2
    const std::string input = "aaabb";
    std::string result = reducer(input);
    const std::string out = "a3b2";

    assert(compare_string(result, out));

    // Кейс 3
    const std::string input = "aaabb";
    std::string result = reducer(input);
    const std::string out = "a3b2";

    assert(compare_string(result, out));

    // Кейс 4
    const std::string input = "";
    std::string result = reducer(input);
    const std::string out = "a3b2";

    assert(compare_string(result, out));


    // Кейс 5
    
}*/

int main(){

    std::string inp = "aaabbcaa";
    std::string result = reducer(inp);
    std::cout << result;
    return 0;

}
