#ifndef DATABASE_HEADERFILE
#define DATABASE_HEADERFILE

#include <vector>
#include <string>

#include "player.hpp"
#include "user.hpp"
#include "admin.hpp"
#include "club.hpp"
#include "match.hpp"

const std::string ADMIN_USERNAME = "admin";
const std::string ADMIN_PASSWORD = "123456";
const std::string OWN_GOAL = "OWN_GOAL";

class Database {
private:
    std::vector<Player*> players;
    std::vector<User*> users;
    std::vector<Admin*> admins;
    std::vector<Club*> clubs;
    std::vector<Match*> matches;
    void read_league_csv();
    Club* create_club(std::string club_name);
    Player* create_player(std::string name, Club* club, PLAYER_POSITION position, int cost);
    Match* create_match(Club* home, Club* away, Result res, int week);
    void update_standings_stats(Club* home, Club* away, Result res);
    Admin* create_admin();
public:
    Database() {
        create_admin();
        read_league_csv();
    }
    std::vector<Player*> get_players();
    void read_week_stats(int week);
    std::vector<Club*> get_clubs_sorted();
    User* get_user(std::string username);
    Admin* get_admin(std::string username);
    User* create_user(std::string username, std::string pasword);
    std::vector<Player*> get_best_players(PLAYER_POSITION position, int count, int week);
    std::vector<Match*> get_matches(int week);
    Club* get_club(std::string club_name);
    Player* get_player(std::string player_name);
    std::vector<Player*> get_players_of_club(Club* club);
    void tell_player_the_week_passed();
    void reset_transfers_counts();
    void calculate_players_points();
    void calculate_teams_points();
    std::vector<User*> get_users();
};

#endif
