#include "general.hpp"
#include "command.hpp"
#include "custom_exceptions/bad_request_exception.hpp"
#include <vector>

using namespace std;

HTTP_VERB string_to_verb(string str) {
    if (str == "GET") return HTTP_VERB::GET;
    if (str == "POST") return HTTP_VERB::POST;
    if (str == "PUT") return HTTP_VERB::PUT;
    if (str == "DELETE") return HTTP_VERB::DELETE;
    return HTTP_VERB::NO_VERB;
}

Arguments get_args(vector<string> words) {
    Arguments res;
    for (int i = 2; i < (int)words.size(); i++)
        res.push_back(words[i]);
    return res;
}

PLAYER_POSITION shortened_to_position(string pos) {
    if (pos == "fw")
        return FORWARD;
    if (pos == "md")
        return MIDFIELDER;
    if (pos == "df")
        return DEFENDER;
    if (pos == "gk")
        return GOALKEEPER;

    throw BadRequestException();
}

string replace_string(string str, char old_char, char new_char) {
    string res;
    for (char c : str)
        res += (c == old_char ? new_char : c);
    return res;
}

string get_player_name(Arguments args) {
    string res;
    for (int i = 2; i < (int)args.size(); i++)
        res = res + args[i] + " ";
    res.pop_back();
    return res;
}

vector<SQUAD_POSITIONS> get_opp_indexes(SQUAD_POSITIONS pos) {
    vector<SQUAD_POSITIONS> res;
    switch (pos) {
        case LB:
            res.push_back(RW);
            res.push_back(RB);
            break;
        case RB:
            res.push_back(LW);
            res.push_back(LB);
            break;
        case LCB:
            res.push_back(LCB);
            res.push_back(RCB);
            res.push_back(ST);
            break;
        case RCB:
            res.push_back(LCB);
            res.push_back(RCB);
            res.push_back(ST);
            break;
        case LM:
            res.push_back(LM);
            res.push_back(RM);
            res.push_back(CDM);
            break;
        case RM:
            res.push_back(LM);
            res.push_back(RM);
            res.push_back(CDM);
            break;
        case CDM:
            res.push_back(LM);
            res.push_back(RM);
            res.push_back(CDM);
            break;
        default:
            break;
    }
    return res;
}
