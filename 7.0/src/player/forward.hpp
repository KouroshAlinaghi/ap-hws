#ifndef FORWARD_HEADERFILE
#define FORWARD_HEADERFILE

#include "../player.hpp"

const int FWD_INITIAL_PTS = 0;
const int FWD_SCORE_PTS = 3;
const int FWD_ASSIST_PTS = 1;
const int FWD_NO_SCORE_PTS = -1;

class Forward : public Player {
public:
    Forward(Club* team_, std::string name_, PLAYER_POSITION position_, int cost_) 
        : Player(team_, name_, position_, cost_) {}
    void print_with_details(int index);
    void calculate_score(Match* match);
};

#endif
