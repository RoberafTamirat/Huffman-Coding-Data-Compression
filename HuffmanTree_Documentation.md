# Huffman Tree and Priority Queue

## 1. Introduction

Huffman Coding is a lossless data compression
algorithm used to reduce the number of bits
required to represent data.

It assigns shorter binary codes to frequently
occurring characters and longer binary codes
to less frequently occurring characters.

## 2. Huffman Node Structure

A Huffman Node represents a character and
its frequency in the input data.

Each node contains:

- Character
- Frequency
- Left child pointer
- Right child pointer

Leaf nodes represent characters, while
internal nodes represent combined frequencies.

## 3. Huffman Tree Construction

The Huffman Tree is constructed using the
following steps:

1. Calculate the frequency of each character.
2. Create a leaf node for every character.
3. Insert all nodes into a Min Priority Queue.
4. Extract the two nodes with the smallest
   frequencies.
5. Create a new parent node with the sum
   of their frequencies.
6. Connect the extracted nodes as left
   and right children.
7. Insert the parent node into the queue.
8. Repeat until only one node remains.

The remaining node becomes the root of
the Huffman Tree.

## 4. Priority Queue and Min Heap

A Priority Queue is a data structure that
processes elements according to their priority.

A Min Heap is a priority queue where the
element with the smallest value has the
highest priority.

In this project, character frequency is
used as the priority.

C++ STL priority_queue is used to implement
the Min Priority Queue.

## 5. Selected Data Structures

### Huffman Node

Used to store character, frequency,
and child pointers.

### Binary Tree

Used to represent the hierarchical
structure of Huffman Coding.

### Min Priority Queue

Used to efficiently select the two nodes
with the smallest frequencies.

## 6. Time Complexity

Let n represent the number of unique
characters in the input.

- Creating nodes: O(n)
- Inserting nodes: O(n log n)
- Extracting minimum nodes: O(log n)
  per extraction.
- Complete tree construction: O(n log n)

Therefore, the overall time complexity
is O(n log n).

## 7. Space Complexity

The Huffman Tree contains 2n - 1 nodes
for n unique characters.

Therefore, the space complexity is O(n).

## 8. Special Cases

### Empty Input

If the frequency table is empty,
the root is nullptr.

### Single Character

If the input contains one character,
the tree contains only one leaf node.

### Invalid Frequency

Zero or negative frequencies are rejected.

### Duplicate Characters

Duplicate characters are rejected because
each character should appear only once
in the frequency table.

## 9. Conclusion

The Huffman Tree module constructs a
binary tree using a Min Priority Queue.

The constructed tree is provided to the
encoding and decoding modules through
the getRoot() function.