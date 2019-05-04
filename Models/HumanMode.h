#pragma once
#include "Board.h"
#include "Bag.h"
#include "Rack.h"
#include "Move.h"
#include "../MoveGenerator/MoveGenerator.h"
#include "../Evaluators/Evaluator.h"
#include "../Evaluators/MidgameEvaluator.h"
#include "../Evaluators/PreendgameEvaluator.h"
#include"../MonteCarlo/MonteCarlo.h"

class HumanMode{
    Board *board_;
    Bag *bag_;
    Rack *rack_;
    Rack *opponentRack_;

    Move *chosenMove_;

    TileScoreCalculator tilescorecalculator_;

    public:
    HumanMode(Board *, Bag*);
    Move MidGame(vector<Move>, map<string, double> * rackLeave, map<char, double> * charValue, MoveGenerator * movGen);
    Move PreEndGame(map<string, double> * syn2,map<char, double>* worth, MoveGenerator * movGen, vector <Move> moves);
    void EndGame(vector<Move>);

    void SetOpponentRack(Rack &rack);
    void SetMyRack(Rack &rack);

    Rack GetOpponentRack();

    Play GetOpponentPlay(bool horizontal, int row, int col, char *arr, int size, Rack &newoppRack, Tile* boardTiles[15][15]);
    void AddPlayToBoard(Play p, Tile* boardTiles[15][15]);

    void UpdateBoardAndRack(Play p, Rack &rack);

    Move GetChosenMove(); // Returns the proper move format to be sent (removes letters already on the board from the Play)
};
