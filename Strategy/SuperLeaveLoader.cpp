#include "SuperLeaveLoader.h"
#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>
#include <map>

const unsigned int maxSize = 40;

SuperLeaveLoader::SuperLeaveLoader(map<string, double> * syn2, map<char, double> * worth, string filename = "assets/syn2", string worthFile = "assets/worths")
{
    ifstream file(filename, ios::in | ios::binary);

	if (!file.is_open())
	{
		cout << "Could not open " << "superleaves" << " to load superleave heuristic" << endl;
		return;
	}

    double value;
    string letters;

	while (!file.eof())
	{
		file >> letters;
        file >> value;
		if (file.eof())
			break;

        (*syn2)[letters] = value;
                
        reverse(letters.begin(), letters.end());
        
		(*syn2)[letters] = value;
        
	}
	
	file.close();

	ifstream worths(worthFile, ios::in | ios::binary);

	if(!worths.is_open())
	{
		cout << "Could not open " << worthFile << " to load worth heuristic "<< endl;
		return;
	}

	double charValue;
	string single;

	while(!worths.eof())
	{
		worths >> single;

		worths >> charValue;

		if(worths.eof())
			break;

		(*worth)[single[0]] = charValue;
	}


	worths.close();
}