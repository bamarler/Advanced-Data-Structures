#ifndef NODE_H
#define NODE_H

#include <string>

class Node {
    public:
        std::string key;
        int value;
        Node* next;

        Node(const std::string& key, int value = 1, Node* next = nullptr)
            : key(key), value(value), next(next) {}
};


#endif