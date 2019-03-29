#include"WordPossibility.h"

WordPossibility::WordPossibility()
{

}
WordPossibility::WordPossibility(string word){
    this->word_=word;
}
void WordPossibility::SetWord(string word){
    this->word_ = "";
    word_=word;
}
void WordPossibility::AddRacks(string possibleRack){
    Rack rack(possibleRack);
    this->remainingracks_.push_back(rack);
}
void WordPossibility::AddPositions(int position){

    this->positions_.push_back(position);
}

int WordPossibility::GetPosition(int i){
    return this->positions_.at(i);
}

string WordPossibility::GetWord(){
    return this->word_;
}
vector <int> WordPossibility::GetPositions(){
    return this->positions_;
}
vector <Rack> WordPossibility::GetRemainingRacks(){
    return this->remainingracks_;
}
WordPossibility::~WordPossibility(){

}