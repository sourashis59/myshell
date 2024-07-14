#pragma once
#include "Command.cpp"
#include "../util/SystemCallWrapper.cpp"


class PipeNode: public Command {
    Command *left_cmd;
    Command *right_cmd; 

public:
    PipeNode(Command *left_cmd, Command *right_cmd): left_cmd(left_cmd), right_cmd(right_cmd) {}

    virtual void run() {
        cout << "\n[DEBUG]: PipeNode.run()" << endl;

        pair<int, int> pipe_fds = SystemCallWrapper::pipe_wrapper();
        int pipe_read_fd = pipe_fds.first;
        int pipe_write_fd = pipe_fds.second;

        /*
         * Current process will fork, and the child process will execute the left command.
         * Then again current process will fork and the child process will exec the right command.
         * Then current process will wait for both children to exit. 
         */

        //* left command
        pid_t left_pid = SystemCallWrapper::fork_wrapper();
        if (left_pid == 0) {
            //* make std out point to pipe write end
            close(STDOUT_FILENO);
            dup(pipe_write_fd);

            //*TODO
            //* For left command, pipe read end is not needed, so better close it
            //* Also pipe's write end will not be used. The left process will write 
            //* to pipe's write end via STDOUT file descriptor [Because of duplicating this file usign dup()], 
            //* so we can close pipe_write_fd
            close(pipe_read_fd);
            close(pipe_write_fd);

            //* execute left command (may be composite)
            left_cmd->run();
        }

        //* right command
        pid_t right_pid = SystemCallWrapper::fork_wrapper();
        if (right_pid == 0) {
            //* make std in point to pip read end
            close(STDIN_FILENO);
            dup(pipe_read_fd);

            //*TODO
            //* Similar
            close(pipe_read_fd);
            close(pipe_write_fd);

            //* execute right command (may be composite)
            right_cmd->run();
        }

        //* parent process doe not need to access pipe
        //* so better close these fds to prevent access by mistake
        close(pipe_read_fd);
        close(pipe_write_fd);
        
        //* wait for both children to finish
        int left_status = SystemCallWrapper::wait_wrapper(left_pid);
        int right_status = SystemCallWrapper::wait_wrapper(right_pid);
        
        //*TODO:
        if (left_status == 0 && right_status == 0) { 
            exit(0);
        } else {
            cerr << "\n[ERROR]: pipe operator's operands did not exit properly. Left Status: " << left_status << ", Right Status: " << right_status << endl;
            exit(1);
        } 
    }
};