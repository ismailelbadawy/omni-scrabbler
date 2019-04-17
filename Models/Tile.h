#pragma once
class Tile
{
	char letter_;
	int row_;
	int column_;
	int score_;
	int bonus_;
public:
	Tile();
	void GetIndex(int &row, int&column);
	int GetScore();
	char GetLetter();
	void SetLetter(char c);
	void SetParams(char letter,int row,int col,int score=1,int bonus=1);
	int GetBonus();
	void CalculateTotalScore();
	void SetScore(int score);
	void SetBonus(int  bonus);
	bool operator< (const Tile& tile) const{
		 
        return  tile.letter_ > letter_;
    
	}
	~Tile();
};

