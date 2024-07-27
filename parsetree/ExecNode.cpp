#pragma once
#include "Command.cpp"
#include "../util/SystemCallWrapper.cpp"
#include "../util/Logger.h"
#include "../config.h"


class ExecNode: public Command {
    vector<string> args; 

public:
    ExecNode(const vector<string> &args): args(args) {
        for (string &arg: this->args)
            strip_quotes(arg);
    }

    virtual ~ExecNode() {
        if (Config::get_instance()->debug_mode == true) {
            Logger::get_instance()->log("Destructor called for ExecNode");
        }

    }


    virtual void run() {
        if (Config::get_instance()->debug_mode == true) {
            string message = "ExecNode.run(";
            for (const string& arg: args)
                message += arg + " ";
            
            message += ")";
            Logger::get_instance()->log(message);
        }

        if (args.size() && args[0].size())
            SystemCallWrapper::execvp_wrapper(args[0], args);
        
        exit(0);
    }

    virtual void print() {
        cout << "(";
        for (const auto &str: args)
            cout << str << " ";
        cout << ")";
    }

    //* TODO
    void strip_quotes(string &str) {
        if (str.size() == 0 
            || (str.back() != '\'' && str.back() != '\"' && str.front() != '\'' || str.front() != '\"')) 
            return;

        if (str.back() == '\'' || str.back() == '\"') str.pop_back();
        if (str.front() == '\'' || str.front() == '\"') str = str.substr(1, str.size() - 1);
        strip_quotes(str);        
    }
};