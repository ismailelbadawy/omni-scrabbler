#include <iostream>
#include <string>
#include <fstream>
#include <vector>

using namespace std;

//Function to read the dictionary represented as an input file 
//in the assets folder, it takes the inputsPath and returns 
//a vector containing all the data
vector<string> readFile(string InputPath){

    ifstream InputFile;
    vector<string> Dictionary;
    
    InputFile.open(InputPath);
    if (InputFile.is_open()) {
        string Line;
        while (getline(InputFile, Line)) {
            Dictionary.push_back(Line);
        }
    }
    return Dictionary;
}

int main(){
    string InputPath = "../assets/Dict.txt";
    vector<string> Dictionary = readFile(InputPath);
    
    
     return 0;
}

