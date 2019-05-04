
#include "EndGameSim.h"
// SimulationEndGame::SimulationEndGame(MoveGenerator MovGen,Board board)
// {
//     this->MovGen=MovGen;
//     this->board=board;
    
// }

// Move* SimulationEndGame::LookAhead(int Depth,int turn,Rack MyRack,Rack OppRack,Board CurrBoard,int ScoreRedArmy,int ScoreOpp,EndgameEvaluator* Eval)
// {

//    //checks if depth equals zero (end of simulation)
//     if(Depth==0)
//     return NULL;
    
//     vector<Move> AllMoves;
//     vector<Move> Moves;
//     vector<Move> * NewMoves;
//    //if it is my turn generate my moves
//     if(turn%2==0)
//     {//checks if myrack is empty
//     if(MyRack.GetLength()==0)
//         return NULL;
//     //generate my moves
//     AllMoves=MovGen.Generate(&MyRack,CurrBoard,false);
//     Eval->SetAttributes(AllMoves,CurrBoard,MyRack,OppRack);
//     NewMoves = Eval->Evaluate();
//      vector<Move> * evaluatedMoves = Eval->Evaluate();
//     Moves=*NewMoves;
    
    
//     } 
//     //if it is opp turn generate his moves
//     else
//     {//checks if myrack is empty
//      if(OppRack.GetLength()==0)
//         return NULL;
//     //generate opp moves
//     AllMoves=MovGen.Generate(&OppRack,CurrBoard,false);
//     Eval->SetAttributes(AllMoves,CurrBoard,OppRack,MyRack);
//     NewMoves=Eval->Evaluate();
//     Moves=*NewMoves;
//     }
//     //if no moves are generated return
//     if(Moves.size()==0)
//     {
//         return NULL;
//     }

   
    
//     int Max=0;

//     int TotalMax=0;

//     Move* NewMax=NULL;
//     Move* EndMax=NULL;
//     Rack* newRackOp=NULL;
//     Rack* newRackMe=NULL;
//     Play * p=NULL;

//     //loops on the moves generated
//     for(int i=0;i<Depth;i++)
//     {
//         p=new Play(Moves[i].GetPlay()->GetLetters(),Moves[i].GetPlay()->GetRow(),Moves[i].GetPlay()->GetColumn(),Moves[i].GetPlay()->GetIsHorizontal());
         
//         //gets the remaining rack if this moved is played
//         string newrack=Moves[i].GetRack();
//     if(turn%2==0)
//        {
//            //gets the new rack for both players 
//            newRackMe=new Rack(newrack);
//            string s="";
//            for(int i=0;i<OppRack.GetLength();i++)
//            {
//                s+=OppRack.GetLetter(i);
//            }
//            newRackOp=new Rack(s);
//            //evaluates the score if this play was choosen
//          ScoreRedArmy+=Moves[i].GetPlay()->GetScore();
       
//        }
//     else
//     {
//         //gets the new rack for both players 
//          newRackOp=new Rack(newrack);
//         string s="";
//            for(int i=0;i<MyRack.GetLength();i++)
//            {
//                s+=MyRack.GetLetter(i);
//            }
//            newRackMe=new Rack(s);
//         //evaluates the score if this play was choosen
//         ScoreOpp+=Moves[i].GetPlay()->GetScore();
//     }
//    //evalutes the board after the play
//    Tile* tiles[15][15];
//    CurrBoard.GetTiles(tiles);
//    Board newBoard;
//    newBoard.SetwholeTiles(tiles);
//     newBoard.UpdateBoard(Moves[i].GetPlay()->GetLetters(),Moves[i].GetPlay()->GetRow(),Moves[i].GetPlay()->GetColumn() ,Moves[i].GetPlay()->GetIsHorizontal());
   
//   //recursion to get the next move NewMax is next best move
//     NewMax=LookAhead(Depth-1,turn+1,*newRackMe,*newRackOp,CurrBoard,ScoreRedArmy,ScoreOpp,Eval);
    
//     int scoreofNewMax=0;
    
//     if(NewMax==NULL)
//     scoreofNewMax=0;
//     else
//     scoreofNewMax=NewMax->GetPlay()->GetScore();

//     if(TotalMax <  Moves[i].GetPlay()->GetScore()+scoreofNewMax)
//     {TotalMax=Moves[i].GetPlay()->GetScore()+scoreofNewMax;
//     //EndMax is the current best move from the ones we iterate on
//     EndMax=new Move();
    
//      EndMax->SetPlay(p);
//      EndMax->SetRack(Moves[i].Getwholerack());
//      EndMax->GetPlay()->SetScore(Moves[i].GetPlay()->GetScore());
//      EndMax->SetPenalty(Moves[i].GetPenalty());
//     }
   
         

     
//     }

   
//      if(NewMax==NULL)
//     { 
//     return EndMax;
//     }
//     return NewMax;



// }



SimulationEndGame:: ~SimulationEndGame()
{


}

