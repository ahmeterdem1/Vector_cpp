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

    Graph() {
        this->vertices = new nodelist<gnode<T>*>;
    }

    friend std::ostream& operator<< (std::ostream& o, const Graph& g) {
        if (g.vertices->length == 0) {
            o << "[EmptyGraph]" << std::endl;
            return o;
        }

        auto temp = g.vertices->head;
        for (int i = 0; i < g.vertices->length; i++) {
            o << temp->data->label << ": ";
            for (int j = 0; j < temp->data->connected->length; j++) {

            }
            temp = temp->next;
        }
    }

    void addVertex(const std::string& given_label) {
        if (this->vertices->length == 0) {
            auto new_gnode = new gnode<T>(given_label);
            this->vertices->head = new node<gnode<T>*>(new_gnode);
            //new_gnode->__connectUndirected(new_gnode, 0); // Connect to itself with 0 weight
        } else {
            auto temp = this->vertices->head; // node<gnode<T>*>
            for (int i = 1; i < this->vertices->length; i++) {
                if (temp->data->label == given_label) return;
                temp = temp->next;
            }
            if (temp->data->label == given_label) return;
            auto new_gnode = new gnode<T>(given_label);
            temp->next = new node<gnode<T>*>(new_gnode);

            // Connect to all other vertices always undirectionally
            // with weight 0. Length is not yet incremented.
            temp = this->vertices->head;
            for (int i = 0; i < this->vertices->length; i++) {
                temp->data->__connectUndirected(new_gnode, 0);
                temp = temp->next;
            }
            //new_gnode->__connectUndirected(new_gnode, 0);
        }

        this->vertices->length++;
    };

    void addEdge(const std::string& from, const std::string& to, T weight = 1) {
        auto temp = this->vertices->head;
        gnode<T>* from_gnode;
        gnode<T>* to_gnode;
        bool found = false;
        for (int i = 0; i < this->vertices->length; i++) {
            if (temp->data->label == from) {
                from_gnode = temp->data;
                if (found) break;
                found = true;
            }

            if (temp->data->label == to) {
                to_gnode = temp->data;
                if (found) break;
                found = true;
            }
            temp = temp->next;
        }

        if (this->directed) {
            from_gnode->__connectDirected(to_gnode, weight);
        } else {
            from_gnode->__connectUndirected(to_gnode, weight);
        }

    };
    void popVertex();
    void popEdge();
    bool isConnected();
    bool isEuler();
    Matrix<T> generateAdjacency() {
        //bugs bugs bugs
        if (this->vertices->length == 0) return Matrix<T>();

        Vector<T> new_data[this->vertices->length];
        auto temp = this->vertices->head;
        node<T>* tempweight;
        for (int i = 0; i < this->vertices->length; i++) {
            T templist[this->vertices->length];
            tempweight = temp->data->weights->head;
            for (int j = 0; j < this->vertices->length; j++) {
                *(templist + j) = tempweight->data;
                tempweight = tempweight->next;
            }
            Vector<T> tempv(this->vertices->length, templist);
            // I don't know how to explain below "ordering" code.
            // It is probably the most efficient solution though.
            tempv.length = i + 1;
            tempv.shift(1, false);
            tempv.length = this->vertices->length;
            *(new_data + i) = tempv;
            temp = temp->next;
        }

        Matrix<T> result(this->vertices->length, this->vertices->length, new_data);
        for (int i = 0; i < this->vertices->length; i++) {
            (new_data + i)->clear();
        }
        return result;
    }
    void clear() {
        if (this->vertices->length == 0) return;
        node<gnode<T>*>* temp = this->vertices->head;
        node<gnode<T>*>* to_delete;
        for (int i = 0; i < this->vertices->length; i++) {
            to_delete = temp;
            temp->data->clear(); // clear the gnode<T>
            temp = temp->next;
            delete to_delete;
        }
        //this->vertices->clear();
    };
};

#endif //VECTOR_CPP_GRAPH_H
