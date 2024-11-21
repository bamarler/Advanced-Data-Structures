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

        Node* search(Node* x, int target);
        Node* min(Node* x);
        Node* max(Node* x);
        Node* successor(Node* x);
        Node* predecessor(Node* x);
        void insert(int key);
        void deleteNode(Node* target);
        void inlineWalk(Node* x);
        void layeredWalk(Node* x);
    
    private:
        void leftRotate(Node* x);
        void rightRotate(Node* y);
        void insertFixup(Node* z);
        void transplant(Node* u, Node* v);
};

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
        root->color = BLACK;
        return;
    }
    else if (z->key < y->key) {
        y->left = z;
    }
    else {
        y->right = z;
    }
    insertFixup(z);
}

void RedBlackTree::deleteNode(Node* z) {
    if (!z->left) {
        transplant(z, z->right);
    }
    else if (!z->right) {
        transplant(z, z->left);
    }
    else {
        Node* y = min(z->right);
        if (y != z->right) {
            transplant(y, y->right);
            y->right = z->right;
            y->right->parent = y;
        }
        transplant(z, y);
        y->left = z->left;
        y->left->parent = y;
    }  
}

void RedBlackTree::inlineWalk(Node* x) {
    if (x) {
        inlineWalk(x->left);
        cout << x->key << "(" << x->color << ") ";
        inlineWalk(x->right);
    }
}

void RedBlackTree::layeredWalk(Node* x) {
    if (x) {
        cout << " " << x->key << " ";
        layeredWalk(x->left);
        layeredWalk(x->right);
        cout << endl;
    }
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
    while (z && z->parent && z->parent->parent && z->parent->color == RED) {
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
        cout << "Root Key: " << root << "Root Color: " << endl;
    }
    root->color = BLACK;
}

void RedBlackTree::transplant(Node* u, Node* v) {
    if (!u->parent) {
        root = v;
    }
    else if (u == u->parent->left) {
        u->parent->left = v;
    }
    if (v) {
        v->parent = u->parent;
    }
}

void loadArrayIntoRBTree(const vector<int>& arr, RedBlackTree &rb) {
    for (int num : arr) {
        cout << "Inserting " << num << " into the Tree." << endl;
        rb.insert(num);
    }
}

int main() {
    int m;
    cout << "Enter the number of elements to test: ";
    cin >> m;

    // Generate an array of random integers of length m
    vector<int> arr;
    srand(time(0)); // Seed the random number generator
    for (int i = 0; i < m; ++i) {
        int randomNum = rand() % 100; // Random numbers between 0 and 99
        arr.push_back(randomNum);
    }

    RedBlackTree rbTree;

    loadArrayIntoRBTree(arr, rbTree);

    cout << "Print In Line Walk: ";
    rbTree.inlineWalk(rbTree.root);

    RedBlackTree rbTree2;

    cout << "\n\nPrint Test In LIne Walk: ";

    rbTree2.insert(11);
    rbTree2.insert(7);
    rbTree2.insert(15);
    rbTree2.insert(5);

    rbTree2.inlineWalk(rbTree2.root);

    cout << "\n Successor 11: " << rbTree2.successor(rbTree2.search(rbTree2.root, 11))->key;

    return 0;
}