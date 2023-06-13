#ifndef DEFENDER_HEADERFILE
#define DEFENDER_HEADERFILE

#include "../player.hpp"

const int DEF_INITIAL_PTS = 1;
const int DEF_SCORE_PTS = 4;
const int DEF_ASSIST_PTS = 3;
const int DEF_CLEANSHEET_PTS = 2;

class Defender : public Player {
public:
    Defender(Club* team_, std::string name_, PLAYER_POSITION position_, int cost_) 
        : Player(team_, name_, position_, cost_) {}
    void print_with_details(int index);
    void calculate_score(Match* match);
};

#endif
