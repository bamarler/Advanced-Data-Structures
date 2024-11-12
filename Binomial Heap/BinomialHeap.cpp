#include <iostream>
#include <climits>

class Node {
    public:
        int key;
        int order;
        Node* parent;
        Node* child;
        Node* sibling;

        Node(int k) : key(k), order(0), parent(nullptr), child(nullptr), sibling(nullptr) {}
};

class BinomialHeap {
    public:
        Node* head;

        BinomialHeap() : head(nullptr) {}

        void insert(int key);
        Node* minimum();
        void extractMin();
        void decreaseKey(Node* node, int newKey);
        void deleteNode(Node* node);
        void unionHeaps(BinomialHeap* otherHeap);
    
    private:
        void linkTrees(Node* y, Node*);
};

void BinomialHeap::linkTrees(Node* y, Node* z) {
    y->parent = z;
    y->sibling = z->child;
    z->child = y;
    z->order++;
}