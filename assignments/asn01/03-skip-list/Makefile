all:
	g++ -std=c++20 -msse2 skip_list.cpp main.cpp -o sl_test.out
	#./sl_test.out

test:
	g++ -std=c++20 -msse2 skip_list.cpp main.cpp -o sl_test.out
	./sl_test.out test > results.csv

fmt:
	clang-format *.cpp -i

clean:
	rm -rf *.o
