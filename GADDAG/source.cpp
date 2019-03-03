#include "GADDAG.h"

int main(){
	string InputPath = "../assets/Dict.txt";
    GADDAG Dag(InputPath);

	string Hook = "arr...es..";
	string Rack = "aaabrrcdefgdhijklsswwwrrrtttmnppopqreestuvwxyz";

    vector <string> possible;
	
	possible = Dag.ContainsHookWithRackAtPos(Hook,Rack, 3, 15);

	for (int i = 0; i < possible.size(); i++) {
		cout << possible[i] << endl;
	}
    return 0;
}