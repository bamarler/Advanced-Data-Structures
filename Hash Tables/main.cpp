#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <cctype>
#include <algorithm>
#include "HashTable.h"

void loadFileIntoHashTable(const std::string& filename, HashTable& hashTable) {
    std::ifstream file(filename);
    std::string line;
    std::string word;

    while (std::getline(file,line)) {
        std::istringstream iss(line);
        while (iss >> word) {
            // Normalize the word (convert to lowercase and remove punctuation)
            word.erase(std::remove_if(word.begin(), word.end(), ::ispunct), word.end());
            std::transform(word.begin(), word.end(), word.begin(), ::tolower);

            // Insert the word into the hash table
            hashTable.insert(word, 1);
        }
    }
}

int main() {
    int m = 1000;
    HashTable hashTable = HashTable(m);

    loadFileIntoHashTable("alice_in_wonderland.txt", hashTable);

    std::cout << "Testing with m = " << m << std::endl;

    std::vector<int> lengths = hashTable.calculateCollisionLengths();
    for (int length : lengths) {
        std::cout << length << " ";
    }
    std::cout << "\n" << std::endl;

    std::cout << "Variance of collision list lengths: " << hashTable.calculateVariance() << "\n" << std::endl;

    hashTable.printLongestLists();
}
