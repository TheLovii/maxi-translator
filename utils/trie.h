#ifndef MAXI_TRANSLATOR_TRIE_H
#define MAXI_TRANSLATOR_TRIE_H
#include <iostream>
#include <vector>
#include <queue>

class Trie {
public:
    Trie() = default;
    ~Trie() {
        std::queue<Node*> unprocessed;
        unprocessed.push(head_);
        while (!unprocessed.empty()) {
            for (auto &elem : unprocessed.front()->next) {
                if (elem != nullptr) unprocessed.push(elem);
            }
            unprocessed.pop();
        }
    }

    void insert(std::string &word, int type) {
        Node *curr = head_;
        for (const char&s : word) {
            if (curr->next[s] == nullptr) {
                curr->next[s] = new Node;
            }
            curr = curr->next[s];
        }
        curr->type = type;
    }

    void move_to(const char& s) noexcept(false) {
        if (cursor_->next[s] == nullptr) throw std::logic_error("Move to nullptr point it's a very very bad!");
        cursor_ = cursor_->next[s];
    }

    bool check_next_move_to(const char& s) noexcept {
        return cursor_->next[s] != nullptr;
    }


    int get_cursor_type() {
        return cursor_->type;
    }

    void reset_cursor() {
        cursor_ = head_;
    }
private:
    struct Node {
        int type = -1;
        Node* next[257]{nullptr};
    };
    Node *head_ = new Node;
    Node *cursor_ = head_;
};
#endif //MAXI_TRANSLATOR_TRIE_H
