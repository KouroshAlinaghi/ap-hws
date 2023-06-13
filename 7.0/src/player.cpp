#include <iomanip>
#include <iostream>
#include <cmath>

#include "player.hpp"
#include "player_stats.hpp"

using namespace std;

string Player::get_name() {
    return name;
}

Club* Player::get_team() {
    return team;
}

PLAYER_POSITION Player::get_position() {
    return position;
}

void Player::get_injured() {
    weeks_left_until_injury_recovery = WEEKS_NEEDED_TO_RECOVER_INJURY;
    stats.back()->set_injury(true);
}

void Player::recieve_yellow_card() {
    yellow_cards_count++;
    stats.back()->set_yellow_card(true);
}

void Player::recieve_red_card() {
    stats.back()->set_red_card(true);
}

void Player::initialize_new_week_stats() {
    stats.push_back(new PlayerStats());
}

double Player::get_current_week_score() {
    if (stats.empty()) return 0;
    return stats.back()->get_score();
}

double Player::get_week_score(int week) {
    if (stats.empty()) return 0;
    return stats[week-1]->get_score();
}

string Player::get_position_shortened() {
    switch (position) {
        case GOALKEEPER:
            return "gk";
        case DEFENDER:
            return "df";
        case MIDFIELDER:
            return "md";
        default:
            return "fw";
    }
}

bool Player::is_available() {
    if (stats.empty()) return true;
    if (yellow_cards_count % 3 == 0 and stats.back()->had_yellow_card()) return false;
    if (stats.back()->had_red_card()) return false;
    if (weeks_left_until_injury_recovery) return false;
    return true;
}

void Player::recover_one_week() {
    weeks_left_until_injury_recovery = max(0, weeks_left_until_injury_recovery-1);
}

double Player::get_avg_score() {
    if (get_weeks_played() == 0) return 0;
    return get_total_points() / get_weeks_played();
}

int Player::get_weeks_played() {
    int c = 0;
    for (PlayerStats* w : stats)
        if (w->did_play())
            c++;
    return c;
}

void Player::print_week_details(int week) {
    cout << name << " | score: "<< fixed << setprecision(1) << get_week_score(week) << endl;
}

void Player::print_with_details(int index) {
    cout << index << ". name: " << name << " | role: " << get_position_shortened() << " | score: " << fixed << setprecision(1) << get_avg_score() << endl;
}

int Player::get_cost() {
    return cost;
}

void Player::score_goal() {
    stats.back()->score_goal();
}

void Player::score_assist() {
    stats.back()->score_assist();
}

void Player::score_own_goal() {
    stats.back()->score_own_goal();
}

void Player::set_no_cleansheet() {
    stats.back()->set_no_cleansheet();
}

int Player::get_cleansheets() {
    int counter = 0;
    for (PlayerStats* week : stats)
        if (week->did_play() and week->did_cleansheet())
            counter++;
    return counter;
}

int Player::get_goals() {
    int counter = 0;
    for (PlayerStats* week : stats)
        counter += week->get_goals();
    return counter;
}

int Player::get_assists() {
    int counter = 0;
    for (PlayerStats* week : stats)
        counter += week->get_assists();
    return counter;
}

void Player::play() {
    stats.back()->set_played(true);
}

int Player::get_last_week_goals() {
    return stats.back()->get_goals();
}

double Player::pts_fnc_calc(int raw_points) {
    return 10.0 * (1.0 / (1.0 + exp((double)-1*raw_points/6.0)));
}

double Player::get_total_points() {
    double pts = 0;
    for (PlayerStats* week : stats)
        pts += week->get_score();
    return pts;
}
