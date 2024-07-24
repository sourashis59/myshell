#include "config.h"


unordered_map<Config::PROMPT_COLOR, string> Config::PROMPT_COLOR_CODE = {
    {DEFAULT, "\033[0m"},    
    {GREEN, "\033[32m"},   
    {BLUE, "\033[34m"},    
    {RED, "\033[31m"},     
    {YELLOW, "\033[33m"},  
    {MAGENTA, "\033[35m"}, 
    {CYAN, "\033[36m"},    
    {WHITE, "\033[37m"},   
    {GRAY, "\033[90m"},    
    {LIGHT_RED, "\033[91m"},  
    {LIGHT_GREEN, "\033[92m"},
    {LIGHT_YELLOW, "\033[93m"},
    {LIGHT_BLUE, "\033[94m"}, 
    {LIGHT_MAGENTA, "\033[95m"},
    {LIGHT_CYAN, "\033[96m"}, 
    {LIGHT_WHITE, "\033[97m"} 
};

Config& Config::get_instance() {
    //* Static instance created once
    static Config instance; 
    return instance;
}

Config::Config() {
    //* default
    debug_mode = false;
    prompt_color_code = Config::PROMPT_COLOR_CODE[PROMPT_COLOR::LIGHT_CYAN];
    prompt_cwd_color_code = Config::PROMPT_COLOR_CODE[PROMPT_COLOR::LIGHT_GREEN];
}


Config::~Config() {}

