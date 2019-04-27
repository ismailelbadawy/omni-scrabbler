#include"PreendgameEvaluator.h"

PreendgameEvaluator::PreendgameEvaluator(){

}
PreendgameEvaluator::~PreendgameEvaluator(){

}

PreendgameEvaluator::PreendgameEvaluator(map<string, double> *rackLeave){
    this->doubleValued_ = rackLeave;
    this->remainingletters_ = {'a','b','d','a','e','c','h','h','x','p'}; //for testing only
    this->numTilesByOponent_ = 3; //for testing only
}

double PreendgameEvaluator::CalculateLeave(string rack)
{
	double rackLeave = 0.0;
    for(int i = 0; i < rack.length(); i++)
    {
        for(int j = i + 1; j < rack.length(); j++)
        {
            // Rack
            if(i != j)
            {
				char leaveValue[] = { rack[i] - 32, rack[j] - 32};
				string leave(leaveValue);
				leave = leave.substr(0, 2);
				rackLeave += (*doubleValued_)[leave];
            }
        }
    }
	return rackLeave;
}

void PreendgameEvaluator:: Combination(int start, int size, string &rem, string &temprack, double &MaxScore, string &BestRackLeave){
    if (size == 0) {
		//double rackleave_score = rackleavefn(temprack)
        double rackleave_score = CalculateLeave(temprack);
        if (rackleave_score > MaxScore){
            MaxScore = rackleave_score;
            BestRackLeave = temprack;
            return;
        }
        //else keep the max and best rack leave as they are and return
		return;
	}
	for (int i = start; i <= rem.size() - size; ++i) {
		temprack.push_back(rem[i]);
		Combination(i + 1, size - 1, rem, temprack, MaxScore, BestRackLeave);
		temprack.pop_back();
	}

}

Rack PreendgameEvaluator:: OponentRackEstimation(){
    //vector<char> remainingletters = remainingletters_; //take a copy of remainingletters_
    string remainingletters= "";
    for (int i=0; i< remainingletters_.size(); i++){
        remainingletters += remainingletters_[i];
    }
    
    int TilesLeftOponent = 7 - numTilesByOponent_; //if opponent played 3 letters in the last turn, there are 4 letters left to guess
    double MaxRackLeave=0.0;

    //vector<char> tempRack, OpponentRack;
    string tempRack, OpponentRack;
  
    //get first part of oponent rack based on his last move
    Combination(0, TilesLeftOponent, remainingletters, tempRack, MaxRackLeave, OpponentRack);

    //remove from remainingletters letters picked in opponent Rack
    for (int i=0; i<OpponentRack.size(); i++){
        remainingletters.erase(std::remove(remainingletters.begin(), remainingletters.end(), OpponentRack[i]), remainingletters.end());
    } 

    //get second part of oponent rack randomly
    for (int i = 0; i < numTilesByOponent_; i++) {
		int randomIndex = rand() % remainingletters.size();
		OpponentRack.push_back(remainingletters[randomIndex]);	
		remainingletters.erase(remainingletters.begin()+randomIndex);

	}

    string OppRackString ="";
    for (int i=0; i<OpponentRack.size(); i++){
        OppRackString += OpponentRack[i];
    }
    Rack rack(OppRackString);
    return rack;
}