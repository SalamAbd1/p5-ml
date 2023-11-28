#include <iostream>
#include "BinarySearchTree.hpp"
#include "Map.hpp"
#include "csvstream.hpp"

using namespace std;

int main(int argc, char* argv[]) {
    if ((argc != 4 && argc != 3) || (argc == 4 && argv[3] != "--debug")) { 
        cout << "Usage: main.exe TRAIN_FILE TEST_FILE [--debug]" << endl;
        return 1; 
    }

    try {
        csvstream trainStream(argv[1]);
    }

    catch (csvstream_exception failToOpen) {
        cout << "Error opening file: " << argv[1] << endl;
        return 1;
    }

    try {
        csvstream testStream(argv[2]);
    }

    catch (csvstream_exception failToOpen) {
        cout << "Error opening file: " << argv[2] << endl;
        return 1;
    }
}