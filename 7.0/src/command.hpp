#ifndef COMMAND_HEADERFILE
#define COMMAND_HEADERFILE

#include <string>
#include <functional>
#include <vector>

#include "database.hpp"
#include "session.hpp"

using executer = std::function<void(Database*, Session*, std::vector<std::string>)>;

const int PLAYERS_COUNT_IN_SQUAD[5] = {1, 2, 1, 1};
const int NUM_OF_PLAYERS_IN_SQUAD = 5;
const int MAX_BUYS = 1;
const int MAX_SELLS = 2;

typedef std::vector<std::string> Arguments;

enum HTTP_VERB {
    GET,
    POST,
    DELETE,
    PUT,
    NO_VERB
};

class Command {
private:
    std::string name;
    executer exec;
    HTTP_VERB verb;
public:
    Command(executer fnc, std::string name_, HTTP_VERB verb_) {
        exec = fnc;
        name = name_;
        verb = verb_;
    }
    void execute(Database*, Session*, Arguments);
    std::string get_name();
    HTTP_VERB get_verb();
};

void get_team_of_the_week(Database*, Session*, Arguments);
void get_players(Database*, Session*, Arguments);
void get_league_standings(Database*, Session*, Arguments);
void get_users_ranking(Database*, Session*, Arguments);
void get_matches_result(Database*, Session*, Arguments);
void signup(Database*, Session*, Arguments);
void login(Database*, Session*, Arguments);
void register_admin(Database*, Session*, Arguments);
void logout(Database*, Session*, Arguments);
void sell_player(Database*, Session*, Arguments);
void buy_player(Database*, Session*, Arguments);
void get_squad(Database*, Session*, Arguments);
void close_transfer_window(Database*, Session*, Arguments);
void open_transfer_window(Database*, Session*, Arguments);
void pass_week(Database*, Session*, Arguments);
void set_captain(Database*, Session*, Arguments);
void show_budget(Database*, Session*, Arguments);

#endif
