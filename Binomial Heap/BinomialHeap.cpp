#include <iostream>
#include <climits>
#include <vector>

using namespace std;

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
        void unionHeaps(BinomialHeap* other);
        void printTree(Node* root, int level);
        void printHeap();
    private:
        // links two trees of the same order
        void mergeTrees(Node* y, Node*);
        Node* mergeRoots(Node* h1, Node* h2);
};

void BinomialHeap::insert(int key) {
    Node* newNode = new Node(key);
    BinomialHeap newHeap;
    newHeap.head = newNode;

    unionHeaps(&newHeap);
}

Node* BinomialHeap::minimum() {
    if (!head) return nullptr;

    Node* minNode = head;
    Node* current = head->sibling;

    while (current) {
        if (current->key < minNode->key) {
            minNode = current;
        }
        current = current->sibling;
    }

    return minNode;
}

void BinomialHeap::extractMin() {
    Node* min = minimum();

    if (head == min) {
        head = head->sibling;
    }
    else {
        Node* prev = head;

        while (prev->sibling != min) {
            prev = prev->sibling;
        }

        prev->sibling = min->sibling;
    }
    min->sibling = nullptr;

    BinomialHeap subtrees;
    subtrees.head = min->child;

    unionHeaps(&subtrees);    

    delete min;
}

void BinomialHeap::decreaseKey(Node* node, int newKey) {
    node->key = newKey;

    while (node->key < node->parent->key) {
        int k = node->key;

        node->key = node->parent->key;
        node->parent->key = k;

        node = node->parent;
    }
}

void BinomialHeap::deleteNode(Node* node) {
    decreaseKey(node, INT_MIN);

    extractMin();
}

void BinomialHeap::unionHeaps(BinomialHeap* other) {
    Node* newHead = mergeRoots(this->head, other->head);
    this->head = nullptr;
    other->head = nullptr;

    if(!newHead) return;

    Node* prev = nullptr;
    Node* curr = newHead;
    Node* next = curr->sibling;

    while (next) {
        if ((curr->order != next->order) || (next->sibling && next->sibling->order == curr->order)) {
            prev = curr;
            curr = next;
        }
        else if (curr->key <= next->key) {
            curr->sibling = next->sibling;
            mergeTrees(next, curr);
        }
        else {
            if (prev) {
                prev->sibling = next;
            } else {
                newHead = next;
            }
            mergeTrees(curr, next);
            curr = next;
        }
        next = curr-> sibling;
    }

    this->head = newHead;
}

void BinomialHeap::printTree(Node* root, int level = 0) {
    if (!root) return;

    // Print the root key and its immediate children on the same line
    cout << string(level * 2, ' ') << root->key;

    Node* child = root->child;
    if (child) {
        cout << " -- " << child->key; // Print the first child of the root
        child = child->sibling;
    }

    // Print remaining siblings of the first child, if any
    while (child) {
        cout << " -- " << child->key;
        child = child->sibling;
    }
    cout << endl;

    // Move to the next level for each child and print their subtree
    child = root->child;
    while (child) {
        // Print the vertical '|' line with indentation for child levels
        cout << string(level * 2, ' ') << "| ";
        
        // Recursive call to print each child's subtree
        printTree(child, level + 1);
        child = child->sibling;
    }
}

void BinomialHeap::printHeap() {
    cout << "Binomial Heap:" << endl;

    Node* current = head;
    int treeIndex = 1;

    // Traverse each tree in the root list
    while (current) {
        printTree(current);  // Print each binomial tree starting from the root node
        current = current->sibling;
        cout << endl; // Separate each tree with a new line for clarity
    }
}

void BinomialHeap::mergeTrees(Node* y, Node* z) {
    y->parent = z;
    y->sibling = z->child;
    z->child = y;
    z->order++;
}

Node* BinomialHeap::mergeRoots(Node* h1, Node* h2) {
    if (!h1) return h2;
    if (!h2) return h1;

    Node* head = nullptr;
    Node* tail = nullptr;

    if (h1->order <= h2->order) {
        head = h1;
        h1 = h1->sibling;
    }
    else {
        head = h2;
        h2 = h2->sibling;
    }
    tail = head;

    while(h1 && h2) {
        if (h1->order <= h2->order) {
            tail->sibling = h1;
            h1 = h1->sibling;
        }
        else {
            tail->sibling = h2;
            h2 = h2->sibling;
        }
        tail = tail->sibling;
    }
    tail->sibling = (h1) ? h1 : h2;
    return head;
}

void loadArrayIntoBinomialHeap(const vector<int>& arr, BinomialHeap* heap) {
    for (int num : arr) {
        cout << "Inserting " << num << " into the heap." << endl;
        heap->insert(num);
    }
}

int main() {
    int m;
    cout << "Enter the number of elements (m) to test: ";
    cin >> m;

    // Generate an array of random integers of length m
    vector<int> arr;
    srand(time(0)); // Seed the random number generator
    for (int i = 0; i < m; ++i) {
        int randomNum = rand() % 100; // Random numbers between 0 and 99
        arr.push_back(randomNum);
    }

    // Create a new binomial heap
    BinomialHeap heap;

    // Load the array into the binomial heap
    loadArrayIntoBinomialHeap(arr, &heap);

    // Print the heap and test various functions
    cout << "\nHeap after all insertions:" << endl;
    heap.printHeap();

    // Find the minimum element in the heap
    Node* minNode = heap.minimum();
    if (minNode) {
        cout << "Minimum element in the heap: " << minNode->key << endl;
    }

    // Extract the minimum element and print the heap after each extraction
    cout << "\nExtracting minimum elements:" << endl;
    while (heap.head) {
        minNode = heap.minimum();
        if (minNode) {
            cout << "Minimum element: " << minNode->key << endl;
        }
        heap.extractMin();
        heap.printHeap();
    }

    // Optionally, demonstrate decreaseKey and deleteNode here
    // Uncomment the lines below to test:
    /*
    if (m > 0) {
        // Re-insert elements to demonstrate decreaseKey and deleteNode
        loadArrayIntoBinomialHeap(arr, &heap);

        // Example decreaseKey: decrease the minimum node's key to 5
        minNode = heap.minimum();
        if (minNode) {
            cout << "\nDecreasing key of node with key " << minNode->key << " to 5" << endl;
            heap.decreaseKey(minNode, 5);
            printHeapSideways(&heap);
        }

        // Example deleteNode: delete the node with key 5
        Node* nodeToDelete = heap.minimum(); // For simplicity, delete the new minimum
        if (nodeToDelete) {
            cout << "\nDeleting node with key " << nodeToDelete->key << endl;
            heap.deleteNode(nodeToDelete);
            printHeapSideways(&heap);
        }
    }
    */

    return 0;
}