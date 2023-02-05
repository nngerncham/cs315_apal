#include "skip_list.cpp"
#include <x86intrin.h>
#include <typeinfo>
#include <map>

//#define N 16
#define N (1 << 12)

using ullong = unsigned long long;

void print_result(int i, std::string src, std::string type, ullong begin, ullong end) {
	std::cout << i << "," << src << "," << type << "," << end - begin << std::endl;
}

int main(int argc, char *argv[]) {
	bool is_test = argc > 1;

    SimpleSkipList *skip_list = new SimpleSkipList();
	std::map<int, std::string> ordered_map;

	if (is_test) std::cout << "i,src,type,latency" << std::endl;

	// insertion
    for (int i = 1; i <= N; i++) {
		ullong begin = __rdtsc();
        skip_list->insert(i, "element " + std::to_string(i));
		ullong end = __rdtsc();
		if (is_test) print_result(i, "skip_list", "insert", begin, end);

		begin = __rdtsc();
        ordered_map.insert(std::pair<int, std::string>(i, "element " + std::to_string(i)));
		end = __rdtsc();
		if (is_test) print_result(i, "ordered_map", "insert", begin, end);
    }

	// search
    for (int i = 1; i <= N + 1; i++) {
		ullong begin = __rdtsc();
        std::string value = skip_list->get(i);
		ullong end = __rdtsc();
		if (is_test) print_result(i, "skip_list", "search", begin, end);

		begin = __rdtsc();
        ordered_map[i];
		end = __rdtsc();
		if (is_test) print_result(i, "ordered_map", "search", begin, end);
    }

	// deletion
    for (int i = 1; i <= N; i++) {
		ullong begin = __rdtsc();
        skip_list->remove(i);
		ullong end = __rdtsc();
		if (is_test) print_result(i, "skip_list", "remove", begin, end);

		begin = __rdtsc();
        ordered_map.erase(i);
		end = __rdtsc();
		if (is_test) print_result(i, "ordered_map", "remove", begin, end);
    }

    delete skip_list;

    return 0;
}
