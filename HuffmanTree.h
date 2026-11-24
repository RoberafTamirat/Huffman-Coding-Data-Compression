#ifndef HUFFMAN_TREE_H
#define HUFFMAN_TREE_H

#include "HuffmanNode.h"

#include <vector>
#include <utility>
#include <memory>

class HuffmanTree {
private:
    HuffmanNode* root;

    std::vector<std::unique_ptr<HuffmanNode>> nodes;

public:
    
    HuffmanTree();

     HuffmanTree(const HuffmanTree&) = delete;
    HuffmanTree& operator=(const HuffmanTree&) = delete;

    void buildTree(
        const std::vector<std::pair<char, int>>& frequencies
    );

    HuffmanNode* getRoot() const;
    bool isEmpty() const;
};

#endif