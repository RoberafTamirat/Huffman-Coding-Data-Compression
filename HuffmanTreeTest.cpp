#include "HuffmanTree.h"

#include <iostream>
#include <vector>
#include <cassert>
#include <stdexcept>

using namespace std;


void printTree(HuffmanNode* node) {

    if (node == nullptr) {
        return;
    }

    if (node->isLeaf()) {
        cout << "Character: " << node->character
             << " | Frequency: " << node->frequency
             << endl;
    }
    else {
        cout << "Internal Node | Frequency: "
             << node->frequency << endl;
    }

    printTree(node->left);
    printTree(node->right);
}

int main() {

    cout << "HUFFMAN TREE TESTING" << endl;
    cout << "====================" << endl;

    
    HuffmanTree tree;

    vector<pair<char, int>> frequencies = {
        {'A', 5},
        {'B', 9},
        {'C', 12},
        {'D', 13},
        {'E', 16},
        {'F', 45}
    };

    tree.buildTree(frequencies);

    assert(!tree.isEmpty());
    assert(tree.getRoot()->frequency == 100);

    cout << "\nTest 1: Normal input PASSED" << endl;

    HuffmanTree emptyTree;

    emptyTree.buildTree({});

    assert(emptyTree.isEmpty());

    cout << "Test 2: Empty input PASSED" << endl;

    
    HuffmanTree singleTree;

    singleTree.buildTree({{'A', 10}});

    HuffmanNode* singleRoot = singleTree.getRoot();

    assert(singleRoot != nullptr);
    assert(singleRoot->character == 'A');
    assert(singleRoot->frequency == 10);
    assert(singleRoot->isLeaf());

    cout << "Test 3: Single character PASSED" << endl;

    
    try {
        tree.buildTree({
            {'A', 5},
            {'A', 10}
        });

        assert(false);
    }
    catch (const invalid_argument&) {
        cout << "Test 4: Duplicate input PASSED" << endl;
    }

    
    try {
        tree.buildTree({
            {'A', 0}
        });

        assert(false);
    }
    catch (const invalid_argument&) {
        cout << "Test 5: Invalid frequency PASSED" << endl;
    }

      tree.buildTree(frequencies);

    cout << "\nConstructed Huffman Tree:" << endl;

    printTree(tree.getRoot());

    cout << "\nAll tests completed successfully!" << endl;

    return 0;
}