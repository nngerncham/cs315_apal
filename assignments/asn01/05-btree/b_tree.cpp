#include <iostream>
#include <limits>
#include <map>
#include <optional>
#include <stack>
#include <string>
#include <vector>

#define B 18

class Node {
  public:
    class SplitResult {
      public:
        Node *child1;
        Node *child2;
        std::pair<int, int> kv_pair;

        SplitResult(Node *c1, Node *c2, std::pair<int, int> kv) {
            child1 = c1;
            child2 = c2;
            kv_pair = kv;
        }
    };

    std::vector<std::pair<int, int>> kv_pairs;
    std::vector<Node *> children;

    Node() {
        kv_pairs.reserve(B);
        children.reserve(B);
    }

    Node(std::vector<std::pair<int, int>> _kvs, std::vector<Node *> _children) {
        kv_pairs = _kvs;
        children = _children;

        kv_pairs.reserve(B);
        children.reserve(B);
    }

    SplitResult *split_and_promote() {
        std::pair<int, int> mid = kv_pairs[B / 2];

        std::vector<std::pair<int, int>> front_kvs =
            std::vector<std::pair<int, int>>(kv_pairs.begin(),
                                             kv_pairs.begin() + (B / 2));
        std::vector<std::pair<int, int>> back_kvs =
            std::vector<std::pair<int, int>>(kv_pairs.begin() + (B / 2 + 1),
                                             kv_pairs.end());

        std::vector<Node *> front_child;
        std::vector<Node *> back_child;

        if (!children.empty()) {
            front_child =
                std::vector<Node *>(children.begin(), children.begin() + ((children.size() - 1) / 2 + 1));
            back_child =
                std::vector<Node *>(children.begin() + ((children.size() - 1) / 2 + 1), children.end());
        }

        Node *c1 = new Node(front_kvs, front_child);
        Node *c2 = new Node(back_kvs, back_child);

		SplitResult *to_return = new SplitResult(c1, c2, mid);
        return to_return;
    }
};

class BTree {
    Node *root;
    int size;

    static int
    compute_good_insert_index(std::vector<std::pair<int, int>> kv_pairs,
                              int key) {
        int i = 0;
        for (; i < kv_pairs.size() && kv_pairs[i].first < key; i++) {
        }
        return i;
    }

    std::optional<int> get_max_key(std::vector<std::pair<int, int>> kv_pairs) {
        if (kv_pairs.empty())
            return std::optional<int>();
        return kv_pairs[kv_pairs.size() - 1].first;
    }

    std::optional<int> get_min_key(std::vector<std::pair<int, int>> kv_pairs) {
        if (kv_pairs.empty())
            return std::optional<int>();
        return kv_pairs[0].first;
    }

  public:
    BTree() { root = new Node(); }

    void insert(int key, int value) {
        Node *v = root;
        std::stack<Node *> path_to_leaf;

        // get to appropriate leaf
        while (!v->children.empty()) {
            path_to_leaf.push(v);
            if (get_max_key(v->kv_pairs).has_value() &&
                key > get_max_key(v->kv_pairs).value()) {
                int last_idx = v->children.size() - 1;
                v = v->children[last_idx];
            } else {
                int i = compute_good_insert_index(v->kv_pairs, key);
                v = v->children[i];
            }
        }

        // now at leaf node
        int i = compute_good_insert_index(v->kv_pairs, key);
        v->kv_pairs.insert(v->kv_pairs.begin() + i,
                           std::pair<int, int>(key, value));

        // restructure
        for (Node *parent; !path_to_leaf.empty(); path_to_leaf.pop()) {
            parent = path_to_leaf.top();
            if (v->kv_pairs.size() == B) {
                Node::SplitResult *split_result = v->split_and_promote();
                int parent_pos = compute_good_insert_index(
                    parent->kv_pairs, split_result->kv_pair.first);
                parent->kv_pairs.insert(parent->kv_pairs.begin() + parent_pos,
                                        split_result->kv_pair);

                parent->children.erase(parent->children.begin() + parent_pos);
                parent->children.insert(parent->children.begin() + parent_pos,
                                        split_result->child1);
                parent->children.insert(parent->children.begin() + parent_pos +
                                            1,
                                        split_result->child2);

                delete v;
            }

            v = parent;
        }

        // grow root if needed
        if (v->kv_pairs.size() == B) {
            Node::SplitResult *split_result = v->split_and_promote();

            Node *tmp = new Node();
            tmp->kv_pairs.push_back(split_result->kv_pair);
            tmp->children.push_back(split_result->child1);
            tmp->children.push_back(split_result->child2);

			delete root;
            root = tmp;
        }
    }

    std::optional<int> get(int key) {
		if (root->kv_pairs.empty())
            return std::optional<int>(std::nullopt);

        Node *v = root;

        while (!v->children.empty()) {
            if (key < get_min_key(v->kv_pairs).value())
                v = v->children[0];
            else if (key > get_max_key(v->kv_pairs).value())
                v = v->children[v->children.size() - 1];
			else {
				int i = compute_good_insert_index(v->kv_pairs, key);
				if (v->kv_pairs[i].first == key)
					return v->kv_pairs[i].second;
				else 
					v = v->children[i];
			}
        }

        int i = compute_good_insert_index(v->kv_pairs, key);
        if (v->kv_pairs[i].first == key)
            return v->kv_pairs[i].second;
        else
            return std::optional<int>(std::nullopt);
    }
};
