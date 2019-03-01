#include "GADDAG.h"

int main(){
    GADDAG Dag;
    string InputPath = "../assets/Dict.txt";
    Dag.LoadDag(InputPath);

    vector <string> possible;
	string Hook = "";
	string Rack = "perslaywhattgr";
	possible = Dag.ContainsHookWithRank(Hook,Rack);

	for (int i = 0; i < possible.size(); i++) {
		cout << possible[i] << endl;
	}
    return 0;
}