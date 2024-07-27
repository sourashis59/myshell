#pragma once

#include <iostream>
#include "../config.h"
#include "SystemCallWrapper.cpp"

//* singleton
class Logger {
    static Logger *instance;
    static Config *config;

    ostream* output_stream;

    Logger(): output_stream(&cout) {}
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete; 

public:
    static Logger* get_instance() {
        //* since this will be used by only a single thread (of shell process)
        //* no need to use lock
        if (instance == nullptr) {
            instance = new Logger();
        }
        return instance;
    }
    
    void set_output_stream(ostream *output_stream) {
        this->output_stream = output_stream;
    }

    void log(const string &message) {
        if (!config->debug_mode)
            return;

        if (config->debug_color_enabled) 
            (*output_stream) << config->debug_color;

        (*output_stream) 
            << "[DEBUG][ProcessId: " 
            << SystemCallWrapper::getpid_wrapper()
            << "]: "
            << message;
        
        if (config->debug_color_enabled)
            (*output_stream) << Config::PROMPT_COLOR_CODE[Config::PROMPT_COLOR::DEFAULT];
        
        (*output_stream)<< "\n";
        output_stream->flush();
    }

};

Logger* Logger::instance = nullptr;
Config *Logger::config = Config::get_instance();