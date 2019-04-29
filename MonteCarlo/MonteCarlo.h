#pragma once
#include <iostream>
#include <vector>
#include <queue>
#include "../MoveGenerator/MoveGenerator.h"
#include "../Models/Board.h"
#include "../Models/Rack.h"
#include "../Models/Move.h"
#include "../Models/Bag.h"

//NodeState struct will contain meta data
struct NodeState
{
    int treeDepth;                //depth 0 and 2 represent my turn and depth 1 is the oponent's turn.
    vector<Move> possibleActions; //possible moves to play from the current state of the board.

    //paramaters used to calculate the UCT of the node.
    double reward, UCB; //currently eqauls to the score of the move only.
    int nbOfVisits;     //number of visits of the current node.
};

//The nodes of the tree will contain the current state of the game board.
struct NodeMC
{
    Board boardState; //current game state.
    Bag currentBag;
    Rack Rack;
    NodeMC *Parent;
    NodeState nodeState;
    vector<NodeMC *> children;
};

class MonteCarlo
{

private:
    //function to populate the first level.
    void firstLevel();
	MoveGenerator* movGen;
    //Use heuristics to caluclate the reward of a certain move.
    double calculateMoveReward(NodeMC *node);

    //Utility function to calculate the UCB.
    void calculateUCB(NodeMC *&node);

    //get the node with the best UCB.
    NodeMC *promisingNode(NodeMC *root);

    //backpropagation function.
    void Rollout(NodeMC *&node);

    //adds children to the node of choice.
    void Expand(NodeMC *&node);

public:
    //Root of the tree containing the current and main state of the game.
    NodeMC *Root;
    Rack mainRack;
    Rack oponentRack; //oponent RAck will be generated in the constructor.

    //constructor.
    MonteCarlo(Board boardState, vector<Move> Moves, Rack currentRack, Rack oponentRack, Bag bag, MoveGenerator *movGen);

    //adding new node to the tree.
    NodeMC *newNode(Board boardState, vector<Move> Moves, Rack currentRack, Bag bag, NodeMC *parent, int level);

    //traverse the tree.
    void LevelOrderTraversal(NodeMC *root);

    //Function to start the simulation.
    NodeMC *Simulation();
};