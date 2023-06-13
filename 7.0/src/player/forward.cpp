#include <iostream>
#include <iomanip>

#include "forward.hpp"
#include "../match.hpp"

using namespace std;

void Forward::print_with_details(int index) {
    cout << index << ". name: " << name 
         << " | role: " << get_position_shortened() 
         << " | score: " << fixed << setprecision(1) << get_avg_score() 
         << " | cost: " << get_cost() 
         << " | goals: " << get_goals() 
         << " | assists: " << get_assists() << endl;
}

void Forward::calculate_score(Match* match) {
    int raw_score = FWD_INITIAL_PTS;
    PlayerStats* current_stats = stats.back();

    if (!current_stats->did_play()) {
        current_stats->set_score(0);
        return;
    }
    
    if (!match->get_winner()) raw_score += DRAW_PTS;
    raw_score += match->get_winner() == get_team() ? WIN_PTS : LOSE_PTS;

    raw_score += current_stats->get_goals() * FWD_SCORE_PTS;
    raw_score += current_stats->get_assists() * FWD_ASSIST_PTS;
    raw_score += current_stats->get_ogs() * OWN_GOAL_PTS;

    if (!current_stats->get_goals()) raw_score += FWD_NO_SCORE_PTS;

    current_stats->set_score(pts_fnc_calc(raw_score));
}
