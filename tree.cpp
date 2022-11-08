#include <tree.h>

#include <iostream>
using namespace std;


void RedBlackTree::initializeNULLNode(NodePtr node, NodePtr parent) {
    node->data = 0;
    node->parent = parent;
    node->left = nullptr;
    node->right = nullptr;
    node->color = 0;
    node->b = nullptr;
}

// Preorder
void RedBlackTree::preOrderHelper(NodePtr node) {
    if (node != TNULL) {
        cout << node->data << " ";
        preOrderHelper(node->left);
        preOrderHelper(node->right);
    }
}

// Inorder
void RedBlackTree::inOrderHelper(NodePtr node) {
    if (node != TNULL) {
        inOrderHelper(node->left);
        cout << node->data << " ";
        inOrderHelper(node->right);
    }
}

// Post order
void RedBlackTree::postOrderHelper(NodePtr node) {
    if (node != TNULL) {
        postOrderHelper(node->left);
        postOrderHelper(node->right);
        cout << node->data << " ";
    }
}

NodePtr RedBlackTree::searchTreeHelperCorrectBlock(NodePtr node, size_t key) {
    if (node == TNULL || (key <= node->data && !node->b->busy))
        return node;

    NodePtr left = searchTreeHelperCorrectBlock(node->left, key);
    if (left == TNULL)
        return searchTreeHelperCorrectBlock(node->right, key);
    else 
        return left;
}

bool RedBlackTree::checkFree(NodePtr node) {
    if (node == TNULL)
        return true;
    
    bool free = !node->b->busy;
    bool leftFree = checkFree(node->left);
    bool rightFree = checkFree(node->right);
    return leftFree && rightFree && free;
}

NodePtr RedBlackTree::searchTreeHelper(NodePtr node, size_t key) {
    if (node == TNULL || key == node->data) {
        return node;
    }

    if (key < node->data) {
        return searchTreeHelper(node->left, key);
    }
    return searchTreeHelper(node->right, key);
}

// For balancing the tree after deletion
void RedBlackTree::deleteFix(NodePtr x) {
    NodePtr s;

    while (x != root && x->color == 0) {
        if (x == x->parent->left) {
            s = x->parent->right;
            if (s->color == 1) {
                s->color = 0;
                x->parent->color = 1;
                leftRotate(x->parent);
                s = x->parent->right;
            }

            if ((s->left->color == 0 && s->right->color == 0)) {
                s->color = 1;
                x = x->parent;
            } else {
                if (s->right->color == 0) {
                    s->left->color = 0;
                    s->color = 1;
                    rightRotate(s);
                    s = x->parent->right;
                }

                s->color = x->parent->color;
                x->parent->color = 0;
                s->right->color = 0;
                leftRotate(x->parent);
                x = root;
            }
        } else {
            s = x->parent->left;
            if (s->color == 1) {
                s->color = 0;
                x->parent->color = 1;
                rightRotate(x->parent);
                s = x->parent->left;
            }

            if (s->right->color == 0 && s->right->color == 0) {
                s->color = 1;
                x = x->parent;
            } else {
                if (s->left->color == 0) {
                s->right->color = 0;
                s->color = 1;
                leftRotate(s);
                s = x->parent->left;
                }

                s->color = x->parent->color;
                x->parent->color = 0;
                s->left->color = 0;
                rightRotate(x->parent);
                x = root;
            }
        }
    }
    x->color = 0;
}

void RedBlackTree::rbTransplant(NodePtr u, NodePtr v) {
    if (u->parent == nullptr) {
        root = v;
    } else if (u == u->parent->left) {
        u->parent->left = v;
    } else {
        u->parent->right = v;
    }
    v->parent = u->parent;
}

void RedBlackTree::deleteNodeHelper(NodePtr node, size_t key, block* ptr) {
    NodePtr z = TNULL;
    NodePtr x, y;
    NodePtr nodeTemp = node;
    while (node != TNULL) {
        if (node->data == key && (!ptr || (ptr && node->b == ptr)))
            z = node;
        if (node->data <= key) {
        node = node->right;
        } else {
        node = node->left;
        }
    }

    // if (node == TNULL)
    //     node = nodeTemp;

    // while (node != TNULL) {
    //     if (node->data == key && (!ptr || (ptr && node->b == ptr)))
    //         z = node;

    //     if (node->data < key) {
    //     node = node->right;
    //     } else {
    //     node = node->left;
    //     }
    // }

    if (z == TNULL) {
        cout << "Key not found in the tree" << endl;
        return;
    }

    y = z;
    int y_original_color = y->color;
    if (z->left == TNULL) {
        x = z->right;
        rbTransplant(z, z->right);
    } else if (z->right == TNULL) {
        x = z->left;
        rbTransplant(z, z->left);
    } else {
        y = minimum(z->right);
        y_original_color = y->color;
        x = y->right;
        if (y->parent == z) {
        x->parent = y;
        } else {
        rbTransplant(y, y->right);
        y->right = z->right;
        y->right->parent = y;
        }

        rbTransplant(z, y);
        y->left = z->left;
        y->left->parent = y;
        y->color = z->color;
    }
    delete z;
    if (y_original_color == 0) {
        deleteFix(x);
    }
}

// For balancing the tree after insertion
void RedBlackTree::insertFix(NodePtr k) {
    NodePtr u;
    while (k->parent->color == 1) {
        if (k->parent == k->parent->parent->right) {
        u = k->parent->parent->left;
        if (u->color == 1) {
            u->color = 0;
            k->parent->color = 0;
            k->parent->parent->color = 1;
            k = k->parent->parent;
        } else {
            if (k == k->parent->left) {
            k = k->parent;
            rightRotate(k);
            }
            k->parent->color = 0;
            k->parent->parent->color = 1;
            leftRotate(k->parent->parent);
        }
        } else {
        u = k->parent->parent->right;

        if (u->color == 1) {
            u->color = 0;
            k->parent->color = 0;
            k->parent->parent->color = 1;
            k = k->parent->parent;
        } else {
            if (k == k->parent->right) {
            k = k->parent;
            leftRotate(k);
            }
            k->parent->color = 0;
            k->parent->parent->color = 1;
            rightRotate(k->parent->parent);
        }
        }
        if (k == root) {
        break;
        }
    }
    root->color = 0;
}

void RedBlackTree::printHelper(NodePtr root, string indent, bool last) {
    if (root != TNULL) {
        cout << indent;
        if (last) {
        cout << "R----";
        indent += "   ";
        } else {
        cout << "L----";
        indent += "|  ";
        }

        string sColor = root->color ? "RED" : "BLACK";
        Block block = root->b;

        cout << (block.isBusy() ? "busy " : "free ") << root->data << " | " << block.getPrevSize() << " | " << block.getOffset() <<  (block.isFirst() ? "  first" : " ") << 
            (block.isLast() ? " last " : " ") << " (" << sColor << ")" << endl;

        printHelper(root->left, indent, false);
        printHelper(root->right, indent, true);
    }
}

RedBlackTree::RedBlackTree() {
    cout << "new tree" << endl;
    TNULL = new Node;
    TNULL->color = 0;
    TNULL->left = nullptr;
    TNULL->right = nullptr;
    root = TNULL;
}

void RedBlackTree::RedBlackTree::preorder() {
    preOrderHelper(this->root);
}

void RedBlackTree::inorder() {
    inOrderHelper(this->root);
}

void RedBlackTree::postorder() {
    postOrderHelper(this->root);
}

block* RedBlackTree::findCorrectBlock(size_t size) {
    while (this->root->parent)
        this->root = this->root->parent;
    NodePtr ptr = searchTreeHelperCorrectBlock(this->root, size);
    if (ptr != TNULL)
    {
        return ptr->b;
    }
    else
        return NULL;
}

bool RedBlackTree::checkAllFree() {
    while (this->root->parent)
        this->root = this->root->parent;
    return checkFree(this->root);
}

block* RedBlackTree::searchTree(size_t k) {
    while (this->root->parent)
        this->root = this->root->parent;
    return searchTreeHelper(this->root, k)->b;
}

NodePtr RedBlackTree::minimum(NodePtr node) {
    while (node->left != TNULL) {
        node = node->left;
    }
    return node;
}

NodePtr RedBlackTree::maximum(NodePtr node) {
    while (node->right != TNULL) {
        node = node->right;
    }
    return node;
}

NodePtr RedBlackTree::successor(NodePtr x) {
    if (x->right != TNULL) {
        return minimum(x->right);
    }

    NodePtr y = x->parent;
    while (y != TNULL && x == y->right) {
        x = y;
        y = y->parent;
    }
    return y;
}

NodePtr RedBlackTree::predecessor(NodePtr x) {
    if (x->left != TNULL) {
        return maximum(x->left);
    }

    NodePtr y = x->parent;
    while (y != TNULL && x == y->left) {
        x = y;
        y = y->parent;
    }

    return y;
}

void RedBlackTree::leftRotate(NodePtr x) {
    NodePtr y = x->right;
    x->right = y->left;
    if (y->left != TNULL) {
        y->left->parent = x;
    }
    y->parent = x->parent;
    if (x->parent == nullptr) {
        this->root = y;
    } else if (x == x->parent->left) {
        x->parent->left = y;
    } else {
        x->parent->right = y;
    }
    y->left = x;
    x->parent = y;
}

void RedBlackTree::rightRotate(NodePtr x) {
    NodePtr y = x->left;
    x->left = y->right;
    if (y->right != TNULL) {
        y->right->parent = x;
    }
    y->parent = x->parent;
    if (x->parent == nullptr) {
        this->root = y;
    } else if (x == x->parent->right) {
        x->parent->right = y;
    } else {
        x->parent->left = y;
    }
    y->right = x;
    x->parent = y;
}

// Inserting a node
void RedBlackTree::insert(size_t key, block* b) {
    cout << "insert key " <<  key << endl;
    NodePtr node = new Node;
    node->parent = nullptr;
    node->data = key;
    node->left = TNULL;
    node->right = TNULL;
    node->color = 1;
    node->b = b;

    NodePtr y = nullptr;
    NodePtr x = this->root;

    while (x != TNULL) {
        y = x;
        if (node->data < x->data) {
        x = x->left;
        } else {
        x = x->right;
        }
    }

    node->parent = y;
    if (y == nullptr) {
        root = node;
    } else if (node->data < y->data) {
        y->left = node;
    } else {
        y->right = node;
    }

    if (node->parent == nullptr) {
        node->color = 0;
        return;
    }

    if (node->parent->parent == nullptr) {
        return;
    }

    insertFix(node);
}

NodePtr RedBlackTree::getRoot() {
    return this->root;
}

void RedBlackTree::deleteNode(size_t data) {
    cout << "delete key " << data << endl;
    deleteNodeHelper(this->root, data, NULL);
}

void RedBlackTree::deleteNode(size_t data, block* ptr) {
    cout << "delete key " << data << endl;
    deleteNodeHelper(this->root, data, ptr);
}

void RedBlackTree::printTree() {
    if (root) {
        while (this->root->parent)
            this->root = this->root->parent;
        printHelper(this->root, "", true);
    }
}
