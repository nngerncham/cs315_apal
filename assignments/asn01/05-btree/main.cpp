#include "b_tree.cpp"
#include <iostream>
#include <map>
#include <typeinfo>
#include <x86intrin.h>
#include <numeric>
#include <random>
#include <algorithm>

//#define N 16
#define N (1 << 20)

using ullong = unsigned long long;

void print_result(int i, std::string src, std::string type, ullong begin,
                  ullong end) {
    std::cout << i << "," << src << "," << type << "," << end - begin
              << std::endl;
}

int main(int argc, char *argv[]) {
    bool is_test = argc > 1;

	std::vector<int> keys(N);
	std::iota(keys.begin(), keys.end(), 1);
	std::shuffle(std::begin(keys), std::end(keys), std::default_random_engine());

    BTree *bt = new BTree();
	 std::map<int, int> ordered_map;

    if (is_test)
        std::cout << "i,src,type,latency" << std::endl;

    // insertion
    for (int i = 1; i <= N; i++) {
        ullong begin = __rdtsc();
        bt->insert(keys[i], i);
        ullong end = __rdtsc();
        if (is_test)
            print_result(i, "b_tree", "insert", begin, end);
        else
            std::cout << i << " was inserted\n";

        begin = __rdtsc();
		ordered_map.insert(std::pair<int, int>(i, i));
        end = __rdtsc();
        if (is_test)
            print_result(i, "ordered_map", "insert", begin, end);
    }

    // search
	std::shuffle(std::begin(keys), std::end(keys), std::default_random_engine());
    for (int i = 1; i <= N + 1; i++) {
        ullong begin = __rdtsc();
        std::optional<int> value = bt->get(keys[i]);
        ullong end = __rdtsc();
        if (is_test)
            print_result(i, "b_tree", "search", begin, end);
        else {
            if (value.has_value())
                std::cout << i << ": " << value.value() << std::endl;
            else
                std::cout << i << ": not found" << std::endl;
        }

        begin = __rdtsc();
		ordered_map[i];
        end = __rdtsc();
        if (is_test)
            print_result(i, "ordered_map", "search", begin, end);
    }

    delete bt;

    return 0;
}
