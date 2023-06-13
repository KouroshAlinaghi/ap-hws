#ifndef ACCOUNT_HEADERFILE
#define ACCOUNT_HEADERFILE

#include "string"
#include "vector"

#include "player.hpp"

class Account {
protected:
    std::string username;
    std::string password;
public:
    Account(std::string username_, std::string password_) {
        username = username_;
        password = password_;
    }
    virtual bool is_admin() = 0;
    std::string get_username();
    bool check_password(std::string password_);
};

#endif
