#ifndef SESSION_HEADERFILE
#define SESSION_HEADERFILE

#include "account.hpp"
#include "custom_exceptions/bad_request_exception.hpp"
#include "custom_exceptions/permission_denied_exception.hpp"
#include "custom_exceptions/empty_exception.hpp"
#include "custom_exceptions/not_found_exception.hpp"


class Session {
private:
    Account* current_user;
    int current_week;
    bool transfer_window;
public:
    Session() {
        current_user = nullptr;
        transfer_window = true;
        current_week = 0;
    }
    void ensure_user_logged_in();
    void ensure_admin_logged_in();
    void ensure_user_not_logged_in();
    int get_current_week();
    Account* get_current_user();
    bool is_transfer_window_open();
    void pass_week();
    void set_transfer_window(bool status);
    void set_current_user(Account* current_user_);
};

#endif
