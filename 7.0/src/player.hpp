#ifndef PLAYER_HEADERFILE
#define PLAYER_HEADERFILE

#include <string>
#include <vector>

#include "club.hpp"
#include "player_stats.hpp"

const int WEEKS_NEEDED_TO_RECOVER_INJURY = 4;
const int WIN_PTS = 5;
const int LOSE_PTS = -1;
const int DRAW_PTS = 1;
const int OWN_GOAL_PTS = -3;
const int OPPOSITE_SCORE_PTS = -1;

enum PLAYER_POSITION {
    GOALKEEPER,
    DEFENDER,
    MIDFIELDER,
    FORWARD
};

const std::vector<PLAYER_POSITION> POSITIONS_IN_ORDER { GOALKEEPER, DEFENDER, MIDFIELDER, FORWARD };

class Match;

class Player {
protected:
    std::string name;
    Club* team;
    PLAYER_POSITION position;
    int weeks_left_until_injury_recovery;
    int yellow_cards_count;
    bool availablity;
    int cost;
    std::vector<PlayerStats*> stats;
    double pts_fnc_calc(int raw_points);
public:
    Player(Club* team_, std::string name_, PLAYER_POSITION position_, int cost_) {
        team = team_;
        name = name_;
        position = position_;
        weeks_left_until_injury_recovery = 0;
        availablity = true;
        cost = cost_;
    }
    std::string get_name();
    Club* get_team();
    PLAYER_POSITION get_position();
    void get_injured();
    void recieve_yellow_card();
    void recieve_red_card();
    void initialize_new_week_stats();
    double get_current_week_score();
    double get_week_score(int week);
    double get_avg_score();
    std::string get_position_shortened();
    bool is_available();
    void recover_one_week();
    void print_week_details(int week);
    virtual void print_with_details(int index) = 0;
    int get_cost();
    void score_goal();
    void score_assist();
    void score_own_goal();
    void set_no_cleansheet();
    int get_cleansheets();
    int get_goals();
    int get_assists();
    void play();
    int get_weeks_played();
    virtual void calculate_score(Match* match) = 0;
    int get_last_week_goals();
    double get_total_points();
};

#endif
