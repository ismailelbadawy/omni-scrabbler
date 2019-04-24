#include "Board.h"



Board::Board()  //Default constructor
{
	for (int i = 0; i < 15; i++)
	{
		for (int j= 0; j<15;j++)
		{
			//This should compute somehow the bonus of each tile and place it while contructing the board
			tiles_[i][j].SetParams('0',i,j,1,1);
		}					//Initialize all tiles first with zeroes
	}
	this->count_ = 0;
}

Board::Board(char tiles[15][15])
{
	for (int i = 0; i < 15; i++)
	{
		for (int j= 0; j<15;j++)
		{
			if (tiles[i][j]=='0')
			{
			// Triple word
			if ((i==7&&(j == 0 || j == 14))|| 
			 ((i == 0 || i == 14)&&(j==0 || j==7 || j==14)))
			{
				tiles_[i][j].SetParams(tiles[i][j],i,j,1,5);
			}
			// Double Word
			else if (((i == j)&&(i<5 || i>9)&&(i!=14 && i!=0))||
			((14-i==j)&&(j<5||j>9)&&(j!=14 && j!=0))){
				tiles_[i][j].SetParams(tiles[i][j],i,j,1,4);
			}
			// Triple Letter 
			else if (((i==1||i==13)&&(j==5 || j==9))||
					(((i==5 ||i ==9) &&( j==1 || j == 13 ||j==5 || j==9))))
			{
				tiles_[i][j].SetParams(tiles[i][j],i,j,1,3);
			}
			// Double Letter
			else if 
				(((i == 0 || i == 14)&&(j == 3 || j ==11))||
				((i==2 || i == 12)&&(j==6 || j == 8))||
				((i==3 || i == 11)&&(j==0 || j==7 || j==14))||
				((i==3 || i == 11)&&(j==0 || j==7 || j==14))||
				((i==6 || i == 8)&&(j==2 ||j==6 || j==8 || j==12))||
				((i == 7)&&(j == 3 || j==11)))
			{
				tiles_[i][j].SetParams(tiles[i][j],i,j,1,2);
			}
			}
			else {
				tiles_[i][j].SetParams(tiles[i][j],i,j,1,1);
			}	
		}	
	}
}

void Board::GetTiles(Tile* tiles[15][15])
{
	for (int i = 0; i < 15; i++)
	{
		for (int j = 0; j < 15; j++)
		{
			tiles[i][j] = &tiles_[i][j];
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

void Board::Probe(char c,int row,int col,int score) //For testing purposes only
{
	tiles_[row][col].SetParams(c,row,col,score,1);
	this->count_++;
}

int Board::GetCount(){
	return this->count_;
}

bool Board::IsPositionEmpty(int row, int column){
	return this->tiles_[row][column].GetLetter()=='0'?true:false;
}
void Board::SimulateMove(Move* move){
	int rowIterator = move->GetPlay()->GetRow();
	int columnIterator = move->GetPlay()->GetColumn();
	int iterator =0;
	
	
	for (int j = 0; j < (int)move->GetPlay()->GetLetters().length() ;j++)
	{
		
			this->tiles_[rowIterator][columnIterator]=move->GetPlay()->GetTiles()[j];
		if (move->GetPlay()->GetIsHorizontal())
		{
			columnIterator++;
		}
		else{
			rowIterator++;
		}
	}

	
}




Board::~Board()
{
}
