#ifndef GOALKEEPER_HEADERFILE
#define GOALKEEPER_HEADERFILE

#include "../player.hpp"

const int GK_INITIAL_PTS = 3;
const int GK_CLEANSHEET_PTS = 5;
const int GK_GOAL_CONCEDE_PTS = -1;

class Goalkeeper : public Player {
public:
    Goalkeeper(Club* team_, std::string name_, PLAYER_POSITION position_, int cost_) 
        : Player(team_, name_, position_, cost_) {}
    void print_with_details(int index);
    void calculate_score(Match* match);
};

#endif
