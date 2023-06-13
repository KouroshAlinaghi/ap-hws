#include <iostream>
#include <utility>
#include <vector>

#include "general.hpp"
#include "match.hpp"

using namespace std;

int Match::get_week() {
    return week;
}

void Match::print_result() {
    cout << home->get_name() << " " << result.first << " | " << away->get_name() << " " << result.second << endl;
}

void Match::add_home_squad(Player* player) {
    home_squad.push_back(player);
}

void Match::add_away_squad(Player* player) {
    away_squad.push_back(player);
}

void Match::record_score(Player* scorer, Player* assister) {
    goal_assists.push_back(make_pair(scorer, assister));
}

vector<Player*> Match::get_players() {
    vector<Player*> res;
    for (Player* player : home_squad)
        res.push_back(player);
    for (Player* player : away_squad)
        res.push_back(player);
    return res;
}

Club* Match::get_winner() {
    if (result.first > result.second) return home;
    if (result.first < result.second) return away;
    return nullptr;
}

int Match::get_opposite_scores(Player* player) {
    SQUAD_POSITIONS cur_pos = GK;
    for (int p = GK; p < CLUB_SQUAD_CNT; p++) {
        if (home_squad[p] == player) {
            cur_pos = (SQUAD_POSITIONS)p;
            break;
        }
        if (away_squad[p] == player) {
            cur_pos = (SQUAD_POSITIONS)p;
            break;
        }
    }
    vector<SQUAD_POSITIONS> opp_pos_indexes = get_opp_indexes(cur_pos);
    int cnt = 0;
    for (SQUAD_POSITIONS position : opp_pos_indexes)
        cnt += (player->get_team() == home) ? 
            away_squad[position]->get_last_week_goals() : home_squad[position]->get_last_week_goals();
    return cnt;
}

int Match::get_oponent_team_scores(Player* player) {
    return player->get_team() == home ? result.first : result.second;
}
