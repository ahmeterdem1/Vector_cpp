//
// Created by AHMET ERDEM on 30.01.2024.
//

#pragma once

#ifndef VECTOR_CPP_NODE_H
#define VECTOR_CPP_NODE_H
#include <string>

template <typename T>
class node {
public:
    T data;
    node* next = nullptr;

    node() = default;

    explicit node(const T& in);
};

template <typename T>
class nodelist {
public:
    node<T>* head = nullptr;
    unsigned int length = 0; // So we don't need to traverse the linked list
                             // whenever we need to measure the length,
                             // and also this single 4 bytes makes this
                             // data structure very much easily codeable

    nodelist() = default;

    explicit nodelist(const T& in);

    void append(const T& in);

    bool remove(T in);

    int index(T in) const;

    void clear();
};

// Graph node
template <typename T>
class gnode {
public:
    std::string label; // Each node will have a label
    bool traversed = false; // This will be an alternating flag
    // Below two lists corresponds each other in their order
    nodelist<gnode<T>*>* connected = nullptr; // and an unknown amount of connections
    nodelist<T>* weights = nullptr;

    // Initialize connected to itself
    gnode();

    explicit gnode(const std::string& said_label);

    void __connectUndirected(gnode<T>* said_gnode, T weight = 1);

    void __connectDirected(gnode<T>* said_gnode, T weight = 1);

    bool __disconnectUndirected(gnode<T>* said_gnode);

    bool __disconnectDirected(gnode<T>* said_gnode);

    void clear();
};


#endif //VECTOR_CPP_NODE_H
