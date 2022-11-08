#pragma once

// Implementing Red-Black Tree in C++

#include <block.h>
#include <iostream>
using namespace std;

struct Node {
  size_t data;
  block *b;
  Node *parent;
  Node *left;
  Node *right;
  int color;
};

#define NODE_SIZE sizeof(Node); 

typedef Node *NodePtr;

class RedBlackTree {
private:
  NodePtr root;
  NodePtr TNULL;
  void initializeNULLNode(NodePtr node, NodePtr parent);
  void preOrderHelper(NodePtr node);
  void inOrderHelper(NodePtr node);
  void postOrderHelper(NodePtr node);
  NodePtr searchTreeHelperCorrectBlock(NodePtr node, size_t key);
  NodePtr searchTreeHelper(NodePtr node, size_t key);
  void deleteFix(NodePtr x);
  void rbTransplant(NodePtr u, NodePtr v);
  void deleteNodeHelper(NodePtr node, size_t key, block* ptr);
  void insertFix(NodePtr k);
  void printHelper(NodePtr root, string indent, bool last);
public:
  RedBlackTree();
  void preorder();
  void inorder();
  void postorder();
  block* findCorrectBlock(size_t size);
  block* searchTree(size_t k);
  NodePtr minimum(NodePtr node);
  NodePtr maximum(NodePtr node);
  NodePtr successor(NodePtr x);
  NodePtr predecessor(NodePtr x);
  void leftRotate(NodePtr x);
  void rightRotate(NodePtr x);
  void insert(size_t key, block* b);
  NodePtr getRoot();
  void deleteNode(size_t data);
  void deleteNode(size_t data, block* ptr);
  void printTree();
  bool checkFree(NodePtr node);
  bool checkAllFree();
};


