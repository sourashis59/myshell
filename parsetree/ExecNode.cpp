#pragma once
#include "Command.cpp"
#include "../util/SystemCallWrapper.cpp"


class ExecNode: public Command {
    vector<string> args; 

public:
    ExecNode(const vector<string> &args): args(args) {}

    virtual void run() {
        cout << "\n[DEBUG]: ExecNode.run(" << args[0] << ")" << endl;
        SystemCallWrapper::execvp_wrapper(args[0], args);
        // exit(1);
    }
};