#pragma once

#include <unordered_map>
#include <string>
using namespace std;

//* singleton
class Config {
public:
    enum PROMPT_COLOR {
        DEFAULT,
        GREEN,
        BLUE,
        RED,
        YELLOW,
        MAGENTA,
        CYAN,
        WHITE,
        GRAY,
        LIGHT_RED,
        LIGHT_GREEN,
        LIGHT_YELLOW,
        LIGHT_BLUE,
        LIGHT_MAGENTA,
        LIGHT_CYAN,
        LIGHT_WHITE
    };

    bool debug_mode;
    string prompt_color_code;
    string prompt_cwd_color_code;
 
    static unordered_map<PROMPT_COLOR, string> PROMPT_COLOR_CODE;

    static Config& get_instance();

    // int get_debug_mode() const;
    // void set_debug_mode(bool mode);

private:
    //* Private constructor and destructor to prevent instantiation
    Config();
    ~Config();
    
    //* Private copy constructor and assignment operator to prevent copying
    Config(const Config&) = delete;
    Config& operator=(const Config&) = delete;
};
