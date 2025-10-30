CC = gcc
FLAGS= -Wall -Wextra -Werror

s21_cat: s21_cat.c
	$(CC) $(FLAGS) s21_cat.c -o s21_cat

s21_grep: s21_grep.c
	$(CC) $(FLAGS) s21_grep.c -o s21_grep

cat_tests: tests/cat_tests.sh
	bash tests/cat_tests.sh

grep_tests: tests/grep_tests.sh
	bash tests/grep_tests.sh

clang:
	clang-format -i *.c
	clang-format -i *.h

clean:
	rm -rf s21_cat
	rm -rf s21_grep