# myshell
- A POSIX shell implementation in C++
- Can parse and execute commands
- Supported shell operators: 
    - Pipe `|`
    - I/O redirection `<`, `>`, `>>`
    - Background `&`
    - Line separator `;`



## Tutorial:


## Flow:
- From the given input command, parse tree is built
- Then the parse tree is executed using system calls

    ### Example:
    - Command: `(cmd1 | cmd2 | cmd3) < myFile.txt | cmd4; cmd7 &; cmd5 | cmd6`
    - Parse tree: ` (((((cmd1 )|((cmd2 )|(cmd3 ))) < myFile.txt)|(cmd4 ));(((cmd7 )&);((cmd5 )|(cmd6 ))))`

        <img src="readme-files/parsetree2..png">

    - Parse tree will be traversed in preorder manner and each operator and exec nodes will be executed accordingly.
    - Flow:

        <img src="readme-files/shell_flow.png">
