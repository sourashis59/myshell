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
        cout << "\n[DEBUG]: RedirectNode.run(file_path= " << file_path << ", redir_type: " << redir_type << ")" << endl; 

        int target_fd = -1;
        int source_fd = -1;
        //* TODO: 0644 mode may not be required. Recheck!
        if (redir_type == RedirectType::IN) {
            target_fd = SystemCallWrapper::open_wrapper(file_path, O_RDONLY);
            source_fd = STDIN_FILENO;
        } else if (redir_type == RedirectType::OUT) { 
            target_fd = SystemCallWrapper::open_wrapper(file_path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            source_fd = STDOUT_FILENO;
        } else /*if (redir_type == RedirectType::OUT_APPEND)*/ {
            target_fd = SystemCallWrapper::open_wrapper(file_path, O_WRONLY | O_CREAT | O_APPEND, 0644);
            source_fd = STDOUT_FILENO;
        } 

        SystemCallWrapper::dup2_wrapper(target_fd, source_fd);
        SystemCallWrapper::close_wrapper(target_fd);

        if (cmd == nullptr) throw runtime_error("Redirect node's command is null!");
        cmd->run();
        exit(1);
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