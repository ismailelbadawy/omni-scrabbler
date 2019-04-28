#pragma once
#include <iostream>
#include <vector>
#include <queue>

#include "../Models/Board.h"
#include "../Models/Rack.h"
#include "../Models/Move.h"
#include "../Models/Bag.h"

//NodeState struct will contain meta data
struct NodeState
{
    int treeDepth;       //depth 0 and 2 represent my turn and depth 1 is the oponent's turn.
    vector<Move> possibleActions; //possible moves to play from the current state of the board.
    
    //paramaters used to calculate the UCT of the node.
    double reward,UCB; //currently eqauls to the score of the move only.
    int nbOfVisits; //number of visits of the current node.
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
    
    //Utility function to calculate the UCB.
    double calculateUCB(NodeMC *node);

    //get the node with the best UCB.
    NodeMC *promisingNode(NodeMC *root);

public:
    //Root of the tree containing the current and main state of the game.
    NodeMC *Root;
    Rack mainRack;

    //constructor.
    MonteCarlo(Board boardState,vector<Move> Moves,Rack currentRack,Bag bag);

    //adding new node to the tree.
    NodeMC *newNode(Board boardState, vector<Move> Moves, Rack currentRack, Bag bag, NodeMC *parent, int level);

    //traverse the tree.
    void LevelOrderTraversal(NodeMC *root);
	void Expand(NodeMC*& node);
    


    //Function to start the simulation.
    NodeMC *Simulation();
};