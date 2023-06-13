#ifndef GENERAL_HEADERFILE
#define GENERAL_HEADERFILE

#include <string>

#include "command.hpp"
#include "player.hpp"

HTTP_VERB string_to_verb(std::string str);
Arguments get_args(std::vector<std::string> words);
PLAYER_POSITION shortened_to_position(std::string pos);
std::string replace_string(std::string str, char old_char, char new_char);
std::string get_player_name(Arguments);
std::vector<SQUAD_POSITIONS> get_opp_indexes(SQUAD_POSITIONS);

#endif
