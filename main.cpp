#include "parsetree/ExecNode.cpp"

#include<string>
#include<sstream>
#include<vector>
#include<stdlib.h>
#include<iostream>
#include<unistd.h>
#include <sys/types.h>
#include<sys/wait.h>

using namespace std;



vector<string> parseTokens(const string &str) {
    stringstream ss(str);
    vector<string> res;
    string token;
    while (ss >> token) {
        res.push_back(token);
    }
    return res;
}

int main() {
    string input;
    int pid;
    while (true) {
        cout << "\n[basic-shell]$ ";
        cout.flush();
        getline(cin, input);
        if (input == "exit") 
            exit(0);
        
        pid = fork();
        if (pid == 0)
            ExecNode(parseTokens(input)).run();
        
        int status;
        waitpid(pid, &status, 0);
    }
}

