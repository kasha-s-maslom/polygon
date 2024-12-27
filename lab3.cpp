#include <iostream>
#include <map>
#include <string>
#include <vector>

std::string reducer(std::string fullLine){
    std::map<char, int> chars;
    std::string out;
    
    for(int i = 1; i <= fullLine.size(); i ++){ 
        if(fullLine[i] == fullLine[i-1]){
            chars[fullLine[i]] ++;
        }
        else{
            std::cout << fullLine[i-1] << chars[fullLine[i-1]] + 1;
            chars[fullLine[i-1]] = 0;
        }       
    }
    return out;
}

int main(){

    std::cout << reducer("aaabbaaccccde");
    return 0;

}
