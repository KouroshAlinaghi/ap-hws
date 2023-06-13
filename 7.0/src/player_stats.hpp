#ifndef PLAYER_STATS_HEADERFILE
#define PLAYER_STATS_HEADERFILE

class PlayerStats {
private:
    bool played;
    bool injured;
    bool yellow_card;
    bool red_card;
    double score;
    double raw_score;
    int goals;
    int assists;
    int own_goals;
    bool cleansheet;
public:
    PlayerStats() {
        played = false;
        injured = false;
        yellow_card = false;
        red_card = false;
        score = 0;
        goals = 0;
        assists = 0;
        cleansheet = true;
        own_goals = 0;
    }
    void set_injury(bool is_injured);
    void set_yellow_card(bool y_c);
    void set_red_card(bool r_c);
    void set_score(double score_);
    void set_played(bool played_);
    void score_goal();
    void score_assist();
    void score_own_goal();
    void set_no_cleansheet();
    int get_goals();
    int get_assists();
    int get_ogs();
    bool had_yellow_card();
    bool had_red_card();
    bool did_play();
    bool did_cleansheet();
    double get_score();
};

#endif
