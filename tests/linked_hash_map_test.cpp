#include "../LinkedHashMap.hpp"
#include <cassert>
#include <random>

void check(const LinkedHashMap& map, const vector<Data>& model, bool byTime) {
    vector<Data> expected;
    for (int key = 0; key < map.len; ++key) {
        vector<string> bucket;
        for (size_t i = model.size(); i > 0; --i) {
            if (model[i - 1].key == key) {
                bucket.push_back(model[i - 1].value);
                expected.push_back(model[i - 1]);
            }
        }
        assert(map.ask(key) == bucket);
    }
    if (byTime)
        expected = model;
    vector<Data> actual = map.forEach();
    assert(actual.size() == expected.size());
    for (size_t i = 0; i < actual.size(); ++i) {
        assert(actual[i].key == expected[i].key);
        assert(actual[i].value == expected[i].value);
    }
    Node* previous = map.beg;
    for (Node* node = map.beg->timeNext; node != nullptr; node = node->timeNext) {
        assert(node->timePre == previous);
        previous = node;
    }
    assert(previous == map.current);
}

int main() {
    std::mt19937 random(1779);
    for (int mode = 0; mode < 2; ++mode) {
        LinkedHashMap map(8, mode != 0);
        vector<Data> model;
        check(map, model, mode != 0);
        for (int step = 0; step < 10000; ++step) {
            int key = random() % 8;
            string value = std::to_string(random() % 16);
            if (random() % 3 == 0) {
                map.insert(key, value);
                model.push_back(Data{key, value});
            } else {
                map.remove(key, value);
                for (size_t i = model.size(); i > 0; --i) {
                    if (model[i - 1].key == key && model[i - 1].value == value) {
                        model.erase(model.begin() + i - 1);
                        break;
                    }
                }
            }
            check(map, model, mode != 0);
        }
        while (!model.empty()) {
            Data last = model.back();
            map.remove(last.key, last.value);
            model.pop_back();
            check(map, model, mode != 0);
        }
        map.insert(0, "after-empty");
        model.push_back(Data{0, "after-empty"});
        check(map, model, mode != 0);
    }
    LinkedHashMap empty(0, true);
    assert(empty.forEach().empty());
}
