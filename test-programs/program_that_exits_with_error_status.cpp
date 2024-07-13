#include <iostream>
#include <thread>
#include <chrono>
#include <fstream>

using namespace std;

int main() {
    this_thread::sleep_for(chrono::seconds(1));
    printf("\n[program_that_exists_with_error_status]: Exiting with 1 status code\n");
    cout.flush();
    exit(1);
    return 1;
}
