#include "parsetree/ExecNode.cpp"
#include "parsetree/BackgroundNode.cpp"

#include<string>
#include<sstream>
#include<vector>
#include<stdlib.h>
#include<iostream>
#include<unistd.h>
#include <sys/types.h>
#include<sys/wait.h>

#include <thread>
#include <chrono>


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
    // string input;
    // int pid;
    // while (true) {
    //     cout << "[basic-shell]$ ";
    //     cout.flush();
    //     getline(cin, input);
    //     if (input == "exit") 
    //         exit(0);
        
    //     // parse command 
    //     // Command *command = parse(input);

    //     pid = fork();
    //     if (pid == 0)
    //         ExecNode(parseTokens(input)).run();
        
    //     int status;
    //     waitpid(pid, &status, 0);
    // }

    // vector<string> args = {"ls", "./parsetree", "-l"};
    vector<string> args = {"test-programs/wait_and_print_hello.out"};
    Command *cmd = new BackgroundNode(new ExecNode(args));
    cmd->run();

    this_thread::sleep_for(chrono::seconds(5));
    printf("\nParent Process: Thank you!\n");
}

