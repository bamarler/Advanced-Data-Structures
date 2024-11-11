#include "HashTable.h"
#include <iostream>
#include <vector>
#include <cmath>
#include <numeric>
#include <algorithm>
#include <random>

// prime number to be used for hashing
const int PRIME = 10000019;

HashTable::HashTable(int size) : size(size), table(size, nullptr) {
    // Produce random constants for hashing
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(1, PRIME-1);

    a = dist(gen);
    b = dist(gen);
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

int HashTable::hashFunction(const std::string& key) const {
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

void HashTable::insert(const std::string& key, int value) {
    if (find(key) != -1) {
        increase(key, value);
    } else {
        int index = hashFunction(key);

        // add new node at the beginning of the corresponding linked list
        Node* newNode = new Node(key, value, table[index]);
        table[index] = newNode;
    }
}

bool HashTable::deleteKey(const std::string& key) {
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

bool HashTable::increase(const std::string& key, int value) {
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

int HashTable::find(const std::string& key) const {
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
            std::cout << "Key: " << current->key << ", Value: " << current->value << std::endl;
            current = current->next;
        }
    }
}

std::vector<int> HashTable::calculateCollisionLengths() {
    std::vector<int> lengths;

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

double HashTable::calculateVariance() {
    std::vector<int> lengths = calculateCollisionLengths();

    double mean = std::accumulate(lengths.begin(), lengths.end(), 0.0) / lengths.size();
    double variance = 0;
    for (int length : lengths) {
        variance += (length - mean) * (length - mean);
    }

    return std::sqrt(variance / lengths.size());

}

void HashTable::printLongestLists() {
    std::vector<int> lengths = calculateCollisionLengths();

    std::sort(lengths.begin(), lengths.end(), std::greater<int>());
    std::cout << "Lengths of the 10% longest lists are: ";
    for (int i = 0; i < lengths.size() / 10; ++i) {
        std::cout << lengths[i] << " ";
    }
    std::cout << std::endl;
}