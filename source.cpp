#include "GADDAG.h"

int main(){
	string InputPath = "../assets/Dict.txt";
    GADDAG Dag(InputPath);

	string Hook = "";
	string Rack = "abcdefghijklmnopqrstuvwxyz";

    vector <string> possible;
	possible = Dag.ContainsHookWithRack(Hook,Rack);

	for (int i = 0; i < possible.size(); i++) {
		cout << possible[i] << endl;
	}
    return 0;
}