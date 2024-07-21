
//*TODO: uncomment after removing main() [after testing]
// #pragma once

#include "parsetree/ExecNode.cpp"
#include "parsetree/BackgroundNode.cpp"
#include "parsetree/SeparatorNode.cpp"
#include "parsetree/PipeNode.cpp"
#include<unordered_map>
#include<queue>

namespace {
    enum TokenType {
        //*----------- Operators -----------//
        BACKGROUND,
        SEPARATOR,
        PIPE,
        REDIR_OUT,
        REDIR_OUT_APPEND,
        REDIR_IN,
        OPENING_PARENTHESIS,
        CLOSING_PARENTHESIS,
        //*---------------------------------------------//


        //*---------- Non-operators----------//
        WORD,    
        UNKNOW 
        //*--------------------------------------------//
    };

    unordered_map<string, TokenType> operator_str_to_token_type = {
        {"&", TokenType::BACKGROUND},
        {";", TokenType::SEPARATOR},
        {"|", TokenType::PIPE},
        {">", TokenType::REDIR_OUT},
        {">>", TokenType::REDIR_OUT_APPEND},
        {"<", TokenType::REDIR_IN},
        {"(", TokenType::OPENING_PARENTHESIS},
        {")", TokenType::CLOSING_PARENTHESIS},
    };
} 


class Token {
public:
    TokenType type;
    string value; 

    Token(TokenType type = TokenType::UNKNOW, string value = "unknown"): type(type), value(value) {}
};



class Lexer {
public:
    static queue<Token> tokenize(const string &input) {
        int pos = 0;
        queue<Token> q;

        while (pos < input.size()) {
            string curr = string(1, input[pos]);
            //* space
            if (curr == " " || curr == "\n") {
                ++pos;
            } else if (operator_str_to_token_type.count(curr)) {
                ++pos;
                //* if curr is ">", check if next char is also ">"
                //* if yes, then operator is >> otherwise operator is >
                if (curr != ">") { 
                    q.push(Token(operator_str_to_token_type[curr], curr));
                } else {
                    if (pos < input.size() && input[pos] == '>') { 
                        q.push(Token(TokenType::REDIR_OUT_APPEND, ">>"));
                        ++pos;
                    } else 
                        q.push(Token(TokenType::REDIR_OUT, ">"));
                }
            } else if (is_word_char(curr[0])) {
                q.push(Token(TokenType::WORD, read_word(input, pos)));
            }  else {
                //* throw error
                q.push(Token(TokenType::UNKNOW, curr));
                ++pos;
            }
        } 
        return q;
    }


private:
    static bool is_word_char(char c) {
        // return (isdigit(c) || isalpha(c) || c == '-' || c == '_' || c == '/' || c == '\\' || c == '\"' || c == '\'' || c == '.' || c == '!');
        //*TODO:
        return operator_str_to_token_type.count(string(1, c)) == 0 && c != ' ';

    }


    //*TODO: refactor
    //* updates the pos
    static string read_word(const string &input, int &pos) {
        int start = pos;
        if (input[pos] == '\"' || input[pos] == '\'') {
            char quote = input[pos]; //* can be single or double quote
            ++pos;
            //* loop until we hit same quote such that prev character is not '\' 
            while (pos < input.size()) {
                if (input[pos] == quote && input[pos - 1] != '\\') {
                    ++pos;
                    break;
                }
                ++pos;
            }
        } else {
            while (pos < input.size() && is_word_char(input[pos]) && input[pos] != '\"' && input[pos] != '\'')
                ++pos;
        }
        return input.substr(start, pos - start);
    }
};


int main() {
    Lexer lexer;

    // string input = "echo \"Hello world!!!!1242797!@#\\\"wad\\\"$6189247273|>><<??\"12_abcd-123_lol___3|<&&&-awd";
    string input = "1aw-+-2=--=-13|12\"1231231241243kl;ksdlf-=\"&&  \n;;     & 123";
    
    queue<Token> q = lexer.tokenize(input);
    cout << "\ninput: " << input << endl << endl;
    while (q.size()) {
        Token token = q.front();
        q.pop();
        cout << "\"" << token.value << "\"" << endl;
    }
}



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