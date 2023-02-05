#include <array>
#include <iostream>
#include <limits>
#include <string>
#include <vector>

#define P 0.5
#define SENTINEL_KEY -std::numeric_limits<int>::infinity()
#define SENTINEL_VAL "sentinel"

class SimpleSkipList {
    class Node {
      public:
        int key;
        std::string value;

        Node *next;
        Node *below;

        Node(int key, std::string value, Node *n_next, Node *n_below) {
            this->key = key;
            this->value = value;

            this->next = n_next;
            this->below = n_below;
        }

        void print_node() { std::cout << "{" << key << ": " << value << "}\n"; }
    };

  private:
    Node *top_sentinel;
    unsigned int size;
    unsigned int height;

    int random_new_level() {
        int new_level = 1;
        while (static_cast<float>(rand()) / static_cast<float>(RAND_MAX) <= P) {
            new_level++;
        }

        return new_level;
    }

    void update_height(int new_height) {
        Node *current_sen = top_sentinel;

        while (height < new_height) {
            Node *new_top_sentinel =
                new Node(SENTINEL_KEY, SENTINEL_VAL, NULL, top_sentinel);
            top_sentinel = new_top_sentinel;

            height++;
        }
    }

    // returns the node we created
    Node *update_level(Node *c_sen, Node *above_node, int new_key,
                       std::string new_value) {
        Node *c_node = c_sen;
        while (c_node->next != NULL && c_node->next->key < new_key) {
            c_node = c_node->next;
        }

        Node *new_node = new Node(new_key, new_value, c_node->next, NULL);
        c_node->next = new_node;
        if (above_node != NULL) {
            above_node->below = new_node;
        }

        return new_node;
    }

    void delete_level(Node *sentinel) {
        Node *c_node = sentinel;
        while (c_node != NULL) {
            Node *tmp = c_node;
            c_node = c_node->next;
            delete tmp;
        }
    }

  public:
    SimpleSkipList() {
        this->top_sentinel = new Node(SENTINEL_KEY, SENTINEL_VAL, NULL, NULL);

        size = 0;
        height = 1;
    }

    ~SimpleSkipList() {
        Node *c_sen = top_sentinel;
        while (c_sen->below != NULL) {
            c_sen = c_sen->below;
        }

        std::vector<int> keys;
        for (Node *v = c_sen->next; v != NULL; v = v->next) {
            keys.push_back(v->key);
        }
        for (int key : keys) {
            this->remove(key);
        }

        c_sen = top_sentinel;
        while (c_sen->below != NULL) {
            Node *tmp = c_sen;
            c_sen = c_sen->below;
            delete tmp;
        }
        delete c_sen;
    }

    int get_size() { return this->size; }

    void insert(int key, std::string value) {
        int new_height = random_new_level();
        if (new_height > height) {
            update_height(new_height);
        }

        Node *c_sen = top_sentinel;
        Node *above_node = NULL;
        while (c_sen != NULL) {
            above_node = update_level(c_sen, above_node, key, value);
            c_sen = c_sen->below;
        }

        size++;
    }

    std::string get(int k) {
        Node *c_node = top_sentinel;
        while (c_node->key < k) { // exits when next->key <= k
            while (c_node->next != NULL && c_node->next->key > k &&
                   c_node->below != NULL) {
                c_node = c_node->below;
            }

            if (c_node->next != NULL)
                c_node = c_node->next;
            else
                break;
        }

        if (c_node != NULL && c_node->key == k)
            return c_node->value;
        else
            return "NOT FOUND";
    }

    void remove(int k) {
        Node *c_node = top_sentinel;
        while (c_node->key <= k) { // exits when next->key <= k
            while (c_node->next != NULL && c_node->next->key >= k &&
                   c_node->below != NULL) {
                if (c_node->next->key == k) {
                    Node *tmp_next = c_node->next->next;
                    delete c_node->next;
                    c_node->next = tmp_next;
                }
                c_node = c_node->below;
            }

            if (c_node->next->key == k) {
                Node *tmp_next = c_node->next->next;
                delete c_node->next;
                c_node->next = tmp_next;
                break;
            }
        }

        this->size--;
    }

    void print_sl() {
        Node *c_sen = top_sentinel;
        while (c_sen->below != NULL) {
            c_sen = c_sen->below;
        }

        for (Node *c_node = c_sen->next; c_node != NULL;
             c_node = c_node->next) {
            c_node->print_node();
        }
    }
};
