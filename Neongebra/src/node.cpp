//
// Created by AHMET ERDEM on 5.05.2024.
//

#include "../include/node.h"
#include <string>

template <typename T>
node<T>::node(const T& in) {
    this->data = in;
}

template <typename T>
nodelist<T>::nodelist(const T& in) {
    this->head = new node<T>(in);
    this->length = 1;
}

template <typename T>
void nodelist<T>::append(const T& in) {
    auto to_add = new node(in);
    if (this->length == 0) {
        this->head = to_add;
    } else {
        auto temp = this->head;
        for (int i = 1; i < this->length; i++) {
            temp = temp->next;
        }
        temp->next = to_add;
    }
    this->length++;
}

template <typename T>
bool nodelist<T>::remove(T in) {
    if (this->length == 0) return false;
    if (this->head->data == in) {
        auto temp = this->head;
        this->head = this->head->next;
        delete temp;
        this->length--;
        return true;
    }
    auto temp = this->head;
    for (int i = 1; i < this->length; i++) {
        if (temp->next->data == in) { // never gives segmentation fault, length=1 for the dangerous case
            auto to_delete = temp->next;
            temp->next = temp->next->next;
            delete to_delete;
            this->length--;
            return true;
        }
        temp = temp->next;
    }
    return false;
}

template <typename T>
int nodelist<T>::index(T in) const {
    auto temp = this->head;
    for (int i = 0; i < this->length; i++) {
        if (temp->data == in) return i;
        temp = temp->next;
    }
    return -1; // That means not found.
}

template <typename T>
void nodelist<T>::clear() {
    auto temp = this->head;
    node<T>* to_delete;
    for (int i = 0; i < this->length; i++) {
        to_delete = temp;
        temp = temp->next;
        delete to_delete;
    }
}

template <typename T>
gnode<T>::gnode() {
    this->connected = new nodelist<gnode<T>*>;
    this->connected->append(this);
    this->weights = new nodelist<T>;
    this->weights->append(0);
}

template <typename T>
gnode<T>::gnode(const std::string& said_label) {
    this->label = said_label;
    this->connected = new nodelist<gnode<T>*>;
    this->connected->append(this);
    this->weights = new nodelist<T>;
    this->weights->append(0);
}

template <typename T>
void gnode<T>::__connectUndirected(gnode<T>* said_gnode, T weight) {

    // The most special case, connection to self:
    if (this == said_gnode) {
        this->weights->head->data = weight;
        return;
    }

    node<gnode<T>*>* temp = this->connected->head;
    for (int i = 0; i < this->connected->length; i++) {
        if (temp->data->label == said_gnode->label) {
            // i is the matching index
            auto traverse_weights = this->weights->head;
            int j; // Economize the stack memory
            for (j = 0; j < i; j++) traverse_weights = traverse_weights->next;
            traverse_weights->data = weight;

            j = said_gnode->connected->index(this);
            traverse_weights = said_gnode->weights->head;
            while (j > 0) {
                traverse_weights = traverse_weights->next;
                j--;
            }
            traverse_weights->data = weight;
            return;
        }
        temp = temp->next;
    }

    this->connected->append(said_gnode);
    this->weights->append(weight);
    said_gnode->connected->append(this);
    said_gnode->weights->append(weight);
}

template <typename T>
void gnode<T>::__connectDirected(gnode<T>* said_gnode, T weight) {

    if (this == said_gnode) {
        // There is no measurable direction in connection to self.
        this->weights->head->data = weight;
        return;
    }

    node<gnode<T>*>* temp = this->connected->head;
    for (int i = 0; i < this->connected->length; i++) {
        if (temp->data->label == said_gnode->label) {

            auto traverse_weights = this->weights->head;
            for (int j = 0; j < i; j++) traverse_weights = traverse_weights->next;
            traverse_weights->data = weight;

            // Other directions weight remains unchanged.
            return;
        }
        temp = temp->next;
    }

    this->weights->append(weight);
    this->connected->append(said_gnode);
    said_gnode->connected->append(this);
    said_gnode->weights->append(0);
}

template <typename T>
bool gnode<T>::__disconnectUndirected(gnode<T>* said_gnode) {
    if (this->connected->length == 0) return false;

    int index = this->connected->index(said_gnode);
    if (index == -1) return false;
    this->connected->remove(said_gnode);
    auto temp = this->weights->head;
    for (int i = 0; i < index; i++) {
        temp = temp->next;
    } // Will remove temp->next
    auto to_delete = temp->next;
    temp->next = temp->next->next;
    delete to_delete;
    this->weights->length--;

    // We may be disconnecting a node
    index = said_gnode->connected->index(this);
    if (index != -1) {
        said_gnode->connected->remove(this);
        temp = said_gnode->weights->head;
        for (int i = 0; i < index; i++) {
            temp = temp->next;
        }
        to_delete = temp->next;
        temp = temp->next->next;
        delete to_delete;
        said_gnode->weights->length--;
    }
    return true;
}

template <typename T>
bool gnode<T>::__disconnectDirected(gnode<T>* said_gnode) {
    if (this->connected->length == 0) return false;

    int index = this->connected->index(said_gnode);
    if (index == -1) return false;
    this->connected->remove(said_gnode);
    auto temp = this->weights->head;
    for (int i = 0; i < index; i++) {
        temp = temp->next;
    } // Will remove temp->next
    auto to_delete = temp->next;
    temp->next = temp->next->next;
    delete to_delete;
    this->weights->length--;
    return true;
}

template <typename T>
void gnode<T>::clear() {
    this->connected->clear();
    this->weights->clear();
}
