CC = gcc
LFLAGS = -lstdc++
LDFLAGS = -lgtest -lgtest_main --coverage 
ifeq ($(shell uname -s), Linux)
	LDFLAGS += -lm #-lsubunit
endif
CFLAGS += -Wall -Wextra -Werror -std=c++17
SOURCES = source_code/model_test.cc source_code/model/model.cc
OBJECTS = *.o

all: tests

tests: clean
	$(CC) $(CFLAGS) $(SOURCES) $(LDFLAGS) -o test $(LFLAGS)
	./test

gcov_report: clean
	$(CC) $(CFLAGS) -c $(SOURCES) --coverage $(LFLAGS)
	$(CC) $(CFLAGS) $(OBJECTS) --coverage $(LDFLAGS) -o test $(LFLAGS)
	./test
	lcov -t "s21_test" -o s21_test.info -c -d .
	genhtml -o s21_test_report s21_test.info
	rm -f *.o *.gcda *.gcno *.gcov *.info test
	open s21_test_report/index.html

gcov_report1: clean
	$(CC) $(CFLAGS) --coverage $(SOURCES) $(LDFLAGS) -o test $(LFLAGS)
	./test
	gcov *.gcda
	gcovr 
	mkdir s21_test_report1
	gcovr --html-details s21_test_report1/index.html
	rm -f *.gcov *.gcda *.gcno test
	open s21_test_report1/index.html

install:
	mkdir build
	cd build && cmake ../source_code/CMakeLists.txt
	pwd
	cd build &&  cmake --build .

uninstall:
	rm -rf build

dist: install
	cp build/SmartCalc_2 SmartCalc_2 
	tar -czf smart_calc_2.tar.gz SmartCalc_2
	rm -rf build
	rm SmartCalc_2

clean:
	rm -f test
	rm -rf build
	rm -f *.out *.a *.o asd.c s21_test *.html *.gcno *.gcda *.gcno *.gcov coverage_report.css 
	rm -f *.info s21_test smart_calc/*.gcda smart_calc/*.gcno smart_calc.tar.gz
	rm -rf report cov-report QT_start build*
	rm -f *.log readme.aux readme.dvi readme.log readme.toc readme.pdf
	rm -rf s21_test_report s21_test_report1

style:
	cp ../materials/linters/.clang-format .clang-format
	clang-format -n source_code/model/*.cc
	clang-format -n source_code/model/*.h
	clang-format -n source_code/controller/*.cc
	clang-format -n source_code/controller/*.h
	clang-format -n source_code/view/*.cc
	clang-format -n source_code/view/*.h
	clang-format -n source_code/*.cc
	clang-format -n source_code/*.h

dvi_md:
	open README.md

leaks_check_GUI_ubuntu:
	valgrind --trace-children=yes --track-fds=yes --track-origins=yes --leak-check=full --show-leak-kinds=all build/SmartCalc_2

leaks_check_unit_test_ubuntu:
	make tests
	valgrind --trace-children=yes --track-fds=yes --track-origins=yes --leak-check=full --show-leak-kinds=all ./test
