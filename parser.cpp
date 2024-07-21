
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
        //*----------- Operators -----------//


        //*---------- Non-operators----------//
        WORD,    
        UNKNOW 
        //*---------- Non-operators----------//
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
            if (curr == " ") {
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
        return operator_str_to_token_type.count(string(1, c)) == 0 && c != ' ';

    }


    //* updates the pos
    static string read_word(const string &input, int &pos) {
        int start = pos;
        if (input[pos] == '\"' || input[pos] == '\'') {
            //*TODO:



        } else {
            while (pos < input.size() && is_word_char(input[pos]))
                ++pos;
        }
        return input.substr(start, pos - start);
    }
};


int main() {
    Lexer lexer;

    string input = "echo Hello world!| grep     ()(Hell|||&&>>>";
    
    queue<Token> q = lexer.tokenize(input);
    while (q.size()) {
        Token token = q.front();
        q.pop();
        cout << "\"" << token.value << "\"" << endl;
    }
}