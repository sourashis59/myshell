#pragma once

#include<string>
#include<sstream>
#include<vector>
#include<stdlib.h>
#include<iostream>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<fcntl.h>

#define MAX_BUFF_SIZE_FOR_CWD (1024)

using namespace std;

//* Contains wrapper functions wrapping the glibc sys call wrapper functions
class SystemCallWrapper {

    /*
     * Given vector of string objects, converts the string objects
     * to char* and returns new vector<char *> 
     * [char* is need instead of string object for sys calls]
     */
    static vector<char *> getCharPointerVector(vector<string>& args) {
        vector<char *> res(args.size());
        for (int i = 0; i < args.size(); ++i) {
            res[i] = &(args[i][0]); 
        }
        res.push_back(nullptr);
        return res;
    }

public:

    /*
     * Returns process id  
     */
    static pid_t getpid_wrapper() {
        return getpid();
    }
    
    /*
    *- Print error message to standard error, and exits with status code 1
    */
    static void panic(const string &message) {
        cerr << "\nError happened: " << message << endl;
        exit(1);
    }

    /*
    * - Executes the given `file` using execvp() sys call [with args]
    * - Panics if execvp() sys call fails 
    */
    static void execvp_wrapper(string &file, vector<string> &args) {
        vector<char *> char_pointer_args = getCharPointerVector(args); 
        execvp(char_pointer_args[0], char_pointer_args.data());
        
        //* if exec is successful, current process's address space will be 
        //* replaced by the new process and next lines will never be called
        panic("execvp(\"" + file + "\") failed!");
    }

    /*
    *- Returns pid
    *- If fork() is not successful, panics
    */
    static int fork_wrapper() {
        int pid = fork();
        if (pid == -1) panic("fork");
        return pid;
    }
    
    /*
     * Returns exit status code of process with pid 
     */
    static int wait_wrapper(pid_t pid) {
        int status;
        waitpid(pid, &status, 0);
        // wait(0);
        return status;
    }

    /*
     * Creates a pipe and returns the file descriptors {read end fd, write end fd}.
     * Panics if any thing goes wrong.
     */
    static pair<int, int> pipe_wrapper() {
        int pipeFds[2];
        if (pipe(pipeFds) < 0) panic("pipe() sys call failed");
        return pair<int,int>(pipeFds[0], pipeFds[1]);
    }

    /*
     * Duplicates the soource_fd to target_fd 
     * [target_fd will point to same file pointed to by source_fd] 
     * Example: to make STDIN_FILENO fd point to your custom FD,
     *          use: dup2_wrapper(custom_fd, STDIN_FILENO)
     * If dup2() sys call fails, panics!
     */
    static void dup2_wrapper(int source_fd, int target_fd) {
        if (dup2(source_fd, target_fd) < 0) panic("dup2() sys call failed");
    }

    /*
     * Opens file and returns the file descriptor.
     * If open() sys call fails, panics!
     * 
     * - file_path: path of the file to be opened
     * - flag: O_WRONLY, O_CREAT, O_TRUNC
     * - mode: permission mode
     */
    static int open_wrapper(const string &file_path, int flag, int mode = 0644) {
        int new_fd = open(file_path.c_str(), flag, mode); 
        if (new_fd < 0) panic("open() sys call failed for file: " + file_path);
        return new_fd;
    }
    
    /*
     * Closes the given file descriptor.
     * If close() sys call fails, panics! 
     */
    static void close_wrapper(int fd) {
        if (close(fd) < 0) panic("close() sys call faield!");
    }


    /*
     * - Returns the current working directory.
     * - If `getcwd()` sys call fails, returns  
     *  "getcwd() sys call not successful!"
     */
    static string getcwd_wrapper() {
        //* buffer to store current working directory of the process
        char cwd_buffer[MAX_BUFF_SIZE_FOR_CWD];
        string res;
        if (getcwd(cwd_buffer, sizeof(cwd_buffer)) == nullptr){
            cerr << "\ngetcwd() sys call not successful!" << endl;
            res = "getcwd() sys call not successful!";
        } else {
            res = string(cwd_buffer);
        }
        return res;
    }
};

