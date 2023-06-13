#ifndef MATCH_HEADERFILE
#define MATCH_HEADERFILE

#include "club.hpp"
#include "player.hpp"
#include <vector>

const int CLUB_SQUAD_CNT = 11;

typedef std::pair<int, int> Result;

enum SQUAD_POSITIONS {
    GK, 
    LB, LCB, RCB, RB,
    LM, CDM, RM,
    LW, ST, RW
};

class Match {
private:
    Club* home;
    Club* away;
    Result result;
    int week;
    std::vector<std::pair<Player*, Player*>> goal_assists;
    std::vector<Player*> home_squad;
    std::vector<Player*> away_squad;
public:
    Match(Club* home_, Club* away_, Result result_, int week_) {
        home = home_;
        away = away_;
        result = result_;
        week = week_;
    }
    int get_week();
    void print_result();
    void add_home_squad(Player*);
    void add_away_squad(Player*);
    void record_score(Player*, Player*);
    std::vector<Player*> get_players();
    Club* get_winner();
    int get_opposite_scores(Player* player);
    int get_oponent_team_scores(Player* player);
};

#endif
