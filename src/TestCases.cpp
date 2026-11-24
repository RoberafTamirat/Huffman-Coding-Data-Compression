
#include <iostream>
#include <string>
#include <limits>
#include "CompressionAnalyzer.h"

using namespace std;

int main() {
    const CompressionAnalyzer analyzer;
    int choice;

    while (true) {
        cout << "\n===== HUFFMAN TESTING SYSTEM =====\n";
        cout << "1. Short text\n";
        cout << "2. Repeated characters\n";
        cout << "3. Different characters\n";
        cout << "4. Long text\n";
        cout << "5. Empty input\n";
        cout << "6. Single character\n";
        cout << "7. Enter your own text\n";
        cout << "0. Exit\n";
        cout << "Choose: ";

        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Enter a number from 0 to 7.\n";
            continue;
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        string text;

        switch (choice) {
            case 1:
                text = "HELLO";
                break;
            case 2:
                text = "AAAAAAAAAAAA";
                break;
            case 3:
                text = "ABCDEF";
                break;
            case 4:
                text = "Huffman coding is a data compression algorithm "
                       "that reduces the size of data.";
                break;
            case 5:
                text = "";
                break;
            case 6:
                text = "A";
                break;
            case 7:
                cout << "Enter text: ";
                getline(cin, text);
                break;
            case 0:
                cout << "Testing finished.\n";
                return 0;
            default:
                cout << "Invalid choice. Try again.\n";
                continue;
        }

        cout << "\nInput: " << text << "\n";

        CompressionStats result = analyzer.analyze(text);
        analyzer.displayResults(result);
    }

    return 0;
}

