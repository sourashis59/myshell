#pragma once
#include "Command.cpp"
#include "../util/SystemCallWrapper.cpp"


class SeparatorNode: public Command {
    Command *leftCmd;
    Command *rightCmd; 

public:
    SeparatorNode(Command *leftCmd, Command *rightCmd): leftCmd(leftCmd), rightCmd(rightCmd) {}

    virtual void run() {
        cout << "\n[DEBUG]: SeparatorNode.run()" << endl;

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
            cerr << "\n[Error]: Separator operator's left command returned status: " << status;
            cerr.flush();
        }
        
        //* execute second command
        rightCmd->run();
        exit(0);
    }
};