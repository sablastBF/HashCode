#include <iostream>
#include <fstream>
#include <random>

using namespace std;

int main(){
    ofstream inFile;
    inFile.open("in_test_1.txt");
    int n;
    cin >> n;   
    for (int i = 0; i < n; i++){
        inFile << to_string(rand());
        inFile <<" + ";
        inFile << to_string(rand());
        inFile <<" = ";
        inFile << to_string(rand());
        inFile <<'\n';
    }
    return 0;
}


//  304089431572
// 1303945573639
// 1608035005211
