#include <iostream>
#include <vector>

using namespace std;

const string RED = "red";
const string BLACK = "black";

class Node {
    public:
        int key;
        string color;
        Node* parent;
        Node* left;
        Node* right;

        Node(int k, string color) : key(k), color(color), parent(nullptr), left(nullptr), right(nullptr) {}
};

class RedBlackTree {
    public:
        Node* root;

        RedBlackTree() : root(nullptr) {}

        void sort(Node* x);
        Node* search(Node* x, int target);
        Node* min(Node* x);
        Node* max(Node* x);
        Node* successor(Node* x);
        Node* predecessor(Node* x);
        void insert(int key);
        void deleteNode(Node* target);
        void printTree();
    
    private:
        void leftRotate(Node* x);
        void rightRotate(Node* y);
        void insertFixup(Node* z);
};

void RedBlackTree::sort(Node* x) {
    if (x) {
        sort(x->left);
        cout << x->key << " ";
        sort(x->right);
    }
}

Node* RedBlackTree::search(Node* x, int target) {
    while (x && (target != x->key)) {
        if (target < x->key) {
            x = x->left;
        }
            x = x->right;
    }
    return x;
}

Node* RedBlackTree::min(Node* x) {
    while (x->left) {
        x = x->left;
    }
    return x;
}

Node* RedBlackTree::max(Node* x) {
    while (x->right) {
        x = x->right;
    }
    return x;
}

Node* RedBlackTree::successor(Node* x) {
    if (x->right) {
        return min(x->right);
    }
    else {
        Node* y = x->parent;
        while (y && (x == y->right)) {
            x = y;
            y = y->parent;
        }
        return y;
    }
}

Node* RedBlackTree::predecessor(Node* x) {
    if (x->left) {
        return max(x->left);
    }
    else {
        Node* y = x->parent;
        while (y && (x == y->left)) {
            x = y;
            y = y->parent;
        }
        return y;
    }
}

void RedBlackTree::insert(int key) {
    Node* x = root;
    Node* y = nullptr;
    Node* z = new Node(key, RED);
    while (x) {
        y = x;
        if (z->key < x->key) {
            x = x->left;
        }
        else {
            x = x->right;
        }
    }
    z->parent = y;
    if (!y) {
        root = z;
    }
    else if (z->key < y->key) {
        y->left = z;
    }
    else {
        y->right = z;
    }
    insertFixup(z);
}

void RedBlackTree::leftRotate(Node* x) {
    Node* y = x->right;
    x->right = y->left;
    if (y->left) {
        y->left->parent = x;
    }
    y->parent = x->parent;
    if (!x->parent) {
        root = y;
    }
    else if (x == x->parent->left) {
        x->parent->left = y;
    }
    else {
        x->parent->right = y;
    }
    y->left = x;
    x->parent = y;
}

void RedBlackTree::rightRotate(Node* y) {
    Node* x = y->left;
    y->left = x->right;
    if (x->right) {
        x->right->parent = y;
    }
    x->parent = y->parent;
    if (!y->parent) {
        root = x;
    }
    else if (y == y->parent->right) {
        y->parent->right = x;
    }
    else {
        y->parent->left = x;
    }
    x->right = y;
    y->parent = x;
}

void RedBlackTree::insertFixup(Node* z) {
    while (z->parent->color == RED) {
        if (z->parent == z->parent->parent->left) {
            Node* y = z->parent->parent->right;
            if (y->color == RED) {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            }
            else {
                if (z == z->parent->right) {
                    z = z->parent;
                    leftRotate(z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                rightRotate(z->parent->parent);
            }
        }
        else {
            Node* y = z->parent->parent->left;
            if (y->color == RED) {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            }
            else {
                if (z == z->parent->left) {
                    z = z->parent;
                    rightRotate(z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                leftRotate(z->parent->parent);
            }
        }
    }
    root->color = BLACK;
}

void loadArrayIntoRBTree(const vector<int>& arr, RedBlackTree* rb) {
    for (int num : arr) {
        cout << "Inserting " << num << " into the heap." << endl;
        rb->insert(num);
    }
}