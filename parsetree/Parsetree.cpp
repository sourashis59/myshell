#pragma once

#include "Command.cpp"
#include "../config.h"
#include "../util/Logger.h"


class ParseTree {
    Command *command;

public:
    ParseTree(Command *command): command(command) {}

    virtual ~ParseTree() {
        Logger::get_instance()->log("Destructor called for ParseTree");
        delete command;
    }

    virtual void run() {
        command->run();
    }

    virtual void print() {
        command->print();
    }
};
