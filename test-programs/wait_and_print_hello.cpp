#include <iostream>
#include <thread>
#include <chrono>
#include <fstream>

using namespace std;

int main() {
    string filename = "test-programs/test.txt";
    ofstream outFile;

    outFile.open(filename, ios::app);
    if (!outFile.is_open()) {
        cerr << "\nFailed to open the file." << endl;
        exit(1);
    }
    
    for (int i = 0; i < 15; ++i) {
        this_thread::sleep_for(chrono::seconds(1));
    
        outFile << "Appended: " << i << endl;
        cout << "Appended: " << i << endl;
    
        outFile.flush();
        cout.flush();
    }
    
    outFile.close();
    return 0;
}
