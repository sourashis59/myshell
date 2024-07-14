#include <iostream>
#include <string>
using namespace std;

int main(int argc, char *argv[]) {
    
    string name = "A";
    if (argc > 1) name = argv[1];
    cout << ("Command line arg: " + name) << endl;
    string input;
    while (!cin.eof()) {
        cout << "\n[Program " << name << "]: ";
        getline(cin, input); // Read the whole line, including spaces
        cout.flush();        
        cout << "[Program " << name << "]: GOT INPUT: \"" << input << "\"";
        cout.flush();        

        if (input == "exit") 
            break;
    }

    cout << "\n[Program " << name << "]: Exit!!!!!!!!!!!!" << endl;
    return 0;
}

