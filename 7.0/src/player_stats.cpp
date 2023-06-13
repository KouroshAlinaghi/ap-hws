#include <iostream>

#include "player_stats.hpp"

using namespace std;

void PlayerStats::set_injury(bool is_injured) {
    injured = is_injured;
}

void PlayerStats::set_yellow_card(bool y_c) {
    yellow_card = y_c;
}

void PlayerStats::set_red_card(bool r_c) {
    red_card = r_c;
}

void PlayerStats::set_score(double score_) {
    score = score_;
}

void PlayerStats::set_played(bool played_) {
    played = played_;
}

double PlayerStats::get_score() {
    return score;
}

bool PlayerStats::had_yellow_card() {
    return yellow_card;
}

bool PlayerStats::had_red_card() {
    return red_card;
}

bool PlayerStats::did_play() {
    return played;
}

void PlayerStats::score_goal() {
    goals++;
}

void PlayerStats::score_assist() {
    assists++;
}

void PlayerStats::score_own_goal() {
    own_goals++;
}

void PlayerStats::set_no_cleansheet() {
    cleansheet = false;
}

bool PlayerStats::did_cleansheet() {
    return cleansheet;
}

int PlayerStats::get_goals() {
    return goals;
}

int PlayerStats::get_assists() {
    return assists;
}

int PlayerStats::get_ogs() {
    return own_goals;
}
