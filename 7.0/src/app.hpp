#ifndef APP_HEADERFILE
#define APP_HEADERFILE

#include <iostream>
#include <vector>

#include "database.hpp"
#include "session.hpp"
#include "command.hpp"

class App {
private:
    Database* db;
    Session* cookie;
    std::vector<Command*> commands;
    void initialize_commands();
    void execute_command(HTTP_VERB verb, std::string command_name, Arguments args);
public:
    App() {
        db = new Database();
        cookie = new Session();
        initialize_commands();
    }
    void start();
};

#endif
