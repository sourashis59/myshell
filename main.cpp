#include "parsetree/ExecNode.cpp"
#include "parsetree/BackgroundNode.cpp"
#include "parsetree/SeparatorNode.cpp"
#include "parsetree/PipeNode.cpp"
#include "util/parser.cpp"
#include "config.h"

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
    Config::get_instance().debug_mode = false;
    
    string input;
    int pid;
    while (true) {
        //* print shell prompt
        cout << Config::get_instance().prompt_color_code
            << "[myshell:" 
            << Config::get_instance().prompt_cwd_color_code
            << SystemCallWrapper::getcwd_wrapper() 
            << Config::get_instance().prompt_color_code
            << "]$ "
            << Config::PROMPT_COLOR_CODE[Config::PROMPT_COLOR::DEFAULT];

        cout.flush();
        getline(cin, input);
        if (input == "exit") 
            exit(0);
        
        // parse command 
        Command *command = Parser::parse(input);

        if (Config::get_instance().debug_mode == true) {
            cout << "parse tree: ";
            command->print();
            cout << endl;
        }

        pid = fork();
        if (pid == 0)
            command->run();
        
        int status;
        waitpid(pid, &status, 0);

        if (Config::get_instance().debug_mode == true) {
            cout << "\nAll child processes finished.\n";
        }
    }
}

// ls -l|grep out >> test_output.txt