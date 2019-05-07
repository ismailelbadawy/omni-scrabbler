#include <map>

using namespace std;

class TileScoreCalculator{  
    map<char, int> scoreMap_;

    public:
    TileScoreCalculator();
    ~TileScoreCalculator();
    int GetTileScore(char c);
};// da ? asl mesh ana el 3amlah sawany tayyeb