#pragma once
#include "lexer.cpp"

class Parser {
    class ParserUtil {
        queue<Token> q;

    public:
        ParserUtil(const string &input) {
            q = Lexer::tokenize(input);
        }

        Command* parse() {
            Command *parse_tree_root = parse_line();
            if (!q.empty())
                throw runtime_error("Trailing chracters!");

            return parse_tree_root;
        }

    private:

        bool match(TokenType type) {
            return q.size() && q.front().type == type;
        }

        Command* parse_line() {
            //*TODO:
            // if (q.empty()) return nullptr;
            //* no command 
            if (q.empty()) return new ExecNode({""});

            Command *x = parse_background();
            if (match(TokenType::SEPARATOR)) {
                q.pop();

                //*TODO: remove this if condition [Check again: can be reason of bugs]
                // if (!q.empty())
                x = new SeparatorNode(x, parse_line());
            }
            return x;
        }

        Command* parse_background() {
            Command *x = parse_pipe();
            if (match(TokenType::BACKGROUND)) {
                q.pop();
                x = new BackgroundNode(x);
            }
            return x;
        }

        Command* parse_pipe() {
            Command *x = parse_exec();
            if (match(TokenType::PIPE)) {
                q.pop();
                x = new PipeNode(x, parse_pipe());
            }
            return x;
        }

        Command* parse_exec() {
            Command *x = nullptr;
            if (match(TokenType::OPENING_PARENTHESIS)) {
                q.pop();
                x = parse_line();
                if (!match(TokenType::CLOSING_PARENTHESIS))
                    throw runtime_error("Missing ')' !");
                
                //* pop closing parenthesis
                q.pop();
            } else {
                x = parse_commandargs();
            }

            //*TODO: redirects [refactor]
            if (match(TokenType::REDIR_IN) || match(TokenType::REDIR_OUT) || match(TokenType::REDIR_OUT_APPEND)) {
                pair<RedirectNode*, RedirectNode*> redirect_nodes = parse_redirects();
                redirect_nodes.second->set_cmd(x);
                x = redirect_nodes.first;
            }

            return x;
        }

        Command *parse_commandargs() {
            vector<string> args;
            while (match(TokenType::WORD)) {
                args.push_back(q.front().value);
                q.pop();
            }
            return new ExecNode(args);
        }

        pair<RedirectNode*, RedirectNode*> parse_redirects() {
            RedirectNode* start_node = nullptr;
            RedirectNode* end_node = nullptr;
            while (match(TokenType::REDIR_IN) || match(TokenType::REDIR_OUT) || match(TokenType::REDIR_OUT_APPEND)) {
                //* figure out redirect type
                RedirectType redir_type;
                {
                    TokenType redir_token_type = q.front().type;
                    q.pop();

                    if (redir_token_type == TokenType::REDIR_IN) redir_type = RedirectType::IN;
                    else if (redir_token_type == TokenType::REDIR_OUT) redir_type = RedirectType::OUT;
                    else /* (redir_token_type == TokenType::REDIR_OUT_APPEND) */ redir_type = RedirectType::OUT_APPEND;
                }
                
                //* file path should be provided with redirect operator
                if (!match(TokenType::WORD))
                    throw runtime_error("No file path provided for redirect operator!");

                string file_path = q.front().value;
                q.pop();


                RedirectNode *new_node = new RedirectNode(redir_type, file_path, nullptr);
                if (start_node == nullptr) {
                    start_node = new_node;
                    end_node = start_node;
                } else {
                    end_node->set_cmd(new_node);
                    end_node = (RedirectNode*)end_node->get_cmd();
                }
            }
            return {start_node, end_node};
        }
    };

public:
    /*
     * Parses the input command and returns the parse tree. 
     * Throws exception in case of syntax error.
     */
    static Command* parse(const string &input) {
        return ParserUtil(input).parse();
    }
};


// int main() {
//     string input = "echo Hello   |   grep test";
    
//     Command* root = Parser(input).parse();
//     root->print();
//     root->run();

//     cout << endl;
//     return 0;
// }

// int main() {
//     Lexer lexer;

//     // string input = "echo \"Hello world!!!!1242797!@#\\\"wad\\\"$6189247273|>><<??\"12_abcd-123_lol___3|<&&&-awd";
//     string input = "1aw-+-2=--=-13|12\"1231231241243kl;ksdlf-=\"&&  \n;;     & 123";
    
//     queue<Token> q = lexer.tokenize(input);
//     cout << "\ninput: " << input << endl << endl;
//     while (q.size()) {
//         Token token = q.front();
//         q.pop();
//         cout << "\"" << token.value << "\"" << endl;
//     }
// }



// vector<pair<string, vector<string>>> test_cases = {
//     {"echo hello world", {"echo", "hello", "world"}},
//     {"echo hello world goodbye world             | grep    hell    ;;;ls -l haha02", {"echo", "hello", "world", "goodbye", "world", "|",
//     "grep", "hell", ";", ";", ";", "ls", "-l", "haha02"}},
//     {"                                                   ", {}},
//     {"12312123123-+-2=--=-123123|123124\"1231231241243kl;ksdlf-=\"&&       & 123"}
// };

// int main() {
//     int success = 0;
//     for (const auto& tc: test_cases) {
//         string input = tc.first;
//         vector<string> expected_output = tc.second;

//         bool failed = false;
//         queue<Token> q = Lexer::tokenize(input);
//         if (q.size() != expected_output.size()) {
//             cerr << "\n\n[Test Case Failed]: input " << input << endl;
//             continue;
//         } 
//         for (const string &expected_token: expected_output) {
//             Token actual_token = q.front();
//             q.pop();
//             if (actual_token.value != expected_token) {
//                 cerr << "\n\n[Test Case Failed]: input " << input << endl;
//                 failed = true;
//                 break;
//             }
//         }
//         if (!failed) ++success;
//     }

//     printf("\n\n\nTotal tc: %d, successful: %d, failed: %d\n\n", (int)test_cases.size(), success, (int)(test_cases.size() - success));
// }