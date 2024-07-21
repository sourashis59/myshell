#pragma once
#include "Command.cpp"
#include "../util/SystemCallWrapper.cpp"



enum RedirectType {
    IN,
    OUT,
    OUT_APPEND
};

class RedirectNode: public Command {
    Command *cmd; 
    RedirectType redir_type;
    string file_path;

public:
    RedirectNode(RedirectType redir_type, const string &file_path, Command *cmd = nullptr): 
        redir_type(redir_type), file_path(file_path), cmd(cmd) {}

    void set_cmd(Command *cmd) {
        this->cmd = cmd;
    } 

    Command *get_cmd() {
        return this->cmd;
    }

    virtual void run() {
        //*TODO:
        // cout << "\n[DEBUG]: RedirectNode.run(" << args[0] << ")" << endl;
        // SystemCallWrapper::execvp_wrapper(args[0], args);
        // // exit(1);

        if (cmd == nullptr) throw runtime_error("Redirect node's command is null!");
    }


    virtual void print() {
        cout << "(";
        cmd->print();

        if (redir_type == RedirectType::IN) cout << " <";
        else if (redir_type == RedirectType::OUT) cout << " >";
        else /*if (redir_type == RedirectType::OUT_APPEND)*/ cout << " >>";
        
        cout << " " << file_path;

        cout << ")";
    }
    
};