#ifndef MIDFIELDER_HEADERFILE
#define MIDFIELDER_HEADERFILE

#include "../player.hpp"

const int MD_INITIAL_PTS = 0;
const int MD_SCORE_PTS = 3;
const int MD_ASSIST_PTS = 2;
const int MD_CLEANSHEET_PTS = 1;

class Midfielder : public Player {
public:
    Midfielder(Club* team_, std::string name_, PLAYER_POSITION position_, int cost_) 
        : Player(team_, name_, position_, cost_) {}
    void print_with_details(int index);
    void calculate_score(Match* match);
};

#endif
