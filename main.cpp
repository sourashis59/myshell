#include "parsetree/ExecNode.cpp"
#include "parsetree/BackgroundNode.cpp"
#include "parsetree/SeparatorNode.cpp"


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


void testBackground(Command *backgroundCmd) {
    //* TEST background
    if (SystemCallWrapper::fork_wrapper() == 0) {
        // if (SystemCallWrapper::fork_wrapper() == 0) 
        backgroundCmd->run();

        // (new ExecNode(ls_program))->run();
    }
    this_thread::sleep_for(chrono::seconds(5));
    printf("\nParent Process: Thank you!\n");
}

void testSeparator(Command *cmd) {
    if (SystemCallWrapper::fork_wrapper() == 0)
        cmd->run();
    
    SystemCallWrapper::wait_wrapper();
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



    vector<string> ls_program = {"ls", "./parsetree", "-l"};
    vector<string> wait_and_print_program = {"test-programs/wait_and_print_hello.out"};
    vector<string> wait_and_print_program1 = {"test-programs/wait_and_print_hello1.out"};

    Command *backgroundCmd = new BackgroundNode(new ExecNode(wait_and_print_program));
    Command *backgroundCmd1 = new BackgroundNode(new ExecNode(wait_and_print_program1));
    Command *separatorNode = new SeparatorNode(backgroundCmd, new ExecNode(ls_program));
    Command *separatorNode1 = new SeparatorNode(backgroundCmd, backgroundCmd1);
    Command *separatorNode2 = new SeparatorNode(backgroundCmd, backgroundCmd);
    Command *separatorNode3 = new SeparatorNode(separatorNode2, new ExecNode(ls_program));
    // lol
    Command *separatorNode4 = new SeparatorNode(separatorNode2, 
        new SeparatorNode(new ExecNode(ls_program), new ExecNode({"ls"})));
    testSeparator(separatorNode4);


}

