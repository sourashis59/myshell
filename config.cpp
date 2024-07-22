#pragma once
using namespace std;

//* singleton
class Config {
    bool debug_mode;

public:
    static Config& get_instance() {
        //* Static instance created once
        static Config instance; 
        return instance;
    }

    int get_debug_mode() const {
        return debug_mode;
    }

    void set_debug_mode(bool mode) {
        debug_mode = mode;
    }

private:
    //* Private constructor and destructor to prevent instantiation
    Config() {
        //* Default debug mode is off
        debug_mode = false; 
    }

    //* Private destructor
    ~Config() {} 
    
    //* Private copy constructor and assignment operator to prevent copying
    Config(const Config&) = delete;
    Config& operator=(const Config&) = delete;
};
