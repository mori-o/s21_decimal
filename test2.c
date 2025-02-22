#include "s21_decimal.h"

int main() {
  s21_decimal num1 = {{2147483647, 2147483647, 2147483647, 0}};
  s21_decimal num2 = {{2147483647, 2147483647, 2147483647, 0}};
  s21_decimal result;

  printf("a:\n");
  print_decimal(num1);

  printf("b:\n");
  print_decimal(num2);

  printf("%d\n", s21_mul(num1, num2, &result));
  print_decimal(result);
}
