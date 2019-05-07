#include "GameManager.h"
#include <string>
#include <vector>
#include <stdint.h>
#include<inttypes.h>
enum Opponent
{
    Human,
    AI
};


GameManager::GameManager()
{
}
int* Convert(uint8_t tiles[7]){
int *newtiles=new int[7];
for (int i=0;i<7;i++){
 newtiles[i]=(int)tiles[i];
 std::cout<<"old tile: "<<tiles[i]<<" new tile: "<<newtiles[i]<<std::endl;
}
return newtiles;
}

int GameManager::InitGame(MoveGenerator *MovGen, map<string, double>* syn2, map<char, double>* worth)
{
    this->numberTilesByOpponent = 0;
    this->MovGen_ = MovGen;
    this->syn2_ = syn2;
    this->worth_ = worth;
    // get mode , if AI init server
    //
     gui = new GUI();
     gui->initConnection();
     char*mode= gui->Receive();
     cout << "\n MODE: " <<mode<<endl;
     string tempString(mode);
     Mode= tempString;
    //char *message = "0,0:00,RedArmy,Youssef,0000000000000000,\0";
    //gui->Send(message);
    if (Mode[0]='M' && Mode[1]=='a'){
        client = new Comm();
        Comm::GetGame().Score = 0;
        Comm::GetGame().Opponent_Score = 0;
        std::cout<<"AI mode"<<std::endl;
    }
    return 0;
}
string GameManager::GetMode(){
    return Mode;
}
// waitttt sanya hashoof 7aga 

void GameManager::PlayAI(bool &ended, Board *board, Bag *bag)
{
    
    if (Comm::mystate == Comm::States::RECEIVE_RACK)
    {
        std::cout << "receive rack" << std::endl;
        while(!Comm::rackreceived){      
        }
        string temprack=ConvertIntRackToString(Comm::GetGame().Tiles);
        toSmall(temprack);
        Rack RecRack(temprack);
        this->AI_Rack = RecRack;
        gui->Send(ConvertMessageAI(1)); //send rack to gui
        std::cout << "sent rack" << std::endl;
        
        //Comm::mystate = Comm::States::IDLE;
    }
    else if (Comm::mystate == Comm::States::EXCHANGE_RESPONSE){
         std::cout << "receive rack" << std::endl;
        while(!Comm::exchconfirmed){      
        }
        string temprack=ConvertIntRackToString(Comm::GetGame().Tiles);
        toSmall(temprack);
        Rack RecRack(temprack);
        this->AI_Rack = RecRack;
        gui->Send(ConvertMessageAI(1)); //send rack to gui
        // send rack to agent
        std::cout << "exchanged tiles" << std::endl;
        Comm::exchconfirmed=false;
        Comm::mystate = Comm::States::IDLE;
    }
    else if (Comm::mystate == Comm::States::THINKING)
    {
        std::cout<<"please start playing"<<std::endl;
        Agent AI_Agent(board, bag, &AI_Rack);
        vector <Move> moves;
        Move chosenMove;

        int BagSize = (int)bag->GetRemainigLetters().size();
        moves = MovGen_->Generate(&AI_Rack, *board, board->GetCount() == 0);

        ///get it from server
        //int numTilesByOpponent = 3;

        if (moves.size() > 0){
			if (BagSize > (9+7)){//MidGame
				chosenMove = AI_Agent.MidGame(moves, this->syn2_, this->worth_,this->MovGen_, this->numberTilesByOpponent); //should return best move
					
			}
			else if (BagSize > (0+7) && BagSize <= (9+7)){
				chosenMove = AI_Agent.PreEndGame(this->syn2_,this->worth_,this->MovGen_,moves, this->numberTilesByOpponent); //should return best move
			}
			else if (BagSize == (0+7)){
				//chosenMove = AI_Agent.EndGame(moves, this->syn2_, this->worth_, this->MovGen_, this->AI_Rack, this->numberTilesByOpponent);
			}

            Move PassMove = AI_Agent.GetPassMove();

            if (chosenMove.GetScore() < PassMove.GetScore()){
				//Send Pass
                Comm::agentmove = "PASS";
			}
            else {
                Comm::agentmove = "PLAY";
                //Send chosen move to server
                ServerPlay finalmove;
                AgentMove finalMoveConv= AI_Agent.MoveToServer(chosenMove);
                std::cout<<"your move is : "<<std::endl;
                finalmove.Column = static_cast<uint8_t>(finalMoveConv.col+1);
                std::cout<<"Columns: "<<finalmove.Column;
                finalmove.Row = static_cast<uint8_t>(finalMoveConv.row+1);
                std::cout<<"Rows: "<<finalmove.Row;
                finalmove.Direction = static_cast<uint8_t>(finalMoveConv.dir);
                std::cout<<"Direction: "<<finalmove.Direction;
                finalmove.Score = finalMoveConv.score;
                std::cout<<"Score: "<<finalmove.Score;
                for (int i=0; i<7; i++){
                    if (i < finalMoveConv.tiles.length()){
                        finalmove.Tiles[i] = static_cast<uint8_t>(GetCorrespondigInt(finalMoveConv.tiles[i]));
                    }else{
                        finalmove.Tiles[i] = static_cast<uint8_t>(0);
                    }
                    std::cout<<"Tile "<<i<<finalmove.Tiles[i]<<" "<<std::endl;
                } 
                Comm::SetPlay(finalmove);
               
            }
		}
        else {
			    //Send Pass
                Comm::agentmove = "PASS";
		}    
        if (Comm::agentmove == "EXCHANGE")
        {
            TilesStruct extiles;
            Comm::SetExchanged(extiles);
        }
        std::cout<<"you finished your play"<<std::endl;
        Comm::simulationfinished = true;
        Comm::mystate = Comm::States::IDLE;
      
    }
    else if (Comm::mystate == Comm::States::PLAY_RESPONSE)
    {
        std::cout << "play response is trueeeeeeeeeee";
      
        while(!Comm::playconfirmed){
        }
        string temprack=ConvertIntRackToString(Comm::GetGame().Tiles);
        toSmall(temprack);
        Rack RecRack(temprack);
        this->AI_Rack = RecRack;
        cout<<"You sent :"<<ConvertMessageAI(2)<<std::endl;
        gui->Send(ConvertMessageAI(2)); // sending the move and the rack to the gui
        std::cout<<"Play received"<<std::endl;
        Comm::playconfirmed=false;
        Comm::mystate = Comm::States::IDLE;
    }
    else if (Comm::mystate == Comm::States::PLAY_OPPONENT)
    {
        std::cout << "opponent played" << std::endl;
        while(!Comm::oppplayed){
        }
        AgentMove opponent;
        opponent.row=(int) Comm::GetGame().OpponentMove.Row;
        opponent.col=(int)Comm::GetGame().OpponentMove.Column;
        opponent.dir=(int)Comm::GetGame().OpponentMove.Direction;
        opponent.tiles="\0";
        for (int i=0;i<7;i++){
            if (( int)Comm::GetGame().OpponentMove.Tiles[i]!=0){
             string temp=GetCorrespondigLetter((int)Comm::GetGame().OpponentMove.Tiles[i]);
             toSmall(temp);
             opponent.tiles=opponent.tiles+ temp; //howa leh hena plus?
             // 3ashan da string fa ba3mel concatenate ayywa sahh tamam fehemt, temo di char wahed, sah?heya string bas fi 7arf wa7ed yes bezabt
             //habtedy tayyebb fightinngg yes
            }      
        }
       //hena ba7ot el moves fel oppnent di , hena momken t update el board, okaay
        // sending  the new board to the agentssssssss\sss
        this->numberTilesByOpponent = opponent.tiles.length(); //tamam khalast
        Tile * tiles [15][15]; // okaay habda2 a run ,hanel3ab ded agent b cin w cout , ya3ni da w wa7ed 3al console 
        board->GetTiles(tiles); // howwa m3alla2 bas sawani
        int StartRow= opponent.row;
        int StartCol= opponent.col;
        int Dir= opponent.dir;
        string oppMove = opponent.tiles;
        TileScoreCalculator tilescorecalculator_;
        while (oppMove.length() > 0){
            if (tiles[StartRow][StartCol]->GetLetter() == '0'){
                board->Probe(oppMove[0], StartRow, StartCol, tilescorecalculator_.GetTileScore(tiles[StartRow][StartCol]->GetLetter())); //howa ana khalast bas fadel agib el score , bta3 meen ?el tile nafsaha kan fi fun bas mesh la3yaha
                oppMove.erase(oppMove.begin());
            }
            if (Dir == 0){//di horizontal wala vertical??  horizontal fadel bas law mehtaga ahawel capital wala la2, bas ghaleban la2, fa yalla n test howwa ana 7ateet el 7aga fel opponent move di small yes yes i know
            //asdy call el calculator mesh 3arfa beya5od small wala capital, ismail mesh beyrod bas mesh moshkla, heyya ghaleban sah keda
        // okaay tamam abda2 a run el server ?sawany nessit haga
                StartCol++;
            }
            else{
                StartRow++;
            }
        }

        gui->Send(ConvertMessageAI(3)); // send the rack and the opponent's move to gui
        std::cout<<"Opponent play received"<<std::endl;
        Comm::mystate = Comm::States::THINKING;
        Comm::oppplayed=false;
        //  Comm::mystate=Comm::States::IDLE;
    }
    else if (Comm::mystate == Comm::States::EXCHANGE_OPPONENT)
    {
         while(!Comm::oppplayed){
        }
        std::cout << "opponent exchanged" << std::endl;
        Comm::oppplayed=false;
        // inform agent about the opponent's move
        Comm::mystate=Comm::States::THINKING;
    }
    else if (Comm::mystate == Comm::States::PASS_OPPONENT)
    {
        while(!Comm::oppplayed){
        }
        std::cout << "opponent passed" << std::endl;
        // inform agent about the opponent's move
        Comm::oppplayed=false;
        Comm::mystate = Comm::States::THINKING;
        // Comm::mystate=Comm::States::IDLE;
    }
    else if (Comm::mystate == Comm::States::ENDCONNECTION)
    {
        gui->Send(ConvertMessageAI(4)); // send terminate connection to gui
        ended = true;
    }
    // }
}

void printBoard(Board *board){
    Tile * tiles [15][15];
    board->GetTiles(tiles);
    for (int i=0; i < 15; i ++){
        for (int k=0; k <15; k++){
            char c = tiles[i][k]->GetLetter();
            cout << c << " ";
        }
        cout << endl;
    }
}

char *GameManager::ConvertMessageAI(int type)
{
    //char *message;
    string tempmessage = "\0";
    if (type == 1)
    { // send the rack and the scores only in case of game init or game exchhange
        tempmessage = "1,0:00," + to_string(Comm::GetGame().Score) + "," + to_string(Comm::GetGame().Opponent_Score) + "," + ConvertIntRackToString(Comm::GetGame().Tiles) + ",";
    }
    else if (type == 2)
    {   std::cout<<"we will send this to the gui"<<std::endl;
        
        int *t=Convert(Comm::GetPlay().Tiles);
       // std::cout<<ConvertIntRackToString(t)<<std::endl;
        // send the rack with a move
        tempmessage = "2,0:00," + to_string(Comm::GetGame().Score) + "," + to_string(Comm::GetGame().Opponent_Score) + "," + ConvertIntRackToString(Comm::GetGame().Tiles) + ",";
        tempmessage = tempmessage + ConvertIntRackToString(t) + "," + to_string(14-Comm::GetPlay().Row+1) + "," + to_string(Comm::GetPlay().Column-1) + "," + to_string(Comm::GetPlay().Direction-48) + ",";
        std::cout<<tempmessage<<std::endl;
    
    }
    else if (type == 3)
    {   //send a rack with the opponent's move
         int *t=Convert(Comm::GetGame().OpponentMove.Tiles);
        tempmessage = "3,0:00," + to_string(Comm::GetGame().Score) + "," + to_string(Comm::GetGame().Opponent_Score) + "," + ConvertIntRackToString(Comm::GetGame().Tiles) + ",";
        tempmessage = tempmessage + ConvertIntRackToString(t) + "," + to_string(14-Comm::GetGame().OpponentMove.Row+1) + "," + to_string(Comm::GetGame().OpponentMove.Column-1) + "," + to_string(Comm::GetGame().OpponentMove.Direction) + ",";
    }
    else if (type == 4)
    {   //terminate connection
        tempmessage = "-1";
    }
    while (tempmessage.size() <= 49)
    {
        tempmessage = tempmessage + "0";
    }
    tempmessage = tempmessage + ",\0";

    char* message = new char[51];
    for (int i = 0; i < 50; i++)
    {
        message[i]=tempmessage[i];
    }
    message[50]='\0';
    return message;
}

void GameManager::PlayHuman(Board *board, Bag *bag, MoveGenerator *movGen,  map<string, double>* syn2, map<char, double>* worth)
{  
    OpponentScore=0;
    AgentScore=0;
    this->Score=0;
    agentmove.score=0;
        vector<Move> moves;	
        moves.clear();
        //HumanMove.clear();
        cout << "moves cleared";

		bool MyTurn = false;
		HumanMode Human(board, bag);
		Rack AgentRack;
		Rack OpponentRack;
		Move chosenMove;

        bool MyMoves= true;
		bool OppMoves = true;

		Tile* boardTiles [15][15];
		board->GetTiles(boardTiles);
	
		Human.SetMyRack(AgentRack);
		Human.SetOpponentRack(OpponentRack);
  
        // take the rack from the agent
        this->HumanRack = OpponentRack.RackToString();
        this->MyRack = AgentRack.RackToString();

        cout << "his rack " << this->HumanRack;
        
        toCapital(HumanRack);
        gui->Send(ConvertMessageHuman(1)); //send to GUI opponent rack

        while(!Human.CheckGameOver(MyMoves, OppMoves)){
            HumanMove.clear();
            char *move=gui->Receive();                 //receive from GUI
            if(move[0]=='-' && move[1]=='1' && move[2]==',' ){
                char*x="n,1234567891234567891234567S456789111111111111111,\0";
                gui->Send(x);
            }
            else{
               cout<<move<<endl;
                InterpretMessage(move);
                
                //string FbMessage="";
                int chosenMoveScore =0;
                if (MoveType == 0 || MoveType == 2 || MoveType == 3){ //opponent played or asked for hint, get best move
                    if (moves.empty()){ //not  calculated before
					moves = movGen->Generate(&OpponentRack, *board, board->GetCount()==0);
					for (int i = 0; i < (int)moves.size(); i++)
					{
						moves[i].SetPenalty(0);
						moves[i].SetRackLeave(0);
						moves[i].CalculateScore();
					}
					
					std::sort(moves.begin(), moves.end());
				    } 
				    if (moves.size() ==  0){ //no chosen word was found
				    	chosenMoveScore = 0;
				    	OppMoves = false;
				    }
				    else{
				    	chosenMove = moves[0];
				    	chosenMoveScore =  chosenMove.GetPlay()->GetScore();
				    	OppMoves = true;
				    }

                }
				
				
				//wait for opponent 0->actual move/1->pass/2->hint/3-exchange from GUI 
				//which is received in MoveType

                if (MoveType == 0){ //actual move
					//receive from GUI vector of WordGUI that has row, col, letter or each new letter on board
					//vector<WordGUI> wordVector; //= haga men GUI HumanMove
					
					Rack NewOpponent = OpponentRack;
                    for (int i=0;  i <  (int) HumanMove.size(); i++){
                        toSmall((HumanMove[i].letter));
                    }
					Play ActualPlay = Human.GetOpponentPlay(HumanMove, NewOpponent, boardTiles);
					if (ActualPlay.GetRow() == -1 && ActualPlay.GetColumn() == -1){
						FbMessage = "NO";
						//send to GUI that word is not vertical or horizontal, or there are gaps between new tiles
						//Still Opponent turn..
                        gui->Send(ConvertMessageHuman(5));
                        HumanMove.clear();
						continue;
					}
				
					string EnemyRack = OpponentRack.RackToString();

					if (board->GetCount() == 0){//first move, word should touch pos 7,7
						bool found = false;
						vector <Tile> tilesVector= ActualPlay.GetTiles();
						for (int i=0; i< (int) tilesVector.size(); i++){
							int row, col;
							tilesVector[i].GetIndex(row, col);
							if (row == 7 && col ==7){
								found = true;
							}
						}
						if (!found){
							FbMessage = "NO";
							//send to GUI that move should include pos 7,7
							//still opponent trun..
                            hintmove = Human.MoveToGui(chosenMove);
                            toCapital(hintmove);
					
                            gui->Send(ConvertMessageHuman(5));
							continue;
						}
					}
					
					if (!movGen->IsValidMove(ActualPlay, EnemyRack)){
						//send to GUI invalid move
						FbMessage = "NO";
						//still opponent turn..
                        hintmove = Human.MoveToGui(chosenMove);
                        toCapital(hintmove);
                        
                        gui->Send(ConvertMessageHuman(5));
						continue;
					}
					else{
						FbMessage ="YES";
                        hintmove = Human.MoveToGui(chosenMove);
                        toCapital(hintmove);
                      //  gui->Send(ConvertMessageHuman(5));
						//send to GUI valid
					}
				
					//Move is valid-> then calculate its score, take tiles from rack, put them on board
					 OpponentScore+= ActualPlay.GetScore();
					OpponentRack = NewOpponent;
					Human.AddPlayToBoard(ActualPlay, boardTiles);

					if (chosenMoveScore > ActualPlay.GetScore()){
						//send opponent a feedback through the GUI: you didnt choose best move, best move was
						//send best move
                        Best = "Less";
						hintmove = Human.MoveToGui(chosenMove);
                        toCapital(hintmove);
					}
					else if (chosenMoveScore == ActualPlay.GetScore()){
						//send opponent a feedback through the GUI: you chose the best move
						Best = "Congratulations";
					}
					else if (chosenMoveScore < ActualPlay.GetScore()){
						//send opponent a feedback through the GUI: you chose a better move than the evaluated
						Best = "Better";
					}    

					MyMoves = true;
					OppMoves= true;
					Human.SetOpponentRack(OpponentRack);
                    this->HumanRack = OpponentRack.RackToString();
                    toCapital(HumanRack);
					MyTurn= true;
					moves.clear();
                    gui->Send(ConvertMessageHuman(5));
				}
				else if (MoveType == 1){//pass
					MyTurn = true;
					moves.clear();
					//continue;
				}
				else if (MoveType == 2){//hint
					//send best move to GUI and wait for opponent to play, if oppMoves = false, send no possible moves
					
					MyTurn = false;
				    hintmove = Human.MoveToGui(chosenMove);
                    toCapital(hintmove);
                    gui->Send(ConvertMessageHuman(5));
					//no clear moves
					continue;
				}
				else if (MoveType == 3) { // Exchange tiles
					vector<char> toBeExchanged = { 'p','i'};
					vector<int> toBeExchangedLocations;
					Rack OpRack2(OpponentRack);
					for (int i = 0; i < (int)toBeExchanged.size(); i++)
					{
						/* code */
						toBeExchangedLocations.push_back(OpRack2.GetPosition(toBeExchanged[i]));
					}
					bag->swapRack(OpponentRack,toBeExchangedLocations);
					string opponentRackToGui="";
					for (int i = 0; i < (int)OpponentRack.GetLength(); i++)
					{
						opponentRackToGui+=OpponentRack.GetRackTiles()[i].GetLetter();
					}
					HumanRack = opponentRackToGui;
                    toCapital(HumanRack);
                    hintmove = Human.MoveToGui(chosenMove);
                    toCapital(hintmove);
                    gui->Send(ConvertMessageHuman(5));
					moves.clear();
					MyTurn = true;
				}





            // send the move to agent
            //gui->Send(ConvertMessageHuman(5));
            gui->Receive();
//MY TURNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN

 printBoard(board);

                int BagSize = (int)bag->GetRemainigLetters().size();
				moves = movGen->Generate(&AgentRack, *board, board->GetCount()==0);

				if (moves.size() > 0){
					if (BagSize > 9){//MidGame
						chosenMove = Human.MidGame(moves, syn2, worth, movGen); 
                    }
					else if (BagSize > 0 && BagSize <=9){
						chosenMove = Human.PreEndGame(syn2,worth, movGen,moves); //should return best move
					}
					else if (BagSize == 0){
						chosenMove = Human.EndGame(moves, syn2, worth, movGen, AgentRack, -1); //should return best move
					}
                       AgentScore+=chosenMove.GetPlay()->GetScore();
				}
				else{
					AgentMove SentMove = Human.PassMoveToGui(); //set -1 paramters
					MyMoves = false;
                    agentmove.score+=SentMove.score;
                    
					//Send to GUI SentMove which is pass
                    agentmove = SentMove;
                    toCapital(agentmove);
                    gui->Send(ConvertMessageHuman(2));
					MyTurn = false;
					continue;
				}
				//by here, there are moves
				Move PassMove = Human.GetPassMove();

				if (chosenMove.GetScore() < PassMove.GetScore()){ //pass was better than best move, but there are moves
					AgentMove SentMove= Human.PassMoveToGui(); //send O
					MyMoves = true;
					//Send to GUI SentMove which is pass
                    agentmove.score+=SentMove.score;
                    agentmove = SentMove;
                    toCapital(agentmove);
                    gui->Send(ConvertMessageHuman(2));
					MyTurn = false;
					continue;
				}
				MyMoves= true;
				OppMoves = true;

               

				agentmove = Human.MoveToGui(chosenMove);
                cout << "Agent Played: ";
                cout << agentmove.tiles << " " << agentmove.row << " " << agentmove.col << " " << agentmove.dir << endl;
                toCapital(agentmove);
				//send agent move to GUI
				//add word to board, remove letter from rack
				Human.UpdateBoardAndRack(*chosenMove.GetPlay(),  AgentRack);

				Human.SetMyRack(AgentRack);
                MyRack = AgentRack.RackToString();
                toCapital(MyRack);
                cout<<"sending to gui : "<<ConvertMessageHuman(2)<<endl;
                gui->Send(ConvertMessageHuman(2));
				moves.clear();
				MyTurn = false;


            //play
           
            }
        }
        gui->Receive();
    
    gui->Send(ConvertMessageHuman(4));  //terminate connection
}

string GameManager::ConvertIntRackToString(int tiles[7])
{   //converts the rack to a single string to send it
    // to the gui
    string rack = GetCorrespondigLetter(tiles[0]);
    std::cout<<"before conversion"<<std::endl;
    for (int i = 1; i < 7; i++)
    {
        std::cout<<(int)tiles[i]<<endl;
        rack = rack + GetCorrespondigLetter(tiles[i]);
    }
    return rack;
}

string GameManager::ConvertVecRackToString(vector<string> srack)
{   //converts the rack to a single string to send it
    // to the gui
    string rack = srack[0];
    for (int i = 1; i < (int)srack.size(); i++)
    {
        rack = rack + srack[i];
    }
    return rack;
}

string GameManager::GetCorrespondigLetter(int number)
{  
    if (number!=0){
    char letterchar = (char)64 +(int)number;
    string letter(1, letterchar);
    //std::cout<<"your converted rack to string is:" <<letter<<std::endl;
    return letter;
    }
    else
    return "0"; 
}


int GameManager::GetCorrespondigInt(char letter)
{  
    
    int intletter = 98-(int)letter;
   
    //std::cout<<"your converted rack to string is:" <<letter<<std::endl;
    return intletter;
    
  
}
/////////////////////////////////////

// start human mode

char *GameManager::ConvertMessageHuman(int type) // TO SEND THE MESSAGE TO GUI
{
    string tempmessage = "\0";
    if (type == 1)
    { // send the rack and the scores only in case of game init or game exchhange
        tempmessage = "1,0:00," + to_string(OpponentScore) + "," + to_string(AgentScore) + "," +HumanRack + ",";
    }
    else if (type==2){
        tempmessage = "2,0:00," + to_string(OpponentScore) + "," + to_string(AgentScore) + "," + HumanRack + ",";
        tempmessage = tempmessage +agentmove.tiles + "," + to_string(14-agentmove.row) + "," + to_string(agentmove.col) + "," + to_string(agentmove.dir) + ",";
    }
    else if (type==4){
        tempmessage = "-1";
    }
    else if (type==5){
        tempmessage = "5,0:00," + to_string(OpponentScore) + "," + to_string(AgentScore) + "," + HumanRack + ",";
        tempmessage = tempmessage + hintmove.tiles + "," + to_string(14-hintmove.row) + "," + to_string(hintmove.col) + "," + to_string(hintmove.dir) + ","+FbMessage+","+Best+",";
    }
    while (tempmessage.size() <= 49)
    {
        tempmessage = tempmessage + "0";
    }
    tempmessage = tempmessage + ",\0";
    string m="";
    char* message = new char[51];
    for (int i = 0; i < 50; i++)
    {
        message[i]=tempmessage[i];
    }
    message[50]='\0';
    
    return message;
}
void GameManager::InterpretMessage(char *message)
{ /// INTERPERT MESSAGE COME FROM THE GUI
    string passed(message);
    vector<string> Parameters = Split(passed);
    if (Parameters[0] == "0")
    {     
        //play
        MoveType=0;
        Parameters.erase(Parameters.begin());
        ConvertStringToMove(Parameters);
    }
    else if (Parameters[0] == "1")
    {  //exchange
        MoveType=3;
      ToExchange= Parameters[1];  
      toSmall(ToExchange);
    }
    else if (Parameters[0]=="2")
    {
         MoveType=1;
    }
    else{
          MoveType=2;
    }
}

void GameManager::ConvertStringToVector(string tiles){

    // for (int i=0;i<tiles.size();i++){
    //     string temp(1,tiles[i]);
    //     ToExchange="";
    // }

}

vector<string> GameManager::Split(string passed)
{
    vector<string> parameters;
    string temp = "";
    for (int i = 0; i < (int)passed.size(); i++)
    {
        if (passed[i] != ',')
        {
            temp = temp + passed[i];
        }
        else
        {
            parameters.push_back(temp);
            temp = "";
        }
    }
    return parameters;
}

void GameManager::ConvertStringToMove(vector<string> tiles)
{
    for (int i=0;i<(int)tiles.size();i++){
        WordGUI word;
        string tile=tiles[i];
        word.letter=tile[0];
        char rowString[] = {tile[1] , tile[2]};
        int rowInteger = atoi(rowString);
        word.row = rowInteger;
        char colString[] = {tile[3], tile[4]};
        int columnInteger = atoi(colString);
        word.col = columnInteger;
        HumanMove.push_back(word);
    }
 
}

void GameManager::toCapital(string &word){
    for (int i=0; i<(int)word.size(); i++){
        if (word[i] != '?'){
            word[i] -= 32;
        }
    }
}

void GameManager:: toSmall(string &word){
    for (int i=0; i<(int)word.size(); i++){
        if (word[i] != '?'){
            word[i] += 32;
        }
    }
}

void GameManager::toCapital(AgentMove &word){
    for (int i=0; i<(int)word.tiles.size(); i++){
        if (word.tiles[i] != '?'){
            word.tiles[i] -= 32;
        }
    }
}

void GameManager:: toSmall(AgentMove &word){
    for (int i=0; i<(int)word.tiles.size(); i++){
        if (word.tiles[i] != '?'){
            word.tiles[i] += 32;
        }
    }
}

GameManager::~GameManager()
{
}
