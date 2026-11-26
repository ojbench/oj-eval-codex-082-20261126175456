#ifndef FINAL_SRC_HPP
#define FINAL_SRC_HPP

#include <string>
#include <vector>
using namespace std;

class LinkedHashMap;
struct Node {
    Node* next = nullptr;
    Node* timePre = nullptr;
    Node* timeNext = nullptr;
    int key = 0;
    string value;
};

struct Data {
    int key;
    string value;
};

vector<Data> traverseByTime(const LinkedHashMap& obj);
vector<Data> traverseByIndex(const LinkedHashMap& obj);

class LinkedHashMap {
public:
    int len;
    Node** array;
    Node* beg;
    Node* current;
    vector<Data> (*forEachMethod)(const LinkedHashMap&);

    LinkedHashMap(int _len_, bool forEachByTime) {
        len = _len_;
        beg = new Node();
        current = beg;
        array = new Node*[len];
        for (int i = 0; i < len; i++)
            array[i] = nullptr;
        if (forEachByTime)
            forEachMethod = traverseByTime;
        else
            forEachMethod = traverseByIndex;
    }

    ~LinkedHashMap() {
        while (beg != nullptr) {
            Node* next = beg->timeNext;
            delete beg;
            beg = next;
        }
        delete[] array;
    }

    void insert(int key, string value) {
        Node* node = new Node();
        node->key = key;
        node->value.swap(value);
        node->next = array[key];
        array[key] = node;
        node->timePre = current;
        current->timeNext = node;
        current = node;
    }

    void remove(int key, string value) {
        // The incoming bucket link handles head and interior removal alike.
        Node** link = &array[key];
        while (*link != nullptr && (*link)->value != value)
            link = &(*link)->next;
        if (*link == nullptr)
            return;
        Node* node = *link;
        *link = node->next;
        node->timePre->timeNext = node->timeNext;
        if (node->timeNext != nullptr)
            node->timeNext->timePre = node->timePre;
        else
            current = node->timePre;
        delete node;
    }

    vector<string> ask(int key) const {
        vector<string> result;
        for (Node* node = array[key]; node != nullptr; node = node->next)
            result.push_back(node->value);
        return result;
    }

    vector<Data> forEach() const {
        return forEachMethod(*this);
    }
};

vector<Data> traverseByTime(const LinkedHashMap& obj) {
    vector<Data> result;
    for (Node* node = obj.beg->timeNext; node != nullptr; node = node->timeNext)
        result.push_back(Data{node->key, node->value});
    return result;
}

vector<Data> traverseByIndex(const LinkedHashMap& obj) {
    vector<Data> result;
    for (int key = 0; key < obj.len; ++key)
        for (Node* node = obj.array[key]; node != nullptr; node = node->next)
            result.push_back(Data{node->key, node->value});
    return result;
}

#endif // FINAL_SRC_HPP
