#include <iostream>
#include <thread>
#include <chrono>
#include <fstream>

using namespace std;

int main() {
    string filename = "test-programs/test1.txt";
    ofstream outFile;

    outFile.open(filename, ios::app);
    if (!outFile.is_open()) {
        cerr << "\nFailed to open the file." << endl;
        exit(1);
    }
    
    for (int i = 0; i < 10; ++i) {
        this_thread::sleep_for(chrono::seconds(1));
    
        outFile << "Hello1: " << i << endl;
        // cout << "Hello1: " << i << endl;
    
        outFile.flush();
        cout.flush();
    }
    
    outFile.close();
    return 0;
}
