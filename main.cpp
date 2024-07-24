#include "parsetree/ExecNode.cpp"
#include "parsetree/BackgroundNode.cpp"
#include "parsetree/SeparatorNode.cpp"
#include "parsetree/PipeNode.cpp"
#include "util/parser.cpp"
#include "config.cpp"

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


int main() {
    Config::get_instance().set_debug_mode(true);
    
    string input;
    int pid;
    while (true) {
        cout << "[myshell:" << SystemCallWrapper::getcwd_wrapper() << "]$ ";
        cout.flush();
        getline(cin, input);
        if (input == "exit") 
            exit(0);
        
        // parse command 
        Command *command = Parser::parse(input);

        if (Config::get_instance().get_debug_mode() == true) {
            cout << "parse tree: ";
            command->print();
            cout << endl;
        }

        pid = fork();
        if (pid == 0)
            command->run();
        
        int status;
        waitpid(pid, &status, 0);

        if (Config::get_instance().get_debug_mode() == true) {
            cout << "\nAll child processes finished.\n";
        }
    }
}

// ls -l|grep out >> test_output.txt