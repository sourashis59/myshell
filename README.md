# myshell
- A POSIX shell implementation in C++
- Can parse and execute commands
- Supported shell operators: 
    - Pipe `|`
    - I/O redirection `<`, `>`, `>>`
    - Background `&`
    - Line separator `;`



## Tutorial:
- You can download the `myshell.out` binary and run the shell using `./myshell.out` command.
- Then any command and shell operator can be run using this shell.
- Example screenshot: [click on the image to zoom in]
    <img src='readme-files/shell_screenshot.png'>



## Flow:
- From the given input command, a parse tree is built.
- Then the parse tree is executed using system calls.

    ### Example:
    - Command: `(cmd1 | cmd2 | cmd3) < myFile.txt | cmd4; cmd7 &; cmd5 | cmd6`
    - Parse tree: ` (((((cmd1 )|((cmd2 )|(cmd3 ))) < myFile.txt)|(cmd4 ));(((cmd7 )&);((cmd5 )|(cmd6 ))))`
    
        <img src="readme-files/parsetree2..png">

    - Parse tree will be traversed in preorder manner and each operator and exec nodes will be executed accordingly.
    - Flow: [click on the image to zoom in]
        - `pread`: pipe read end
        - `pwrite`: pipe write end
          
        <img src="readme-files/shell_flow.png">

## Files:
| File | Description |
|----------|----------|
| `parsetree/Command.cpp` | Interface for parse tree nodes |
| `parsetree/...rest...` | Implementation of parse tree nodes |
| `test-programs/` | Programs to test the shell operators |
| `util/Logger.h` | For logging debug messages |
| `util/lexer.cpp` | For getting tokens from input string |
| `util/parser.cpp` | For building parse tree from tokens |
| `util/SystemCallWrapper.cpp` | Adapter class containing wrappers for C sys calls |
| `config.cpp` | For storing the config |
| `main.cpp` | Contains main function |
