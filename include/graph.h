//
// Created by AHMET ERDEM on 30.01.2024.
//

#ifndef VECTOR_CPP_GRAPH_H
#define VECTOR_CPP_GRAPH_H
#include "./node.h"
#include "./matrix.h"

template <typename T>
class Graph {
public:
    bool traverse_flag = true;
    bool directed = false;
    nodelist<gnode<T>*>* vertices = nullptr;

    Graph();

    friend std::ostream& operator<< (std::ostream& o, const Graph& g);

    void addVertex(const std::string& given_label);

    void addEdge(const std::string& from, const std::string& to, T weight = 1);

    void popVertex();
    void popEdge();
    bool isConnected();
    bool isEuler();

    Matrix<T> generateAdjacency();

    void clear();

};

#endif //VECTOR_CPP_GRAPH_H
