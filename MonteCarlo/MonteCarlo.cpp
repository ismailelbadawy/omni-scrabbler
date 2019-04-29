#include <iostream>
#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <fstream>

#include "./MonteCarlo.h"
#include "../Models/Bag.h"
#include "../Models/Tile.h"
#include "../Models/Play.h"
#include "../Models/Tile.h"
#include "../MoveGenerator/MoveGenerator.h"

using namespace std;

MonteCarlo::MonteCarlo(Board boardState, vector<Move> Moves, Rack currentRack, Rack oponentRack, Bag bag, MoveGenerator* movGen)
{
    NodeMC *temp = new NodeMC;
    Rack tempRack = currentRack;
	this->movGen = movGen;
    temp->boardState = boardState;
    temp->Rack = tempRack;
    temp->Parent = nullptr;

    temp->nodeState.treeDepth = 0;
    temp->nodeState.possibleActions = Moves;
    temp->nodeState.reward = 0;
    temp->nodeState.nbOfVisits = 0;

    temp->nodeState.UCB = 0;
    temp->currentBag = bag;

    vector<NodeMC *> children;
    temp->children = children;

    this->mainRack = tempRack;
    this->oponentRack = oponentRack;
    //populate the first level of actions.
    // i need the new rack of each state
    // board state
    // parent to be set to the root
    //depth
    //set of moves
    //
    this->Root = temp;
    //should generate the oponent Rack.
    firstLevel();
}

void MonteCarlo::firstLevel()
{
    //temp rack to use until we implement the random function.
    //we should use the bag to generate the random racks for each new state inside the loop.
    Tile RackTiles[7];

    RackTiles[0].SetLetter('f');
    RackTiles[0].SetScore(1);
    RackTiles[1].SetLetter('a');
    RackTiles[1].SetScore(4);
    RackTiles[2].SetLetter('l');
    RackTiles[2].SetScore(3);
    RackTiles[3].SetLetter('e');
    RackTiles[3].SetScore(2);
    RackTiles[4].SetLetter('m');
    RackTiles[4].SetScore(1);
    RackTiles[5].SetLetter('o');
    RackTiles[5].SetScore(4);
    RackTiles[6].SetLetter('r');
    RackTiles[6].SetScore(3);

    vector<Tile> rackTiles(RackTiles, RackTiles + sizeof RackTiles / sizeof RackTiles[0]);
    Rack rack(rackTiles);

    Board tempLevel1Board = this->Root->boardState;
    
    //loop over the number of possible actions to make in order to get all the possible states in the level.
    for (int i = 0; i < 10; i++)
    {

        tempLevel1Board = this->Root->boardState;
        //generate the first board state after my move.
        tempLevel1Board.SimulateMove(&(this->Root->nodeState.possibleActions[i]));

        //using the move generator to generate new set of moves for each action.
        vector<Move> nextMoves;

        nextMoves = this->movGen->Generate(&rack,this->Root->boardState, tempLevel1Board.GetCount() == 0);
        vector<Move> simVec;
        for (int i = 0; i < 10; i++)
        {
            simVec.push_back(nextMoves.at(i));
        }

        this->Root->children.push_back(newNode(tempLevel1Board, simVec, rack, Root->currentBag, Root, 1));
    }
	//Every child's board is the same now despite them having different boards really---Changed when updateBoard is made
    cout << "done making first level" << endl;
}

NodeMC *MonteCarlo::newNode(Board boardState, vector<Move> Moves, Rack currentRack, Bag bag, NodeMC *parent, int level)
{
    NodeMC *temp = new NodeMC;
    Rack tempRack = currentRack;

    temp->boardState = boardState;
    temp->Rack = tempRack;
    temp->Parent = parent;

    vector<NodeMC *> children;
    temp->children = children;

    temp->nodeState.treeDepth = level;
    temp->nodeState.possibleActions = Moves;

    //the reward should be the score of the move.
    temp->nodeState.reward = 0;
    temp->nodeState.nbOfVisits = 0;
    temp->nodeState.UCB = INT_MAX;

    return temp;
}

void MonteCarlo::LevelOrderTraversal(NodeMC *root)
{
    if (root == NULL)
        return;

    queue<NodeMC *> q;
    q.push(root);
    while (!q.empty())
    {
        int n = q.size();
        while (n > 0)
        {
            NodeMC *p = q.front();
            q.pop();
            Tile *tiles[15][15];
            p->boardState.GetTiles(tiles);

            cout << "child begin ... " << endl;
            for (int i = 0; i < 15; i++)
            {
                for (int j = 0; j < 15; j++)
                {
                    cout << tiles[i][j]->GetLetter() << " ";
                }
                cout << endl;
            }
            cout << "child end ..." << endl;
            for (int i = 0; i < p->children.size(); i++)
                q.push(p->children[i]);
            n--;
        }

        cout << endl;
    }
}

double MonteCarlo::calculateUCB(NodeMC *node)
{
    //calculate UCB
    return 0.0;
}

double MonteCarlo::calculateMoveReward(NodeMC *node)
{
    //i think it should be used while expansion.
    return 0.0;
}

NodeMC *MonteCarlo::promisingNode(NodeMC *root)
{
    NodeMC *Max = root->children.at(0);
    for (int i = 0; i < root->children.size(); i++)
    {
        if (root->children.at(i)->nodeState.UCB > Max->nodeState.UCB)
        {
            Max = root->children.at(i);
        }
    }
    return Max;
}

void MonteCarlo::Rollout(NodeMC *&node)
{
	
	switch (node->nodeState.treeDepth)
	{
	case 1:
		break;
	case 2:
		node->Parent->nodeState.UCB -= node->nodeState.UCB;
		break;
	case 3:
		node->Parent->nodeState.UCB -= node->nodeState.UCB; //Mutate second element
		node->Parent->Parent->nodeState.UCB -= node->Parent->nodeState.UCB;//mutate the top Element
	default:
		break;
	}
}

void MonteCarlo::Expand(NodeMC *&node)
{
    //Generate Random rack based on current bag ---Ismail and Walaa IIRC
    vector<NodeMC *> newstates;
	Board tempBoard = node->boardState;
	//Gets all possibles moves generated from current game state when we are on this specific node
	vector<Move> moves = movGen->Generate(&node->Rack, tempBoard, node->boardState.GetCount() == 0);
    for (size_t i = 0; i < 30; i++)
    {
        //Tweaks all the new states retrieved from the MoveGeneration & Static eval. so that expansion is complete
        //However Bag state? Rack state? Board state? How do I get those?
		//we Need updateBoard so that we can simulate the new levels
        newstates[i]->Parent = node;
        newstates[i]->nodeState.nbOfVisits = 0;
        newstates[i]->nodeState.treeDepth = node->nodeState.treeDepth + 1;
        newstates[i]->nodeState.UCB = INT_MAX;
		newstates[i]->
		//Random rack needed heree...newstates[i]->Rack=
		
    }
    //Finally append this new vector to the children of this node
    node->children = newstates;
}

NodeMC *MonteCarlo::Simulation()
{

    int i = 0;
    while (i < 3)
    {
        NodeMC *node = promisingNode(this->Root);

        cout << node->children.size() << endl;
        while (node->children.size() != 0)
        {
            node = promisingNode(node);
        }

        if (node->nodeState.nbOfVisits == 0)
        {
            node->nodeState.UCB = calculateUCB(node);
            //No rollout needed here cause the UCB would be infinity and would ruin the tree
			//Rollout(node);
        }
        else
        {
            if (node->nodeState.treeDepth < 3)
            {
                Expand(node);
                node = node->children.at(rand() % 30);
                node->nodeState.UCB = calculateUCB(node);
                Rollout(node);
            }
            else
            {
                node->nodeState.UCB = calculateUCB(node);
                Rollout(node);
            }
        }
    }

    return promisingNode(this->Root);
}