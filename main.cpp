#include "parsetree/ExecNode.cpp"
#include "parsetree/BackgroundNode.cpp"
#include "parsetree/SeparatorNode.cpp"
#include "parsetree/PipeNode.cpp"
#include "parser.cpp"

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

void testCmd(Command *cmd) {
    pid_t pid = SystemCallWrapper::fork_wrapper(); 
    if (pid == 0)
        cmd->run();
    
    SystemCallWrapper::wait_wrapper(pid);
}

int main() {
    string input;
    int pid;
    while (true) {
        cout << "[myshell]$ ";
        cout.flush();
        getline(cin, input);
        if (input == "exit") 
            exit(0);
        
        // parse command 
        Command *command = Parser(input).parse();

        cout << "\nparse tree: ";
        command->print();
        cout << endl;

        pid = fork();
        if (pid == 0)
            command->run();
        
        int status;
        waitpid(pid, &status, 0);
    }



    // Command* cmd = new PipeNode(
    //     new RedirectNode(RedirectType::OUT_APPEND, "test.txt", 
    //         new ExecNode({"echo", "hello world"})
    //     ),
    //     new ExecNode({"grep", ""})
    // );

    // cmd->run();
    // vector<string> ls_program = {"ls", "./parsetree", "-l"};
    // vector<string> wait_and_print_program = {"test-programs/wait_and_print_hello.out"};
    // vector<string> wait_and_print_program1 = {"test-programs/wait_and_print_hello1.out"};

    // Command *backgroundCmd = new BackgroundNode(new ExecNode(wait_and_print_program));
    // Command *backgroundCmd1 = new BackgroundNode(new ExecNode(wait_and_print_program1));
    // Command *separatorNode = new SeparatorNode(backgroundCmd, new ExecNode(ls_program));
    // Command *separatorNode1 = new SeparatorNode(backgroundCmd, backgroundCmd1);
    // Command *separatorNode2 = new SeparatorNode(backgroundCmd, backgroundCmd);
    // Command *separatorNode3 = new SeparatorNode(separatorNode2, new ExecNode(ls_program));
    // // lol
    // Command *separatorNode4 = new SeparatorNode(separatorNode2, 
    //     new SeparatorNode(new ExecNode(ls_program), new SeparatorNode(new ExecNode({"ls"}), new ExecNode({"pwd"}))));
    // Command *separatorNode5 = new SeparatorNode(new ExecNode({"./test-programs/program_that_exits_with_error_status.out"}), new ExecNode(ls_program));
    // Command *separatorNode6 = new SeparatorNode(new BackgroundNode(new ExecNode({"test-programs/wait_and_print_hello.out"})), new ExecNode(ls_program));
    

    // Command *pipeCmd = new PipeNode(new ExecNode({"echo", "hello\nlol haha\nhelloWorld\nhelloLol\nlamao\nlol\nxd\nhahaha\nhelloEnd\n"}), new ExecNode({"grep", "hello"}));
    // Command *pipeCmd0 = new PipeNode(pipeCmd, new ExecNode({"grep", "helloW"}));

    // // Command *pipeCmd = new PipeNode(new ExecNode({"echo", "helloWorld\n"}), new ExecNode({"grep", "hello\n"}));
    // Command *pipeCmd1 = new PipeNode(new ExecNode({"./test-programs/test_echoer.out", "A"}), new ExecNode({"./test-programs/test_echoer.out", "B"}));
    // Command *pipeCmd2 = new PipeNode(pipeCmd1, new ExecNode({"./test-programs/test_echoer.out", "C"}));
    


    // // testCmd(new ExecNode({"echo", "hello"}));
    // // testCmd(new ExecNode({"grep", "hello"}));
    // testCmd(pipeCmd2);


}

// ls -l|grep out >> test_output.txt