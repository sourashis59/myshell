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
#include<fstream>

#include <thread>
#include <chrono>


using namespace std;


int main() {
    Config* config = Config::get_instance(); 
    Logger *logger = Logger::get_instance();

    config->debug_mode = true;
    config->debug_color_enabled = false;
    ofstream log_file_stream("log.txt");
    logger->set_output_stream(&log_file_stream);

    logger->log("Shell Process started!");
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

        logger->log("All child process finished");
    }
}

// ls -l|grep out >> test_output.txt