#include "GameManager.h"
#include <string>
#include <vector>


enum Opponent
{
    Human,
    AI
};

GameManager::GameManager()
{
}

int GameManager::InitGame()
{
    // get mode , if AI init server
    //Comm::GetGame().Score = 0;
    //Comm::GetGame().Opponent_Score = 0;
     gui = new GUI();
     gui->initConnection();
     char*mode= gui->Receive();
     cout << "MODE " <<mode<<endl;
     string tempString(mode);
     Mode= tempString;
    //char *message = "0,0:00,RedArmy,Youssef,0000000000000000,\0";
    //gui->Send(message);
    if (Mode=="AI"){
        client = new Comm();
    }
    return 0;
}
string GameManager::GetMode(){
    return Mode;
}

void GameManager::PlayAI(bool &ended)
{
    // if (Comm::StartAction)
    // {
    if (Comm::mystate == Comm::States::RECEIVE_RACK || Comm::mystate == Comm::States::EXCHANGE_RESPONSE)
    {
        //  gui->Send(ConvertMessage(1)); //send rack to gui
        // send rack to agent
        std::cout << "receive rack" << std::endl;
        //Comm::StartAction = false;
        //Comm::ActionFinished = true;
        Comm::mystate = Comm::States::IDLE;
    }
    else if (Comm::mystate == Comm::States::THINKING)
    {
        //call the agent's simulation function
        std::cout << "please enter your move" << std::endl;
        std::cin >> Comm::agentmove;
        if (Comm::agentmove == "PLAY")
        {
            ServerPlay mover;
            int y;
            std::cout << "enter column" << std::endl;
            std::cin >> y;
            mover.Column = static_cast<uint8_t>(y);
            std::cout << "column" << mover.Column << endl;
            std::cout << "enter row" << std::endl;
            std::cin >> y;
            mover.Row = static_cast<uint8_t>(y);
            std::cout << "row" << mover.Row << endl;
            std::cout << "enter direction" << std::endl;
            std::cin >> mover.Direction;
            std::cout << "enter titles" << std::endl;
            int count = 0;
            int x;
            while (count != 7)
            {
                std::cout << " Enter  " << count << ":";
                std::cin >> x;
                mover.Tiles[count] = static_cast<uint8_t>(x);
                count++;
            }
            std::cin >> mover.Score;
            Comm::SetPlay(mover);
        }
        else if (Comm::agentmove == "EXCHANGE")
        {
            TilesStruct extiles;
            int count = 0;
            int x;
            while (count != 7)
            {
                std::cout << " Enter  " << count << ":";
                std::cin >> x;
                extiles.Tiles[count] = static_cast<uint8_t>(x);
                count++;
            }
            Comm::SetExchanged(extiles);
        }

        Comm::simulationfinished = true;
        Comm::mystate = Comm::States::IDLE;
        // Comm::StartAction = false;
        // Comm::ActionFinished = true;
    }
    else if (Comm::mystate == Comm::States::PLAY_RESPONSE)
    {
        std::cout << "play response is trueeeeeeeeeee";
        // send the new rack to the agent
        //gui->Send(ConvertMessage(2)); // sending the move and the rack to the gui
        //Comm::StartAction = false;
        //Comm::ActionFinished = true;
        Comm::mystate = Comm::States::IDLE;
    }
    else if (Comm::mystate == Comm::States::PLAY_OPPONENT)
    {
        std::cout << "opponent played" << std::endl;

        // sending  the new board to the agent
        //gui->Send(ConvertMessage(3)); // send the rack and the opponent's move to gui
        Comm::StartAction = false;
        Comm::ActionFinished = true;
        Comm::mystate = Comm::States::THINKING;
        //  Comm::mystate=Comm::States::IDLE;
    }
    else if (Comm::mystate == Comm::States::EXCHANGE_OPPONENT)
    {
        std::cout << "opponent exchanged" << std::endl;
        // inform agent about the opponent's move
        //Comm::StartAction = false;
        //Comm::ActionFinished = true;
        //Comm::mystate=Comm::States::IDLE;
    }
    else if (Comm::mystate == Comm::States::PASS_OPPONENT)
    {
        std::cout << "opponent passed" << std::endl;
        // inform agent about the opponent's move
        // Comm::StartAction = false;
        //Comm::ActionFinished = true;
        Comm::mystate = Comm::States::THINKING;
        // Comm::mystate=Comm::States::IDLE;
    }
    else if (Comm::mystate == Comm::States::ENDCONNECTION)
    {
        //gui->Send(ConvertMessage(4)); // send terminate connection to gui
        ended = true;
    }
    // }
}

char *GameManager::ConvertMessageAI(int type)
{
    char *message;
    string tempmessage = "\0";
    if (type == 1)
    { // send the rack and the scores only in case of game init or game exchhange
        tempmessage = "1,0:00," + to_string(Comm::GetGame().Score) + "," + to_string(Comm::GetGame().Opponent_Score) + "," + ConvertIntRackToString(Comm::GetGame().Tiles) + ",";
    }
    else if (type == 2)
    {
        // send the rack with a move
        tempmessage = "2,0:00," + to_string(Comm::GetGame().Score) + "," + to_string(Comm::GetGame().Opponent_Score) + "," + ConvertIntRackToString(Comm::GetGame().Tiles) + ",";
        tempmessage = tempmessage + ConvertIntRackToString((int *)Comm::GetPlay().Tiles) + "," + to_string(Comm::GetPlay().Row) + "," + to_string(Comm::GetPlay().Column) + "," + to_string(Comm::GetPlay().Direction) + ",";
    }
    else if (type == 3)
    {   //send a rack with the opponent's move
        tempmessage = "3,0:00," + to_string(Comm::GetGame().Score) + "," + to_string(Comm::GetGame().Opponent_Score) + "," + ConvertIntRackToString(Comm::GetGame().Tiles) + ",";
        tempmessage = tempmessage + ConvertIntRackToString((int *)Comm::GetGame().OpponentMove.Tiles) + "," + to_string(Comm::GetGame().OpponentMove.Row) + "," + to_string(Comm::GetGame().OpponentMove.Column) + "," + to_string(Comm::GetGame().OpponentMove.Direction) + ",";
    }
    else if (type == 4)
    {   //terminate connection
        tempmessage = "-1";
    }
    while (tempmessage.size() != 49)
    {
        tempmessage = tempmessage + "0";
    }
    tempmessage = tempmessage + ",\0";
    message = const_cast<char *>(tempmessage.c_str());
    return message;
}

void GameManager::PlayHuman(Board *board, Bag *bag, MoveGenerator *movGen,  map<string, double>* syn2, map<char, double>* worth)
{  
        vector<Move> moves;	
        moves.clear();

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
            char *move=gui->Receive();//receive from GUI
            if(move[0]!='-' && move[1]!='1' && move[2]!=',' ){
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
                            gui->Send(ConvertMessageHuman(5));
							continue;
						}
					}
					
					if (!movGen->IsValidMove(ActualPlay, EnemyRack)){
						//send to GUI invalid move
						FbMessage = "NO";
						//still opponent turn..
                        gui->Send(ConvertMessageHuman(5));
						continue;
					}
					else{
						FbMessage ="YES";
						//send to GUI valid
					}
				
					//Move is valid-> then calculate its score, take tiles from rack, put them on board
					int OpponentScore = ActualPlay.GetScore();
					OpponentRack = NewOpponent;
					Human.AddPlayToBoard(ActualPlay, boardTiles);

					if (chosenMoveScore > OpponentScore){
						//send opponent a feedback through the GUI: you didnt choose best move, best move was
						//send best move
                        Best = "Less";
						hintmove = Human.MoveToGui(chosenMove);
                        toCapital(hintmove);
					}
					else if (chosenMoveScore == OpponentScore){
						//send opponent a feedback through the GUI: you chose the best move
						Best = "Congratulations";
					}
					else if (chosenMoveScore < OpponentScore){
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

                int BagSize = (int)bag->GetRemainigLetters().size();
				moves = movGen->Generate(&AgentRack, *board, board->GetCount()==0);

				if (moves.size() > 0){
					if (BagSize > 9){//MidGame
						chosenMove = Human.MidGame(moves, syn2, worth, movGen); //should return best move
					
					}
					else if (BagSize > 0 && BagSize <=9){
						chosenMove = Human.PreEndGame(syn2,worth, movGen,moves); //should return best move
					}
					else if (BagSize == 0){
						chosenMove = Human.EndGame(moves, syn2, worth, movGen, AgentRack, -1); //should return best move
					}
				}
				else{
					AgentMove SentMove = Human.PassMoveToGui(); //set -1 paramters
					MyMoves = false;
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
                    agentmove = SentMove;
                    toCapital(agentmove);
                    gui->Send(ConvertMessageHuman(2));
					MyTurn = false;
					continue;
				}
				MyMoves= true;
				OppMoves = true;

				agentmove = Human.MoveToGui(chosenMove);
                toCapital(agentmove);
				//send agent move to GUI
				//add word to board, remove letter from rack
				Human.UpdateBoardAndRack(*chosenMove.GetPlay(),  AgentRack);

				Human.SetMyRack(AgentRack);
                MyRack = AgentRack.RackToString();
                toCapital(MyRack);
                gui->Send(ConvertMessageHuman(2));
				moves.clear();
				MyTurn = false;


            //play
           
            }
        }
    
    gui->Send(ConvertMessageHuman(4));  //terminate connection
}

string GameManager::ConvertIntRackToString(int tiles[7])
{   //converts the rack to a single string to send it
    // to the gui
    string rack = GetCorrespondigLetter(tiles[0]);
    for (int i = 1; i < 7; i++)
    {
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
    char letterchar = (char)64 + number;
    string letter(1, letterchar);
    return letter;
}
/////////////////////////////////////

// start human mode

char *GameManager::ConvertMessageHuman(int type) // TO SEND THE MESSAGE TO GUI
{
    string tempmessage = "\0";
    if (type == 1)
    { // send the rack and the scores only in case of game init or game exchhange
        tempmessage = "1,0:00," + to_string(Score) + "," + to_string(agentmove.score) + "," +HumanRack + ",";
    }
    else if (type==2){
        tempmessage="2,00.02,123,898,HYRKOFK,FATMAKL,07,01,0,";
    }
    else if (type==4){
        tempmessage = "-1";
    }
    else if (type==5){
        tempmessage = "5,0:00," + to_string(Score) + "," + to_string(agentmove.score) + "," + HumanRack + ",";
        tempmessage = tempmessage + hintmove.tiles + "," + to_string(hintmove.row) + "," + to_string(hintmove.col) + "," + to_string(hintmove.dir) + ","+FbMessage+","+Best+",";
    }
    while (tempmessage.size() != 49)
    {
        tempmessage = tempmessage + "0";
    }
    tempmessage = tempmessage + ",\0";
    char* message = new char[50];
    for (int i = 0; i < 50; i++)
    {
        message[i]=tempmessage[i];
    }
    
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
        word.row=tile[1];
        word.col=tile[2];
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
