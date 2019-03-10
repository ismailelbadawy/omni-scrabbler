// #include "GADDAG.h"

// int main(){
// 	string InputPath = "../assets/Dict.txt";
//     GADDAG Dag(InputPath);

// 	//string Hook = "arr...es..";
// 	string Hook = "e......t";
// 	string Rack = "aaabrrcdefgdhijklsswwwrrrtttmnppopqreestuvwxyz";

//     vector<pair<string, vector<int>>> possible;
	
// 	possible = Dag.ContainsHookWithRackAtPos(Hook,Rack, 2, 4);

// 	for (int i = 0; i < possible.size(); i++) {
// 		cout << possible[i].first << " at positions: ";
// 		for (int j = 0; j < possible[i].second.size(); j++)
// 			cout << possible[i].second[j] << " ";
// 		cout << endl;
// 	}
// 	bool check = Dag.CheckWordInDict("zebra");
// 	cout << check;

//     return 0;
// }