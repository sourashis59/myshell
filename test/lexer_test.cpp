#include "../parser.cpp"



vector<pair<string, vector<string>>> test_cases = {
    {"echo hello world", {"echo", "hello", "world"}},
    {"echo hello world goodbye world             | grep    hell    ;;;ls -l haha02", {"echo", "hello", "world", "goodbye", "world", "|",
    "grep", "hell", ";", ";", ";", "ls", "-l", "haha02"}}
};

int main() {
    int success = 0;
    for (const auto& tc: test_cases) {
        string input = tc.first;
        vector<string> expected_output = tc.second;

        bool failed = false;
        queue<Token> q = Lexer::tokenize(input);
        if (q.size() != expected_output.size()) {
            cerr << "\n\n[Test Case Failed]: input " << input << endl;
            continue;
        } 
        for (const string &expected_token: expected_output) {
            Token actual_token = q.front();
            q.pop();
            if (actual_token.value != expected_token) {
                cerr << "\n\n[Test Case Failed]: input " << input << endl;
                failed = true;
                break;
            }
        }
        if (!failed) ++success;
    }

    printf("\n\n\nTotal tc: %d, successful: %d, failed: %d\n\n", (int)test_cases.size(), success, (int)(test_cases.size() - success));
}