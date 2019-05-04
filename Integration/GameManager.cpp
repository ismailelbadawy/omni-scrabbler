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
    Comm::GetGame().Score = 0;
    Comm::GetGame().Opponent_Score = 0;
    // gui = new GUI();
    //gui->initConnection();
    //char *message = "0,0:00,RedArmy,Youssef,0000000000000000,\0";
    //gui->Send(message);
    client = new Comm();

    return 0;
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
        tempmessage = "2,0:00" + to_string(Comm::GetGame().Score) + "," + to_string(Comm::GetGame().Opponent_Score) + "," + ConvertIntRackToString(Comm::GetGame().Tiles) + ",";
        tempmessage = tempmessage + ConvertIntRackToString((int *)Comm::GetPlay().Tiles) + "," + to_string(Comm::GetPlay().Row) + "," + to_string(Comm::GetPlay().Column) + "," + to_string(Comm::GetPlay().Direction) + ",";
    }
    else if (type == 3)
    {   //send a rack with the opponent's move
        tempmessage = "3,0:00" + to_string(Comm::GetGame().Score) + "," + to_string(Comm::GetGame().Opponent_Score) + "," + ConvertIntRackToString(Comm::GetGame().Tiles) + ",";
        tempmessage = tempmessage + ConvertIntRackToString((int *)Comm::GetGame().OpponentMove.Tiles) + "," + to_string(Comm::GetGame().OpponentMove.Row) + "," + to_string(Comm::GetGame().OpponentMove.Column) + "," + to_string(Comm::GetGame().OpponentMove.Direction) + ",";
    }
    else if (type == 4)
    {   //terminate connection
        tempmessage = "-1";
    }
    while (tempmessage.size() != 39)
    {
        tempmessage = tempmessage + "0";
    }
    tempmessage = tempmessage + ",\0";
    message = const_cast<char *>(tempmessage.c_str());
    return message;
}

void GameManager::PlayHuman()
{
    // take the rack from the agent
    gui->Send(ConvertMessageHuman(1));
    while(MyRack.size()!=0 || HumanRack.size()!=0){
    char *HumanMove = gui->Receive(); // receive the play of the retarded human as a string
    InterpretMessage(HumanMove);
    // check the move and respond to it , send him his new rack,score and the best move, and a message
    gui->Send(ConvertMessageHuman(5));
    gui->Receive();
    InterpretMessage(HumanMove);
    // simulate my play
    gui->Send(ConvertMessageHuman(2));  // send my play to the gui with its score
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
    for (int i = 1; i < srack.size(); i++)
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
    char *message;
    string tempmessage = "\0";
    if (type == 1)
    { // send the rack and the scores only in case of game init or game exchhange
        tempmessage = "1,0:00," + to_string(Score) + "," + to_string(agentmove.score) + "," +HumanRack + ",";
    }
    else if (type==2){
        tempmessage = "2,0:00" + to_string(Score) + "," + to_string(agentmove.score) + "," + HumanRack + ",";
        tempmessage = tempmessage +agentmove.tiles + "," + to_string(agentmove.row) + "," + to_string(agentmove.col) + "," + to_string(agentmove.dir) + ",";
    }
    else if (type==4){
        tempmessage = "-1";
    }
    else if (type==5){
        tempmessage = "5,0:00" + to_string(Score) + "," + to_string(agentmove.score) + "," + HumanRack + ",";
        tempmessage = tempmessage + hintmove.tiles + "," + to_string(hintmove.row) + "," + to_string(hintmove.col) + "," + to_string(hintmove.dir) + ","+FbMessage+","+Best+",";
    }
    while (tempmessage.size() != 39)
    {
        tempmessage = tempmessage + "0";
    }
    tempmessage = tempmessage + ",\0";
    message = const_cast<char *>(tempmessage.c_str());
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
        ConvertStringToVector(Parameters[1]);  
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

    for (int i=0;i<tiles.size();i++){
        string temp(1,tiles[i]);
        ToExchange.push_back(temp);
    }

}

vector<string> GameManager::Split(string passed)
{
    vector<string> parameters;
    string temp = "";
    for (int i = 0; i <passed.size(); i++)
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

GameManager::~GameManager()
{
}
