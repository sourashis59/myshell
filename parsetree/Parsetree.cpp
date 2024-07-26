#pragma once

#include "Command.cpp"
#include "../config.h"

class ParseTree {
    Command *command;

public:
    ParseTree(Command *command): command(command) {}

    virtual ~ParseTree() {
        if (Config::get_instance().debug_mode == true) {
            cout << "[DEBUG]: processId: " << SystemCallWrapper::getpid_wrapper() << ", Destructor called for " << "ParseTree" << endl;
        }
        delete command;
    }

    virtual void run() {
        command->run();
    }

    virtual void print() {
        command->print();
    }
};
