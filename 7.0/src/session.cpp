#include "session.hpp"
#include "custom_exception.hpp"
#include "custom_exceptions/bad_request_exception.hpp"
#include "custom_exceptions/permission_denied_exception.hpp"

using namespace std;

void Session::ensure_user_logged_in() {
    if (!current_user) throw PermissionDeniedException();
}

void Session::ensure_admin_logged_in() {
    ensure_user_logged_in();
    if (!current_user->is_admin()) throw PermissionDeniedException();
}

void Session::ensure_user_not_logged_in() {
    if (current_user) throw BadRequestException();
}

int Session::get_current_week() {
    return current_week;
}

Account* Session::get_current_user() {
    return current_user;
}

bool Session::is_transfer_window_open() {
    return transfer_window;
}

void Session::pass_week() {
    current_week++;
}

void Session::set_transfer_window(bool status) {
    transfer_window = status;
}

void Session::set_current_user(Account* current_user_) {
    current_user = current_user_;
}
