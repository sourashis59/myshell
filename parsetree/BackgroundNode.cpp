#pragma once
#include "Command.cpp"
#include "../util/SystemCallWrapper.cpp"


class BackgroundNode: public Command {
    Command *cmd; 

public:
    BackgroundNode(Command *cmd): cmd(cmd) {}

    virtual void run() {
        cout << "\n[DEBUG]: BackgroundNode.run()" << endl;

        // child process will execute the command (may be composite command)
        if (SystemCallWrapper::fork_wrapper() == 0)
            cmd->run();

        // Background operator: parent process will not wait 
        // for child to finish
    }
};