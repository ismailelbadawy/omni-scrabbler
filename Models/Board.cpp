#include "Board.h"



Board::Board()  //Default constructor
{
	for (int i = 0; i < 15; i++)
	{
		for (int j= 0; j<15;j++)
		{
			//This should compute somehow the bonus of each tile and place it while contructing the board
			tiles_[i][j].SetLetter('0'); 				//Initialize all tiles first with zeroes
		}
	}
	this->count_ = 0;
}

Board::Board(char tiles[15][15])
{
	for (int i = 0; i < 15; i++)
	{
		for (int j= 0; j<15;j++)
		{
			//This should compute somehow the bonus of each tile and place it while contructing the board
			tiles_[i][j].SetParams(tiles[i][j],i,j,1,1);
		}
	}
}

void Board::GetTiles(Tile tiles[15][15])
{
	for (int i = 0; i < 15; i++)
	{
		for (int j = 0; j < 15; j++)
		{
			tiles[i][j] = tiles_[i][j];
		}
	}
}

void Board::UpdateBoard(string word, int row, int column, bool horizontal)
{
	for (int i = 0; i < (int)word.length(); i++) {
		//This means that the word will have the same row but different columns
		if (horizontal)
			tiles_[row][column++].SetParams(word[i], row, column, 1, 1);

		//This means the word will have the same column but different rows
		else 
			tiles_[row++][column].SetParams(word[i], row, column, 1, 1);
	}
	this->count_+= (int)word.length();
}

void Board::Probe(char c,int row,int col) //For testing purposes only
{
	tiles_[row][col].SetParams(c,row,col,1,1);
	this->count_++;
}

int Board::GetCount(){
	return this->count_;
}

Board::~Board()
{
}
