#include <iostream>
#include <iomanip>

#include "goalkeeper.hpp"
#include "../match.hpp"

using namespace std;

void Goalkeeper::print_with_details(int index) {
    cout << index << ". name: " << name 
         << " | role: " << get_position_shortened() 
         << " | score: " << fixed << setprecision(1) << get_avg_score() 
         << " | cost: " << get_cost() 
         << " | clean sheets: " << get_cleansheets() << endl;
}

void Goalkeeper::calculate_score(Match* match) {
    int raw_score = GK_INITIAL_PTS;
    PlayerStats* current_stats = stats.back();

    if (!current_stats->did_play()) {
        current_stats->set_score(0);
        return;
    }
    
    if (!match->get_winner()) raw_score += DRAW_PTS;
    raw_score += match->get_winner() == get_team() ? WIN_PTS : LOSE_PTS;

    raw_score += current_stats->get_ogs() * OWN_GOAL_PTS;
    if (current_stats->did_cleansheet()) raw_score += GK_CLEANSHEET_PTS;

    raw_score += match->get_oponent_team_scores(this) * GK_GOAL_CONCEDE_PTS;

    current_stats->set_score(pts_fnc_calc(raw_score));
}
