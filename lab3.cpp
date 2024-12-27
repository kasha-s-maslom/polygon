#include <iostream>
#include <map>
#include <string>
#include <vector>
/*
std::vector<std::pair<char, int>> reducer(std::string fullLine){ //функция подсчёта количества одинаковых символов последовательности

    std::vector<std::pair <char, int>> counter;
    counter.push_back({fullLine[0], 1});

    for (int i = 1; i < fullLine.size(); i++){ //O(N)
        
        if (fullLine[i] == counter[counter.size()-1].first){//O(1+1+1), если следующий символ повторяется, то увеличиваем счётчик

            counter[counter.size()-1].second ++;

        }

        else{ //если следующий символ отличается, то вводим новый символ со знчением повтроений 1

            counter.push_back({fullLine[i], 1});

        }

    }

    return counter;

}
*/

char get_key(std::map<char, int> new_map, int k){
    
    for(auto i = new_map.begin(); i != new_map.end(); i++){
        if (k == new_map[i->first]){
            return i->first;
        }
    }

    return '*';

}

std::string reducer(std::string fullLine){
    std::map<char, int> chars;
    std::string out;
    
    for(int i = 0; i < fullLine.size() - 1; i ++){ 
        if(i + 1 != fullLine.size()){
            if(chars[fullLine[i]] == 0){
                chars[fullLine[i]]++;
            }
            else if(chars[fullLine[i]] != 0 && fullLine[i] == fullLine[i+1]){
                chars[fullLine[i]] ++;
            }
            else if(chars[fullLine[i]] != 0 && fullLine[i] != fullLine[i+1]){
                chars[fullLine[i]] ++;
                std::cout << fullLine[i] << chars[fullLine[i]];
                chars[fullLine[i]] = 0;
            }
        }
        else if(i + 1 == fullLine.size()){
            if(chars[fullLine[i]] == 0){
                std::cout << fullLine[i] << '1';
            }
            else if(chars[fullLine[i]] != 0 && fullLine[i] == fullLine[i+1]){
                chars[fullLine[i]] ++;
                std::cout << fullLine[i] << chars[fullLine[i]];
            }
            else if(chars[fullLine[i]] != 0 && fullLine[i] != fullLine[i+1]){
                chars[fullLine[i]] ++;
                std::cout << fullLine[i] << chars[fullLine[i]];
                std::cout << fullLine[i+1] << '1';
            }
        }
    }
    return out;
}

int main(){

    std::string word = "aaabbc";//O(1+1+1)
    std::string result = reducer(word);//O(1+1+1)
    std::cout << result;
    
}
