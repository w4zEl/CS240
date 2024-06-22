#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <utility>

class two_sided_PQ {
    std::vector<int> heaps[2];
    std::vector<int> index_maps[2];
    int size{};

    void heap_swap(int dir, int i, int j) {
        std::swap(index_maps[dir ^ 1][index_maps[dir][i]], index_maps[dir ^ 1][index_maps[dir][j]]);
        std::swap(index_maps[dir][i], index_maps[dir][j]);
        std::swap(heaps[dir][i], heaps[dir][j]);
    }

    void fixup(int dir, int idx, long long flip) {
        for (int par; heaps[dir][par = (idx - 1) / 2] * flip < heaps[dir][idx] * flip; idx = par) heap_swap(dir, idx, par);
    }

    int delete_extremum(int dir, long long flip) {
        int extremum = heaps[dir][0], origIdxInOther = index_maps[dir][0];
        heap_swap(dir ^ 1, origIdxInOther, --size);
        fixup(dir ^ 1, origIdxInOther, -flip);
        heap_swap(dir, 0, size);
        for (int i = 2; i--;) heaps[i].pop_back(), index_maps[i].pop_back();
        for (int curr = 0, child; curr < size / 2; curr = child) {
            if ((child = curr * 2 + 1) + 1 < size && heaps[dir][child + 1] * flip > heaps[dir][child] * flip) ++child;
            if (heaps[dir][curr] * flip >= heaps[dir][child] * flip) break;
            heap_swap(dir, curr, child);
        }
        return extremum;
    }

public:
    // returns the size
    int get_size() const {
        return size;
    }

    // returns the minimum key, does not delete it
    int get_min() const {
        return heaps[1][0];
    }

    // returns the maximum key, does not delete it
    int get_max() const {
        return heaps[0][0];
    }

    // deletes mininum key in both arrays and returns it
    int delete_min() {
        return delete_extremum(1, -1);
    }

    // deletes maximum key in both arrays and returns it
    int delete_max() {
        return delete_extremum(0, 1);
    }

    // inserts a new key in both arrays
    void insert(int key) {
        for (int i = 2; i--;) heaps[i].push_back(key), index_maps[i].push_back(size);
        fixup(0, size, 1);
        fixup(1, size++, -1);
    }

    // for debugging and automatic tests, do not change
    void print() const {
        std::cout << "size: " << size << std::endl;
        std::cout << "max heap: ";
        for (int i = 0; i < size; i++)
            std::cout << heaps[0][i] << ((i < size - 1) ? " " : "");
        std::cout << std::endl;
        std::cout << "min heap: ";
        for (int i = 0; i < size; i++)
            std::cout << heaps[1][i] << ((i < size - 1) ? " " : "");
        std::cout << std::endl;
        std::cout << "indices of max in min: ";
        for (int i = 0; i < size; i++)
            std::cout << index_maps[0][i] << ((i < size - 1) ? " " : "");
        std::cout << std::endl;
        std::cout << "indices of min in max: ";
        for (int i = 0; i < size; i++)
            std::cout << index_maps[1][i] << ((i < size - 1) ? " " : "");
        std::cout << std::endl;
    }
};

// Main method given by the assignment
int main(int argc, char** argv) {
    two_sided_PQ T;

    while (true) {
        std::string line, op1, op2;
        std::getline(std::cin, line);
        if (line.length() == 0)
            break;
        std::stringstream stream(line);
        stream >> op1 >> op2;

        if (op1 == "insert") {
            int val = std::stoi(op2);
            T.insert(val);
            std::cout << "after insert " << val << ": " << T.get_min() << " " << T.get_max() << std::endl;
        }

        if (op1 == "delete" && op2 == "max") {
            T.delete_max();
            std::cout << "after delete max: " << T.get_min() << " " << T.get_max() << std::endl;
        }

        if (op1 == "delete" && op2 == "min") {
            T.delete_min();
            std::cout << "after delete min: " << T.get_min() << " " << T.get_max() << std::endl;
        }

        if (op1 == "print" && op2 == "all") {
            std::cout << "print all:\n";
            T.print();
        }
    };
}
