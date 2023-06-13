#include <iostream>
#include <fstream>
#include <string>
#include <utility>
#include <vector>
#include <algorithm>

#include "account.hpp"
#include "custom_exceptions/bad_request_exception.hpp"
#include "database.hpp"
#include "input.hpp"
#include "match.hpp"
#include "player.hpp"
#include "user.hpp"
#include "admin.hpp"
#include "player/goalkeeper.hpp"
#include "player/defender.hpp"
#include "player/midfielder.hpp"
#include "player/forward.hpp"

using namespace std;

Club* Database::create_club(string name) {
    Club* new_club = new Club(name);
    clubs.push_back(new_club);
    return new_club;
}

Player* Database::create_player(string name, Club* club, PLAYER_POSITION position, int cost) {
    Player* new_player;
    switch (position) {
        case GOALKEEPER:
            new_player = new Goalkeeper(club, name, position, cost);
            break;
        case DEFENDER:
            new_player = new Defender(club, name, position, cost);
            break;
        case MIDFIELDER:
            new_player = new Midfielder(club, name, position, cost);
            break;
        case FORWARD:
            new_player = new Forward(club, name, position, cost);
            break;
    }
    players.push_back(new_player);
    return new_player;
}

Match* Database::create_match(Club* home, Club* away, Result res, int week) {
    Match* new_match = new Match(home, away, res, week);
    matches.push_back(new_match);
    return new_match;
}

Club* Database::get_club(string name) {
    for (Club* club : clubs)
        if (club->get_name() == name)
            return club;
    return nullptr;
}

Player* Database::get_player(string name) {
    for (Player* player : players)
        if (player->get_name() == name)
            return player;
    return nullptr;
}

void Database::read_league_csv() {
    string line;
    Club* current_club;
    Player* player;
    vector<string> parts, words, player_and_cost;
    ifstream league_csv_file(LEAGUE_CSV_FILE_PATH);
    getline(league_csv_file, line);
    while (getline(league_csv_file, line)) {
        parts = split(line, ',');
        for (int i = 0; i < (int)parts.size(); i++) {
            if (i == 0) {
                current_club = create_club(parts[0]);
                continue;
            }
            words = split(parts[i], ';');
            for (string word : words) {
                player_and_cost = split(word, ':');
                player_and_cost.back().pop_back();
                player = create_player(player_and_cost[0], current_club, POSITIONS_IN_ORDER[i-1], stoi(player_and_cost[1]));
                current_club->add_player(player);
            }
        }
    }
    league_csv_file.close();
}

vector<Player*> Database::get_players() {
    return players;
}

void Database::update_standings_stats(Club* home, Club* away, Result res) {
    home->inc_goals_scored(res.first);
    away->inc_goals_scored(res.second);
    home->inc_goals_conceded(res.second);
    away->inc_goals_conceded(res.first);
    if (res.first > res.second) {
        home->inc_points(3);
    } else if (res.first < res.second) {
        away->inc_points(3);
    } else {
        home->inc_points(1);
        away->inc_points(1);
    }
}

void Database::read_week_stats(int week) {
    const char DELIMITERS[10] = {':', ':', ';', ';', ';', ';', ';', ';', ';'};
    const WEEK_STATS_COLUMNS COLUMNS[10] = { MATCH, RESULT, INJURED, YELLOW_CARD, RED_CARD, GOALS_AND_ASSISTS, HOME_SQUAD, AWAY_SQUAD };
    vector<string> parts, words, player_scores, goals_and_assists;
    Club *home, *away;
    Match* match;
    Player *scorer, *assister;
    Result res;
    ifstream week_stats_file(WEEK_STATS_CSV_FILE_PATH + "_" + to_string(week) + ".csv");
    string line;
    getline(week_stats_file, line);
    while (getline(week_stats_file, line)) {
        parts = split(line, ',');
        for (WEEK_STATS_COLUMNS c : COLUMNS) {
            words = split(parts[c], DELIMITERS[c]);
            switch (c) {
                case MATCH:
                    home = get_club(words[0]), away = get_club(words[1]);
                    break;
                case RESULT:
                    res = make_pair(stoi(words[0]), stoi(words[1]));
                    match = create_match(home, away, res, week);
                    update_standings_stats(home, away, res);
                    break;
                case INJURED:
                    for (string player_name : words)
                        get_player(player_name)->get_injured();
                    break;
                case YELLOW_CARD:
                    for (string player_name : words)
                        get_player(player_name)->recieve_yellow_card();
                    break;
                case RED_CARD:
                    for (string player_name : words)
                        get_player(player_name)->recieve_red_card();
                    break;
                case GOALS_AND_ASSISTS:
                    for (string scorer_and_assister : words) {
                        goals_and_assists = split(scorer_and_assister, ':');
                        scorer = get_player(goals_and_assists[0]);
                        assister = goals_and_assists[1] == OWN_GOAL ? nullptr : get_player(goals_and_assists[1]);
                        match->record_score(scorer, assister);
                        if (assister) {
                            scorer->score_goal();
                            assister->score_assist();
                            Club* team_conceded = scorer->get_team() == home ? away : home;
                            team_conceded->set_no_cleansheet();
                        } else {
                            scorer->get_team()->set_no_cleansheet();
                            scorer->score_own_goal();
                        }
                    }
                    break;
                case HOME_SQUAD:
                    for (string player_name : words) {
                        get_player(player_name)->play();
                        match->add_home_squad(get_player(player_name));
                    }
                    break;
                case AWAY_SQUAD:
                    for (string player_name : words) {
                        get_player(player_name)->play();
                        match->add_away_squad(get_player(player_name));
                    }
                    break;
            }
        }
    }
    week_stats_file.close();
}

vector<Club*> Database::get_clubs_sorted() {
    vector<Club*> res(clubs);
    sort(res.begin(), res.end(), [](Club* c1, Club* c2) {
        if (c1->get_points() > c2->get_points()) return true;
        if (c1->get_points() < c2->get_points()) return false;
        if (c1->get_goals_diff() > c2->get_goals_diff()) return true;
        if (c1->get_goals_diff() < c2->get_goals_diff()) return false;
        if (c1->get_goals_scored() == c2->get_goals_scored()) return c1->get_name() < c2->get_name();
        return c1->get_goals_scored() > c2->get_goals_scored();
    });
    return res;
}

User* Database::get_user(string username) {
    for (User* user : users) 
        if (user->get_username() == username)
            return user;
    return nullptr;
}

Admin* Database::get_admin(string username) {
    for (Admin* admin : admins) 
        if (admin->get_username() == username)
            return admin;
    return nullptr;
}

User* Database::create_user(string username, string password) {
    User* new_user = new User(username, password);
    users.push_back(new_user);
    return new_user;
}

Admin* Database::create_admin() {
    Admin* admin = new Admin(ADMIN_USERNAME, ADMIN_PASSWORD);
    admins.push_back(admin);
    return admin;
}

vector<Player*> Database::get_best_players(PLAYER_POSITION position, int count, int week) {
    vector<Player*> res;
    for (Player* player : players) 
        if (player->get_position() == position)
            res.push_back(player);

    sort(res.begin(), res.end(), [=](Player* p1, Player* p2) { return p1->get_week_score(week) < p2->get_week_score(week); });
    return vector<Player*>(res.end()-count, res.end());
}

vector<Match*> Database::get_matches(int week) {
    vector<Match*> res;
    for (Match* match : matches) 
        if (match->get_week() == week)
            res.push_back(match);
    return res;
}

vector<Player*> Database::get_players_of_club(Club* club) {
    vector<Player*> res;
    for (Player* player : players)
        if (player->get_team() == club)
            res.push_back(player);
    return res;
}

void Database::tell_player_the_week_passed() {
    for (Player* player : players) {
        player->initialize_new_week_stats();
        player->recover_one_week();
    }
}

void Database::reset_transfers_counts() {
    for (User* user: users)
        user->reset_transfers_counts();
}

void Database::calculate_players_points() {
    for (Match* match : matches)
        for (Player* player : match->get_players())
            player->calculate_score(match);
}

void Database::calculate_teams_points() {
    for (User* user: users)
        if (user->is_complted())
            user->update_total_points();
}

vector<User*> Database::get_users() {
    return users;
}
