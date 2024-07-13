#include<string>
#include<vector>
#include<stdlib.h>
#include<iostream>
#include<unistd.h>
// #include <sys/types.h>
// #include<sys/wait.h>

#include "Command.cpp"

using namespace std;

class ExecNode: public Command {
    vector<string> args; 


private: 
    vector<char *> getCharPointerVector(vector<string>& args) const {
        vector<char *> res(args.size());
        for (int i = 0; i < args.size(); ++i) {
            res[i] = &(args[i][0]); 
        }
        res.push_back(nullptr);
        return res;
    }

    void panik() {
        cerr << "\nError executing command: ";
        for (const string &str: args) {
            cerr << str << " ";
        }
        exit(1);
    }

public:
    ExecNode(const vector<string> &args): args(args) {}

    virtual void run() {
        vector<char *> char_pointer_args = getCharPointerVector(args); 
        execvp(char_pointer_args[0], char_pointer_args.data());
        
        // if exec is successfull, next lines will never be executed
        panik();
    }
};