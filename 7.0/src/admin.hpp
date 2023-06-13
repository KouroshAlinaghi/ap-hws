#ifndef ADMIN_HEADERFILE
#define ADMIN_HEADERFILE

#include "account.hpp"

class Admin : public Account {
public:
    Admin(std::string username_, std::string password_) : Account(username_, password_) {}
    bool is_admin();
};

#endif
