#include "parsetree/ExecNode.cpp"
#include "parsetree/BackgroundNode.cpp"
#include "parsetree/SeparatorNode.cpp"
#include "parsetree/PipeNode.cpp"
#include "util/parser.cpp"
#include "config.h"
#include "util/Logger.h"

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
    Config* config = Config::get_instance(); 
    config->debug_mode = true;
    
    if (config->debug_mode) {
        Logger::get_instance()->log("Shell Process started!");
    }

    string input;
    int pid;
    while (true) {
        //* print shell prompt
        cout << config->prompt_color_code
            << "[myshell:" 
            << config->prompt_cwd_color_code
            << SystemCallWrapper::getcwd_wrapper() 
            << config->prompt_color_code
            << "]$ "
            << Config::PROMPT_COLOR_CODE[Config::PROMPT_COLOR::DEFAULT];

        cout.flush();
        getline(cin, input);
        if (input == "exit") 
            exit(0);
        
        //* parse command 
        ParseTree parse_tree = Parser::parse(input);

        if (config->debug_mode == true) {
            cout << config->debug_color << "[DEBUG]: parse tree: ";
            parse_tree.print();
            cout << Config::PROMPT_COLOR_CODE[Config::PROMPT_COLOR::DEFAULT] << endl;
        }

        pid = fork();
        if (pid == 0) {
            parse_tree.run();
        }

        // //* prevent memory leak
        // delete command;

        int status;
        waitpid(pid, &status, 0);

        if (config->debug_mode == true) {
            Logger::get_instance()->log("All child process finished");
        }
    }
}

// ls -l|grep out >> test_output.txt