
struct Message
{
// Message Type ----> Play = 0  ,  Exchange = 1  , Pass = 2
int MessageType_;

// Play
int Row;
int Col;
int Score;
int Direction;
int *Tiles;


// Exchange
int *TilesExchange;
};
