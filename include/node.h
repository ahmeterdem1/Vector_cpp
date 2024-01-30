//
// Created by AHMET ERDEM on 30.01.2024.
//

#ifndef VECTOR_CPP_NODE_H
#define VECTOR_CPP_NODE_H
#include <string>

template <typename T>
class node {
public:
    T data;
    node* next = nullptr;

    node() = default;

    explicit node(const T& in) {
        this->data = in;
    }
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

    explicit nodelist(const T& in) {
        this->head = new node<T>(in);
        this->length = 1;
    }

    void append(const T& in) {
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

    bool remove(T in) {
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

    int index(T in) {
        auto temp = this->head;
        for (int i = 0; i < this->length; i++) {
            if (temp->data == in) return i;
            temp = temp->next;
        }
        return -1; // That means not found.
    }

    void clear() {
        auto temp = this->head;
        node<T>* to_delete;
        for (int i = 0; i < this->length; i++) {
            to_delete = temp;
            temp = temp->next;
            delete to_delete;
        }
    }
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
    gnode() {
        this->connected = new nodelist<gnode<T>*>;
        this->connected->append(this);
        this->weights = new nodelist<T>;
        this->weights->append(0);
    }

    explicit gnode(const std::string& said_label) {
        this->label = said_label;
        this->connected = new nodelist<gnode<T>*>;
        this->connected->append(this);
        this->weights = new nodelist<T>;
        this->weights->append(0);
    }

    void __connectUndirected(gnode<T>* said_gnode, T weight = 1) {

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

    void __connectDirected(gnode<T>* said_gnode, T weight = 1) {

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

    bool __disconnectUndirected(gnode<T>* said_gnode) {
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

    bool __disconnectDirected(gnode<T>* said_gnode) {
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

    void clear() {
        this->connected->clear();
        this->weights->clear();
    }
};


#endif //VECTOR_CPP_NODE_H
