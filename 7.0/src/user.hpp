#ifndef USER_HEADERFILE 
#define USER_HEADERFILE 

#include "account.hpp"

const int INITIAL_BUDGET = 2500;
const std::string POSITION_NAMES[10] = { "Goalkeeper", "Defender1", "Defender2", "Midfielder", "Striker" };

class User : public Account {
private:
    double total_points;
    int players_sold_this_week;
    int players_bought_this_week;
    std::vector<Player*> squad;
    bool ever_had_completed_squad;
    Player* captain;
    int budget;
public:
    User(std::string username_, std::string password_) : Account(username_, password_) {
        total_points = 0;
        players_sold_this_week = 0;
        players_bought_this_week = 0;
        ever_had_completed_squad = false;
        captain = nullptr;
        budget = INITIAL_BUDGET;
    }
    bool is_admin();
    int get_players_sold_this_week();
    int get_players_bought_this_week();
    void reset_transfers_counts();
    bool has_player(Player* player_);
    void sell_player(Player* player_);
    void inc_solds_count();
    int get_number_of_players(PLAYER_POSITION);
    void buy_player(Player* player_);
    void inc_boughts_count();
    bool is_complted();
    std::vector<Player*> get_squad();
    double get_total_points();
    void update_total_points();
    bool had_completed_squad();
    void print_row(int index);
    void set_captain(Player* player);
    int get_budget();
    int get_team_cost();
    void print_squad();
};

#endif
