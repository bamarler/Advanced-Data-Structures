#include <iostream>
#include <vector>
#include <cmath>
#include <numeric>
#include <map>
#include <algorithm>
#include <random>
#include <fstream>
#include <sstream>
#include <cctype>
using namespace std;

class Node {
    public:
        string key;
        int value;
        Node* next;

        Node(const string& key, int value = 1, Node* next = nullptr)
            : key(key), value(value), next(next) {}
};

class HashTable {
    public:
        HashTable(int size);
        ~HashTable();

        void insert(const string& key, int value);
        bool deleteKey(const string& key);
        bool increase(const string& key, int value);
        int find(const string& key) const;
        void listAllKeys() const;
        vector<int> calculateCollisionLengths();
        void printCollisionsHistogram();
        double calculateVariance();
        void printLongestLists();
    
    private:
        int hashFunction(const string& key) const;
        int randomHash(const string& key) const;
        int murmurHash(const string& key) const;
        vector<Node*> table;
        int size;

        int a;
        int b;

        uint32_t seed;
};

// prime number to be used for randomization and hashing
const int PRIME = 10000019;

HashTable::HashTable(int size) : size(size), table(size, nullptr) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dist(1, PRIME-1);

    // initialize constants for randomHash
    a = dist(gen);
    b = dist(gen);

    // initialize seed for murmurHash
    seed = static_cast<uint32_t>(dist(gen));
}

// Clears the entire HashTable from memory
HashTable::~HashTable() {
    for (int i = 0; i < size; ++i) {
        Node* current = table[i];
        while (current != nullptr) {
            Node* toDelete = current;
            current = current->next;
            delete toDelete;
        }
    }
}

int HashTable::hashFunction(const string& key) const {
    return randomHash(key);
}

int HashTable::randomHash(const string& key) const {
    unsigned long hashValue = 0;
    
    // Position sensitive hashing ("abc" and "cba" produce different values)
    for (char c : key) {
        hashValue = (hashValue * 31 + c) % PRIME;
    }

    // use randomized values to modify value
    hashValue = (a * hashValue + b) % PRIME;

    // produce a final value within our size
    return hashValue % size;
}

int HashTable::murmurHash(const string& key) const {    
    uint32_t hash = seed;

    for (char c : key) {
        uint32_t k = static_cast<uint32_t>(c);

        k *= 0xcc9e2d51;
        k = (k << 15) | (k >> 17);
        k *= 0x1b873593;

        hash ^= k;
        hash = (hash << 13) | (hash >> 19);
        hash = hash * 5 + 0xe6546b64;
    }

    hash ^= key.length();
    hash ^= (hash >> 16);
    hash *= 0x85ebca6b;
    hash ^= (hash >> 13);
    hash *= 0xc2b2ae35;
    hash ^= (hash >> 16);

    return hash % size;
}

void HashTable::insert(const string& key, int value) {
    if (find(key) != -1) {
        increase(key, value);
    } else {
        int index = hashFunction(key);

        // add new node at the beginning of the corresponding linked list
        Node* newNode = new Node(key, value, table[index]);
        table[index] = newNode;
    }
}

bool HashTable::deleteKey(const string& key) {
    int index = hashFunction(key);
    Node* current = table[index];
    Node* previous = nullptr;

    while (current != nullptr) {
        if (current->key == key) {
            if (previous == nullptr) {
                table[index] = current->next;
            }
            else {
                previous->next = current->next;
            }
            delete current;
            return true;
        }
        previous = current;
        current = current->next;
    }

    // key not found, did not delete anything
    return false;
}

bool HashTable::increase(const string& key, int value) {
    int index = hashFunction(key);
    Node* current = table[index];

    while (current != nullptr) {
        if (current->key == key) {
            current->value += value;
            return true;
        }
        current = current->next;
    }

    // key not found, didn't increase
    return false;
}

int HashTable::find(const string& key) const {
    int index = hashFunction(key);
    Node* current = table[index];

    while (current != nullptr) {
        if (current->key == key) {
            return current->value;
        }
        current = current->next;
    }

    return -1;
}

void HashTable::listAllKeys() const {
    for (Node* head : table) {
        Node* current = head;

        while (current != nullptr) {
            cout << "Key: " << current->key << ", Value: " << current->value << endl;
            current = current->next;
        }
    }
}

vector<int> HashTable::calculateCollisionLengths() {
    vector<int> lengths;

    for (Node* head : table) {
        int length = 0;
        Node* current = head;

        while (current != nullptr) {
            ++length;
            current = current->next;
        }

        lengths.push_back(length);
    }

    return lengths;
}

void HashTable::printCollisionsHistogram() {
    vector<int> lengths = calculateCollisionLengths();

    int min = INT_MAX;
    int max = INT_MIN;
    for (int length : lengths) {
        if (length < min) {
            min = length;
        }
        if (length > max) {
            max = length;
        }
    }

    vector<int> buckets(max + 1, 0);

    for (int length : lengths) {
        ++buckets[length];
    }

    int i = 0;
    while (buckets[i] == 0) {
        ++i;
    }

    while (i < max + 1) {
        cout << i << " | ";
        for (int j = 0; j < buckets[i]; j+=1) {
            cout << "*";
        }
        cout << " (" << buckets[i] << ")" << endl;
        ++i;
    }
}

double HashTable::calculateVariance() {
    vector<int> lengths = calculateCollisionLengths();

    double mean = accumulate(lengths.begin(), lengths.end(), 0.0) / lengths.size();
    double variance = 0;
    for (int length : lengths) {
        variance += (length - mean) * (length - mean);
    }

    return variance / lengths.size();
}

void HashTable::printLongestLists() {
    vector<int> lengths = calculateCollisionLengths();

    sort(lengths.begin(), lengths.end(), greater<int>());
    cout << "Lengths of the 10% longest lists are: ";
    for (int i = 0; i < lengths.size() / 10; ++i) {
        cout << lengths[i] << " ";
    }
    cout << endl;
}

void loadFileIntoHashTable(const string& filename, HashTable& hashTable) {
    ifstream file(filename);
    string line;
    string word;

    while (getline(file,line)) {
        istringstream iss(line);
        while (iss >> word) {
            // Normalize the word (convert to lowercase and remove punctuation)
            word.erase(remove_if(word.begin(), word.end(), ::ispunct), word.end());
            transform(word.begin(), word.end(), word.begin(), ::tolower);
            
            // Insert the word into the hash table
            hashTable.insert(word, 1);
        }
    }
}

int main() {
    int m;
    cout << "Enter the size of the Hash table: ";
    cin >> m;

    HashTable hashTable = HashTable(m);

    loadFileIntoHashTable("alice_in_wonderland.txt", hashTable);

    cout << "\nReading Alice in Wonderland..." << endl;

    cout << "\nHistogram of Collision Lists' Lengths:" << endl;
    hashTable.printCollisionsHistogram();

    cout << "\nVariance of collision list lengths: " << hashTable.calculateVariance() << "\n" << endl;

    hashTable.printLongestLists();
}
