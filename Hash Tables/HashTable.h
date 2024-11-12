#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <string>
#include <random>
#include <vector>
#include "Node.h"

class HashTable {
    public:
        HashTable(int size);
        ~HashTable();

        void insert(const std::string& key, int value);
        bool deleteKey(const std::string& key);
        bool increase(const std::string& key, int value);
        int find(const std::string& key) const;
        void listAllKeys() const;
        std::vector<int> calculateCollisionLengths();
        void printCollisionsHistogram();
        double calculateVariance();
        void printLongestLists();
    
    private:
        int hashFunction(const std::string& key) const;
        int randomHash(const std::string& key) const;
        int murmurHash(const std::string& key) const;
        std::vector<Node*> table;
        int size;

        int a;
        int b;

        uint32_t seed;
};

#endif