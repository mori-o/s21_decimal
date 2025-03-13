#include "../s21_decimal.h"

int main() {
  s21_decimal value_1 = {{4, 0, 0, 0}};
  s21_set_scale(&value_1, 13);
  s21_set_sign(&value_1, 0);
  s21_decimal value_2;
  int code = s21_get_max_decimal(&value_2);
  s21_set_scale(&value_2, 28);
  s21_set_sign(&value_2, 0);
  s21_decimal result;
  s21_zero_decimal(&result);
  int error = s21_add(value_1, value_2, &result);
  print_decimal(result);
  printf("%d\n", error);
  printf("%d\n", code);
  return 0;
}