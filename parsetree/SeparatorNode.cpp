#pragma once
#include "Command.cpp"
#include "../util/SystemCallWrapper.cpp"
#include "../config.h"
#include "../util/Logger.h"


class SeparatorNode: public Command {
    Command *leftCmd;
    Command *rightCmd; 

public:
    SeparatorNode(Command *leftCmd, Command *rightCmd): leftCmd(leftCmd), rightCmd(rightCmd) {}

    virtual ~SeparatorNode() {
        if (Config::get_instance()->debug_mode == true) {
            Logger::get_instance()->log("Destructor called for SeparatorNode");
        }
        delete leftCmd;
        delete rightCmd;
    }

    virtual void run() {
        if (Config::get_instance()->debug_mode == true) {
            Logger::get_instance()->log("SeparatorNode.run()");
        }

        //* Create a child process and run the first command.
        //* Creating new process is required, otherwise files descriptors of 
        //* current processed may be altered.
        pid_t pid = SystemCallWrapper::fork_wrapper(); 
        if (pid == 0) 
            leftCmd->run();

        //* Separator (;) operator: parent process must wait 
        //* for child to finish. Then the child second process will be executed
        int status = SystemCallWrapper::wait_wrapper(pid);
        if (status != 0)  {
            cerr << Config::get_instance()->debug_color << "[Error]: Separator operator's left command returned status: " << status;
            cerr.flush();
        }
        
        //* execute second command
        //*TODO: to support 'command ;' [2nd command is empty]
        //*TODO: we need to check non-null
        if (rightCmd != nullptr)
            rightCmd->run();
            
        exit(0);
    }

    virtual void print() {
        cout << "(";
        leftCmd->print();
        cout << ";";
        rightCmd->print();
        cout << ")";
    }
};