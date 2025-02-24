#Стадия test запускает тесты, стадия t привязана к файлу test2.c в котором можно сделать отдельный тестовый случай и проанализировать его 
FLAGS = -Wall -Werror -Wextra
TEST_FLAGS = -lcheck -lm
all:
	find . -type f \( -name "*.c" -o -name "*.h" \) -exec clang-format -i {} +
	gcc $(FLAGS) helpers.c
	./a.out
test:
	find . -type f \( -name "*.c" -o -name "*.h" \) -exec clang-format -i {} +
	gcc $(FLAGS) $(TEST_FLAGS) s21_conversion.c arithmetic.c test.c s21_compare.c   helpers.c 
	./a.out
t:
	find . -type f \( -name "*.c" -o -name "*.h" \) -exec clang-format -i {} +
	gcc $(FLAGS)   s21_conversion.c s21_compare.c helpers.c arithmetic.c test2.c
	./a.out
clean:
	find . -type f \( -name "*.o" -o -name "*.gcda" -o -name "*.gcno" \) -exec rm -f {} +
