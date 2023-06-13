#include <iostream>
#include <algorithm>
#include <vector>
#include <iomanip>

#include "account.hpp"
#include "command.hpp"
#include "custom_exceptions/bad_request_exception.hpp"
#include "custom_exceptions/empty_exception.hpp"
#include "custom_exceptions/not_found_exception.hpp"
#include "custom_exceptions/permission_denied_exception.hpp"
#include "player.hpp"
#include "general.hpp"

using namespace std;

string Command::get_name() {
    return name;
}

HTTP_VERB Command::get_verb() {
    return verb;
}

void Command::execute(Database* db, Session* cookie, Arguments args) {
    exec(db, cookie, args);
}

void get_team_of_the_week(Database* db, Session* cookie, Arguments args) {
    if (args.empty() or args[0] != "?" or (args.size() == 3 and args[1] != "week_num") or (args.size() != 1 and args.size() != 3))
        throw BadRequestException();

    int week = args.size() == 3 ? stoi(args[2]) : cookie->get_current_week();   
    if (week <= 0 or week > cookie->get_current_week()) throw BadRequestException();
    
    vector<vector<Player*>> chosen_players;
    for (PLAYER_POSITION position : POSITIONS_IN_ORDER) 
        chosen_players.push_back(db->get_best_players(position, PLAYERS_COUNT_IN_SQUAD[position], week));
    
    cout << "team of the week:" << endl;
    cout << "Goalkeeper: "; 
    chosen_players[0][0]->print_week_details(week);
    cout << "Defender 1: "; 
    chosen_players[1][0]->print_week_details(week);
    cout << "Defender 2: ";
    chosen_players[1][1]->print_week_details(week);
    cout << "Midfielder: ";
    chosen_players[2][0]->print_week_details(week);
    cout << "Forward: ";
    chosen_players[3][0]->print_week_details(week);
}

void get_players(Database* db, Session*, Arguments args) {
    if (args.empty() or args[0] != "?") throw BadRequestException();
    if (args.size() == 3 and args[1] != "team_name") throw BadRequestException();
    if (args.size() != 1 and args.size() != 3 and args.size() != 4 and args.size() != 5) throw BadRequestException();

    string team_name = replace_string(args[2], '_', ' ');
    Club* team = db->get_club(team_name);
    if (!team) throw NotFoundException();
    bool sort_by_rank = args.back() == "ranks";
    bool filter_by_position = false;
    int pos_index = -1;
    switch (args.size()) {
        case 4:
            if (!sort_by_rank) pos_index = 3;
            break;
        case 5:
            pos_index = 3;
            break;
        default:
            pos_index = -1;
    }
    filter_by_position = pos_index != -1;
    PLAYER_POSITION position;
    vector<Player*> players = db->get_players_of_club(team);
    if (filter_by_position) {
        position = shortened_to_position(args[pos_index]);
        vector<Player*> filtered_players;
        for (Player* player : players) 
            if (player->get_position() == position)
                filtered_players.push_back(player);
        players = filtered_players;
    }

    if (sort_by_rank)
        sort(players.begin(), players.end(), [](Player* p1, Player* p2) { return p1->get_avg_score() > p2->get_avg_score(); });

    for (int i = 0; i < (int)players.size(); i++)
        players[i]->print_with_details(i+1);
}

void get_league_standings(Database* db, Session*, Arguments) {
    vector<Club*> teams = db->get_clubs_sorted();
    cout << "league standing:" << endl;
    for (int i = 0; i < (int)teams.size(); i++)
        teams[i]->print_row(i+1);
}

void get_users_ranking(Database* db, Session*, Arguments args) {
    if (args.size() != 0) throw BadRequestException();

    vector<User*> users = db->get_users();
    if (!users.size()) throw EmptyException();

    sort(users.begin(), users.end(), [](User* u1, User* u2) { 
        if (u1->get_total_points() == u2->get_total_points())
            return u1->get_username() < u2->get_username();
        return u1->get_total_points() > u2->get_total_points();  
    });

    for (int i = 0; i < (int)users.size(); i++) 
        users[i]->print_row(i+1);
}

void get_matches_result(Database* db, Session* cookie, Arguments args) {
    if ((args.size() == 3 and args[1] != "week_num") or (args.size() != 3 and args.size() != 1) or args[0] != "?")
        throw BadRequestException();
    
    int week = args.size() == 3 ? stoi(args[2]) : cookie->get_current_week();   
    if (week <= 0 or week > cookie->get_current_week()) throw BadRequestException();

    for (Match* match : db->get_matches(week))
        match->print_result();
}

void signup(Database* db, Session* cookie, Arguments args) {
    if (args[0] != "?" or args[1] != "team_name" or args[3] != "password") throw BadRequestException();
    cookie->ensure_user_not_logged_in();
    
    Account* found_user = db->get_user(args[2]);
    if (found_user) throw BadRequestException();
    
    Account* new_user = db->create_user(args[2], args[4]);
    cookie->set_current_user(new_user);
    
    cout << "OK" << endl;
}

void login(Database* db, Session* cookie, Arguments args) {
    if (args[0] != "?" or args[1] != "team_name" or args[3] != "password") throw BadRequestException();
    cookie->ensure_user_not_logged_in();

    Account* found_user = db->get_user(args[2]);
    if (!found_user) throw NotFoundException();
    if (!found_user->check_password(args[4])) throw PermissionDeniedException();
    
    cookie->set_current_user(found_user);
    
    cout << "OK" << endl;
}

void register_admin(Database* db, Session* cookie, Arguments args) {
    if (args[0] != "?" or args[1] != "username" or args[3] != "password") throw BadRequestException();
    cookie->ensure_user_not_logged_in();

    Admin* found_admin = db->get_admin(args[2]);
    if (!found_admin) throw BadRequestException();
    if (!found_admin->check_password(args[4])) throw BadRequestException();
    
    cookie->set_current_user(found_admin);

    cout << "OK" << endl;
}

void logout(Database*, Session* cookie, Arguments) {
    cookie->ensure_user_logged_in();
    cookie->set_current_user(nullptr);

    cout << "OK" << endl;
}

void sell_player(Database* db, Session* cookie, Arguments args) {
    if (args.size() < 3 or args[0] != "?" or args[1] != "name") throw BadRequestException();
    if (!cookie->is_transfer_window_open()) throw PermissionDeniedException();
    cookie->ensure_user_logged_in();
    if (cookie->get_current_user()->is_admin()) throw PermissionDeniedException();

    User* current_user = (User*)cookie->get_current_user();
    Player* player = db->get_player(get_player_name(args));

    if (!player) throw NotFoundException();
    if (!current_user->has_player(player)) throw NotFoundException();
    if (current_user->get_players_sold_this_week() >= MAX_SELLS and current_user->had_completed_squad()) throw PermissionDeniedException();

    current_user->sell_player(player);
    current_user->inc_solds_count();

    cout << "OK" << endl;
}

void buy_player(Database* db, Session* cookie, Arguments args) {
    if (args.size() < 3 or args[0] != "?" or args[1] != "name") throw BadRequestException();
    if (!cookie->is_transfer_window_open()) throw PermissionDeniedException();
    cookie->ensure_user_logged_in();
    if (cookie->get_current_user()->is_admin()) throw PermissionDeniedException();

    User* current_user = (User*)cookie->get_current_user();
    Player* player = db->get_player(get_player_name(args));

    if (!player) throw NotFoundException();
    if (current_user->has_player(player)) throw BadRequestException();
    if (current_user->get_players_bought_this_week() >= MAX_BUYS and current_user->had_completed_squad()) throw PermissionDeniedException();
    if (current_user->get_number_of_players(player->get_position()) >= PLAYERS_COUNT_IN_SQUAD[player->get_position()])
        throw BadRequestException();

    if (!player->is_available()) {
        cout << "This player is not available for the next week" << endl;
        return;
    }

    current_user->buy_player(player);
    current_user->inc_boughts_count();

    cout << "OK" << endl;
}

void get_squad(Database* db, Session* cookie, Arguments args) {
    if ((args.size() == 3 and args[1] != "fantasy_team") or (args.size() != 3 and args.size() != 1) or args[0] != "?")
        throw BadRequestException();
    cookie->ensure_user_logged_in();

    User* team = args.size() == 3 ? db->get_user(args[2]) : (User*)cookie->get_current_user();
    if (!team) throw NotFoundException();
    if (team->is_admin()) throw PermissionDeniedException();
    if (!team->is_complted()) throw EmptyException();

    team->print_squad();
}

void close_transfer_window(Database*, Session* cookie, Arguments) {
    cookie->ensure_admin_logged_in();
    cookie->set_transfer_window(false);

    cout << "OK" << endl;
}

void open_transfer_window(Database*, Session* cookie, Arguments) {
    cookie->ensure_admin_logged_in();
    cookie->set_transfer_window(true);

    cout << "OK" << endl;
}

void pass_week(Database* db, Session* cookie, Arguments) {
    cookie->ensure_admin_logged_in();

    db->tell_player_the_week_passed();
    db->reset_transfers_counts();
    db->read_week_stats(cookie->get_current_week()+1);
    db->calculate_players_points();
    db->calculate_teams_points();
    cookie->pass_week();

    cout << "OK" << endl;
}

void set_captain(Database* db, Session* cookie, Arguments args) {
    if (args.size() < 3 or args[0] != "?" or args[1] != "name") throw BadRequestException();

    cookie->ensure_user_logged_in();
    User* current_user = (User*)cookie->get_current_user();
    if (current_user->is_admin()) throw PermissionDeniedException();

    string player_name = get_player_name(args);
    Player* player = db->get_player(player_name);
    if (!player) throw NotFoundException();
    if (!current_user->has_player(player)) throw NotFoundException();

    current_user->set_captain(player);

    cout << "OK" << endl;
}

void show_budget(Database*, Session* cookie, Arguments args) {
    if (!args.empty()) throw BadRequestException();

    cookie->ensure_user_logged_in();
    User* current_user = (User*)cookie->get_current_user();
    if (current_user->is_admin()) throw PermissionDeniedException();
    
    cout << current_user->get_budget() << endl;
}
