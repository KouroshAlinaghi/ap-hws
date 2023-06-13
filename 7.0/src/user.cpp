#include <iostream>
#include <iomanip>
#include <algorithm>

#include "custom_exceptions/bad_request_exception.hpp"
#include "user.hpp"
#include "command.hpp"
#include "player.hpp"

using namespace std;

bool User::is_admin() {
    return false;
}

int User::get_players_sold_this_week() {
    return players_sold_this_week;
}

int User::get_players_bought_this_week() {
    return players_bought_this_week;
}

void User::reset_transfers_counts() {
    players_sold_this_week = 0;
    players_bought_this_week = 0;
}

bool User::has_player(Player* player_) {
    for (Player* player : squad)
        if (player == player_)
            return true;
    return false;
}

void User::sell_player(Player* player_) {
    vector<Player*> new_squad;
    if (player_ == captain) captain = nullptr;
    for (Player* player : squad)
        if (player != player_)
            new_squad.push_back((player));
    squad = new_squad;
    budget += player_->get_cost();
}
    
void User::inc_solds_count() {
    players_sold_this_week++;
}

int User::get_number_of_players(PLAYER_POSITION position) {
    int counter = 0;
    for (Player* player : squad)
        if (player->get_position() == position)
            counter++;
    return counter;
}

void User::buy_player(Player* player_) {
    if (player_->get_cost() > budget) throw BadRequestException();
    budget -= player_->get_cost();
    squad.push_back(player_);
    if (squad.size() == NUM_OF_PLAYERS_IN_SQUAD) 
        ever_had_completed_squad = true;
}
    
void User::inc_boughts_count() {
    players_bought_this_week++;
}

bool User::is_complted() {
    return squad.size() == NUM_OF_PLAYERS_IN_SQUAD;
}

vector<Player*> User::get_squad() {
    return squad;
}

double User::get_total_points() {
    return total_points;
}

void User::update_total_points() {
    for (Player* player : squad)
        total_points += (player == captain ? 2 : 1) * player->get_current_week_score();
}

bool User::had_completed_squad() {
    return ever_had_completed_squad;
}

void User::print_row(int index) {
    cout << index 
         << ". team_name: " << get_username() 
         << " | point: " << fixed << setprecision(1) << get_total_points()
         << endl;
}

void User::set_captain(Player* player) {
    captain = player;
}

int User::get_budget() {
    return budget;
}

int User::get_team_cost() {
    int cost = 0;
    for (Player* player : squad)
        cost += player->get_cost();
    return cost;
}

void User::print_squad() {
    vector<Player*> squad_ = get_squad();
    sort(squad_.begin(), squad_.end(), [](Player* p1, Player* p2) {
        if (p1->get_position() > p2->get_position()) return false;
        if (p1->get_position() < p2->get_position()) return true;
        return p1->get_name() < p2->get_name();
    });

    cout << "fantasy_team: " << get_username() << endl;
    for (int i = 0; i < (int)squad_.size(); i++)
        cout << POSITION_NAMES[i] << ": " << squad_[i]->get_name() 
             << (squad_[i] == captain ? " (CAPTAIN)" : "") << endl;

    cout << "Total Points: " << fixed << setprecision(1) << get_total_points() << endl;
    cout << "Team Cost: " << get_team_cost() << endl;
}
