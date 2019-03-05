#include "GADDAG.h"

int main(){
	string InputPath = "../assets/Dict.txt";
    GADDAG Dag(InputPath);

	string Hook = "arr...es..";
	string Rack = "aaabrrcdefgdhijklsswwwrrrtttmnppopqreestuvwxyz";

    vector <string> possible;
	
	possible = Dag.ContainsHookWithRackAtPos(Hook,Rack, 2, 5);

	for (int i = 0; i < possible.size(); i++) {
		cout << possible[i] << endl;
	}
	bool check = Dag.CheckWordInDict("zebra");
	cout << check;

    return 0;
}