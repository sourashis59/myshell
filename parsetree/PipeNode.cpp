#pragma once
#include "Command.cpp"
#include "../util/SystemCallWrapper.cpp"
#include "../config.h"
#include "../util/Logger.h"


class PipeNode: public Command {
    Command *left_cmd;
    Command *right_cmd; 

public:
    PipeNode(Command *left_cmd, Command *right_cmd): left_cmd(left_cmd), right_cmd(right_cmd) {}

    virtual ~PipeNode() {
        if (Config::get_instance()->debug_mode == true) {
            Logger::get_instance()->log("Destructor called for PipeNode");
        }

        delete left_cmd;
        delete right_cmd;
    }

    /*
     * How pipe wokrs?:
     * ------------------------------------------------------
     * Example: command1 | command2
     *
     *                               shell Process
     *                                   |
     *                                   |
     *                           (Create a pipe)
     *                                   | 
     *                                   |   
     *                                 (fork)
     *                                  / \ 
     *                                 /   \  
     *    Child process  -------------/     \--------------  shell process
     *                 |                                    |
     *                 |                                    |
     *                 |                                  (fork)
     *                 |                                    |
     *   [Make STDOUT file descriptor                      / \
     *    point to the pipe write end                     /   \
     *    and close unnecessary FDs]           child  ---/     \------------- shell prcoess
     *                 |                     process |                        |
     *                 |                             |                        |
     *                 |                             |                        |
     *   [exec(command1) sys call]        [Make STDIN file descriptor         |
     *                 |                   point to pipe read end,            |
     *                 |                   and close unused FDs]              |
     *                 |                             |                (close unnecessary 
     *           command1 process                    |                 File Descriptors)
     *                 |                             |                        |
     *                 |                 [exec(command2) sys call]            |
     *                 |                             |                        |
     *         (exit when done)                      |                        |
     *                 |                       command2 process               |
     *                 |                             |                        |
     *                 |                             |                        |
     *                 |                     (exit when done)                 |
     *                 |                             |                        |
     *                  \                             \                       |
     *                   \                             \                      |
     *                    \                             \------------(wait for 2nd child)
     *                     \                                                  |
     *                      \                                                 |
     *                       \--------------------------------------(wait for 1st child)
     *                                                                        |
     *                                                                        |
     *                                                       (shell process continues or exits)
     *                                                                        |
     *                                                                       ...
     *                                                                       ...
     *     
     * 
     * -----------------------------------------------------------------------
     *  Important Note: Always close the unused pipe ends in each of the child 
     *                  processes and parent process
     * -----------------------------------------------------------------------
     * 
     * REFERENCE: https://www.ibm.com/docs/de/aix/7.2?topic=io-working-pipes
     * 
     * 
     * - In some process if you do `read(pipe read end file descriptor)`, then the 
     * the process will be blocked until some process does `write(pipe write end fd)`
     * or EOF (end of file) is obtained from read end. 
     * 
     * - Pipe read end returns EOF when some process writes EOF to the write end of the pipe 
     * or, all file descriptors pointing to the write end are closed.
     * So when the shell process creates pipe, it has at least these open file 
     * descriptors: STDIN, STDOUT, STDERR, pipe read end fd, pipe write end fd. 
     * When it forks to create children processes to execute 2 commands, both of these 
     * children processes inherit all these file descriptors from parent process. 
     * So all of them are open in the children processes.   
     * 
     * 
     * 
     * - Now consider this example: `echo "hello" | grep "hell"`
     * grep works like this:
     *       do {
     *            read(STDIN, ...)
     *            // process input
     *       } while (EOF is not read from STDIN)
     * 
     *      So grep will stop when EOF is read. And in this case, STDIN of the second 
     *      command of pipe operator, will point to the pipe read end. So grep will 
     *      stop when EOF is read from pipe read end.
     *      
     *      In this second process (grep here), if you do not close pipe write end fd,
     *      then disaster will happen. First command (echo) will exit at some point
     *      and before exiting, it will close all open file descriptors, so echo will 
     *      close it's pipe write end as well. But this pipe write end file is pointed to
     *      by one more fd in grep, which we didn't close. So kernel will pipe's write end
     *      is not closed yet and pipe's read end will not return EOF until all write end FDs
     *      are closed. So grep program (second command) will be stuck forever waiting for EOF.
     *      
     *      Similarly the shell process (or the parent) process must also close pipe's write 
     *      end, so the read(pipe read end FD) returns EOF.
     *      
     *      
     *   =============================================================================================
     *  | MANDATORY: SO PIPE'S WRITE END FD MUST BE CLOSED IN THE SECOND PROCESS AND PARENT PROCESS.  |
     *   =============================================================================================
     *      
     * 
     * - We should close other FDs too, so that in future by mistake they are not accessed, 
     * but they do not necessarily causes this indefinite waiting:
     *      - Left process should close the pipe write end fd as write end will not be used by it.
     *      - Left process should close the pipe read end fd also, after dupping (making STDOUT
     *      point to pipe read end). STDOUT file descriptor is pointing to pipe read end file and 
     *      this process will read inputs using STDOUT, not pipe read file descriptor. So this fd
     *      is waste of resource, so better close this pipe read end file descriptor, so that in 
     *      future that integer can be used if any file descriptor is needed.  
     *      - Right process should close the pipe read end fd similarly, as read end will not be 
     *      used by it.
     *      - Right process should similarly close pipe read end, to save resources, as STDIN is 
     *      anyway pointing to pipe read end.
     *      - [Right process must close pipe write end to prevent indefinite waiting!]
     *      - [Parent process must close pipe write end to prevent indefinite waiting!]
     *      - Parent process should close pipe's read end also, as it is not using pipe for any purpose.
     * 
     */
    virtual void run() {
        if (Config::get_instance()->debug_mode == true) {
            Logger::get_instance()->log("PipeNode.run()");
        }

        pair<int, int> pipe_fds = SystemCallWrapper::pipe_wrapper();
        int pipe_read_fd = pipe_fds.first;
        int pipe_write_fd = pipe_fds.second;

        //* run left command
        pid_t left_pid = SystemCallWrapper::fork_wrapper();
        if (left_pid == 0) {
            //* make STDOUT fd point to pipe write end
            // close(STDOUT_FILENO);
            // dup(pipe_write_fd); //* dup duplicates the file with the least available FD
            SystemCallWrapper::dup2_wrapper(pipe_write_fd, STDOUT_FILENO);

            //* For left command, pipe read end is not needed, so better close it.
            //* Also pipe's write end will not be used. The left process will write 
            //* to pipe's write end via STDOUT file descriptor [Because of duplicating this file usign dup()], 
            //* so we can close pipe_write_fd.
            close(pipe_read_fd);
            close(pipe_write_fd);

            //* execute left command (may be composite)
            left_cmd->run();
        }

        //* run right command
        pid_t right_pid = SystemCallWrapper::fork_wrapper();
        if (right_pid == 0) {
            //* make std in fd point to pipe read end
            // close(STDIN_FILENO);
            // dup(pipe_read_fd);
            SystemCallWrapper::dup2_wrapper(pipe_read_fd, STDIN_FILENO);

            //* For right process, pipe read end fd can be closed, because 
            //* this process will read from pipe read end via STDIN fd [because of dup]
            //* So we can close it [and should close it]
            close(pipe_read_fd);
            //* [WE MUST CLOSE THIS]: to prevent indefinite waiting (explained in earlier comments)
            close(pipe_write_fd);

            //* execute right command (may be composite)
            right_cmd->run();
        }

        //* parent process doe not need to access pipe
        //* so better close these fds to prevent access by mistake
        close(pipe_read_fd);
        //* [WE MUST CLOSE THIS]: to prevent indefinite waiting (explained in earlier comments)
        close(pipe_write_fd);
        
        //* wait for both children to finish
        int left_status = SystemCallWrapper::wait_wrapper(left_pid);
        int right_status = SystemCallWrapper::wait_wrapper(right_pid);
        
        //*TODO:
        if (left_status == 0 && right_status == 0) { 
            exit(0);
        } else {
            if (Config::get_instance()->debug_mode == true) {
                Logger::get_instance()->log("[ERROR]: pipe operator's operands did not exit properly. Left Status: " + to_string(left_status) + ", Right Status: " + to_string(right_status));
            }
            exit(1);
        } 
    }

    
    virtual void print() {
        cout << "(";
        left_cmd->print();
        cout << "|";
        right_cmd->print();
        cout << ")";
    }
};