#include "skip_list.cpp"
#include <x86intrin.h>
#include <typeinfo>
#include <map>

// #define N 16
#define N (1 << 12)

using ullong = unsigned long long;

void print_result(int i, std::string src, std::string type, ullong begin, ullong end)
{
	std::cout << i << "," << src << "," << type << "," << end - begin << std::endl;
}

int main(int argc, char *argv[])
{
	bool is_test = argc > 1;

	SimpleSkipList *skip_list = new SimpleSkipList();
	SimpleSkipList *p_list = new SimpleSkipList(2.71828185);
	SimpleSkipList *p3_list = new SimpleSkipList(3);
	SimpleSkipList *p4_list = new SimpleSkipList(4);

	if (is_test)
		std::cout << "i,src,type,latency" << std::endl;

	// insertion
	for (int i = 1; i <= N; i++)
	{
		ullong begin = __rdtsc();
		skip_list->insert(i, "element " + std::to_string(i));
		ullong end = __rdtsc();
		if (is_test)
			print_result(i, "fair_coin", "insert", begin, end);

		begin = __rdtsc();
		p_list->insert(i, "element " + std::to_string(i));
		end = __rdtsc();
		if (is_test)
			print_result(i, "pe", "insert", begin, end);

		begin = __rdtsc();
		p3_list->insert(i, "element " + std::to_string(i));
		end = __rdtsc();
		if (is_test)
			print_result(i, "p3", "insert", begin, end);

		begin = __rdtsc();
		p4_list->insert(i, "element " + std::to_string(i));
		end = __rdtsc();
		if (is_test)
			print_result(i, "p4", "insert", begin, end);
	}

	// search
	for (int i = 1; i <= N + 1; i++)
	{
		ullong begin = __rdtsc();
		std::string value = skip_list->get(i);
		ullong end = __rdtsc();
		if (is_test)
			print_result(i, "fair_coin", "search", begin, end);

		begin = __rdtsc();
		std::string value2 = p_list->get(i);
		end = __rdtsc();
		if (is_test)
			print_result(i, "pe", "search", begin, end);

		begin = __rdtsc();
		std::string value3 = p3_list->get(i);
		end = __rdtsc();
		if (is_test)
			print_result(i, "p3", "search", begin, end);

		begin = __rdtsc();
		std::string value4 = p4_list->get(i);
		end = __rdtsc();
		if (is_test)
			print_result(i, "p4", "search", begin, end);
	}

	// deletion
	for (int i = 1; i <= N; i++)
	{
		ullong begin = __rdtsc();
		skip_list->remove(i);
		ullong end = __rdtsc();
		if (is_test)
			print_result(i, "fair_coin", "remove", begin, end);

		begin = __rdtsc();
		p_list->remove(i);
		end = __rdtsc();
		if (is_test)
			print_result(i, "pe", "remove", begin, end);

		begin = __rdtsc();
		p3_list->remove(i);
		end = __rdtsc();
		if (is_test)
			print_result(i, "p3", "remove", begin, end);

		begin = __rdtsc();
		p4_list->remove(i);
		end = __rdtsc();
		if (is_test)
			print_result(i, "p4", "remove", begin, end);
	}

	delete skip_list;

	return 0;
}
