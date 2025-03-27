#include <iostream>
#include <vector>
#include <string>
#include <map> 
#include <cassert>

using namespace std;

bool IsInAlphabet(const string& input) { // O(n) 
    for(char с: input){ // O(n)
        if (isalpha(с) == 0){
            return false;
        }
    }
    return true;
}

string Decreaser(string input = ""){ // O(n)

    map<char, int> char_count;
    string result;

    if (input.empty()){
        cout << ">" << " ";
        getline(cin, input);
    }

    if(input == "" || !IsInAlphabet(input)){ return "Error, wrong string"; } 

    for(char с: input){ // O(n)
        char_count[с] ++;
    }

    for(const auto& iter : char_count) { // O(n)
        result += iter.first;
        result += to_string(iter.second);
    }

    return result;

}

void tests(){
    {
        string string = Decreaser("abbccc"); // O(n), O(m)
        assert(string == "a1b2c3");
    }
    {
        string string = Decreaser("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"); // O(n); O(1)
        assert(string == "a30");
    }
    {
        string string = Decreaser(" "); // O(1)
        assert(string == "Error, wrong string");
    }
    {
        string string = Decreaser("*"); // O(n)
        assert(string == "Error, wrong string");
    }
    {
        string string = Decreaser("abcdefghijklmnopqrstuvwxyz"); // O(n * log n); O(n)
        assert(string == "a1b1c1d1e1f1g1h1i1j1k1l1m1n1o1p1q1r1s1t1u1v1w1x1y1z1"); 
    }
    cout << "Tests done correct!";
}

int main(){
    
    /*string a = Decreaser();
    cout << a;*/
    
    tests();

    return 0;

}

// временная сложность в худшем случае O(n * log n), где n - длина строки input и все симолы уникальны. Срений случай O(n)
// пространственная сложность в худшем случае O(n), в среднем случае O(m), где m - количество уникальных символов в строке input 