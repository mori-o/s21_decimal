#include <check.h>

#include "s21_decimal.h"

Suite *suite_s21_comparison(void);
Suite *suite_s21_conversion(void);

START_TEST(test_from_int_to_decimal_positive) {
  s21_decimal result;
  int src = 12345;
  int status = s21_from_int_to_decimal(src, &result);
  ck_assert_int_eq(status, 0);
  ck_assert_int_eq(result.bits[0], 12345);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
  ck_assert_int_eq(s21_get_sign(result), 0);
  ck_assert_int_eq(s21_get_scale(result), 0);
}
END_TEST

START_TEST(test_from_int_to_decimal_negative) {
  s21_decimal result;
  int src = -12345;
  int status = s21_from_int_to_decimal(src, &result);
  ck_assert_int_eq(status, 0);
  ck_assert_int_eq(result.bits[0], 12345);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
  ck_assert_int_eq(s21_get_sign(result), 1);
  ck_assert_int_eq(s21_get_scale(result), 0);
}
END_TEST

// // Тест для s21_from_float_to_decimal
// START_TEST(test_from_float_to_decimal_positive) {
//   s21_decimal result;
//   float src = 123.456;
//   int status = s21_from_float_to_decimal(src, &result);
//   ck_assert_int_eq(status, 0);
//   ck_assert_int_eq(result.bits[0], 123456);
//   ck_assert_int_eq(result.bits[1], 0);
//   ck_assert_int_eq(result.bits[2], 0);
//   ck_assert_int_eq(s21_get_sign(result), 0);
//   ck_assert_int_eq(s21_get_scale(result), 3);
// }
// END_TEST

// START_TEST(test_from_float_to_decimal_negative) {
//   s21_decimal result;
//   float src = -123.456;
//   int status = s21_from_float_to_decimal(src, &result);
//   ck_assert_int_eq(status, 0);
//   ck_assert_int_eq(result.bits[0], 123456);
//   ck_assert_int_eq(result.bits[1], 0);
//   ck_assert_int_eq(result.bits[2], 0);
//   ck_assert_int_eq(s21_get_sign(result), 1);
//   ck_assert_int_eq(s21_get_scale(result), 3);
// }
// END_TEST

// START_TEST(test_from_float_to_decimal_large) {
//   s21_decimal result;
//   float src = 79228162514264337593543950335.0;
//   int status = s21_from_float_to_decimal(src, &result);
//   ck_assert_int_eq(status, 1);  // Ожидаем ошибку
// }
// END_TEST

// START_TEST(test_from_float_to_decimal_small) {
//   s21_decimal result;
//   float src = 1e-29;
//   int status = s21_from_float_to_decimal(src, &result);
//   ck_assert_int_eq(status, 1);  // Ожидаем ошибку
// }
// END_TEST

// Тест для s21_from_decimal_to_int
// START_TEST(test_from_decimal_to_int_positive) {
//   s21_decimal num;
//   num.bits[0] = 12345;
//   num.bits[1] = 0;
//   num.bits[2] = 0;
//   s21_set_sign(&num, 0);
//   s21_set_scale(&num, 0);
//   int result;
//   int status = s21_from_decimal_to_int(num, &result);
//   ck_assert_int_eq(status, 0);
//   ck_assert_int_eq(result, 12345);
// }
// END_TEST

// START_TEST(test_from_decimal_to_int_negative) {
//   s21_decimal num;
//   num.bits[0] = 12345;
//   num.bits[1] = 0;
//   num.bits[2] = 0;
//   s21_set_sign(&num, 1);
//   s21_set_scale(&num, 0);
//   int result;
//   int status = s21_from_decimal_to_int(num, &result);
//   ck_assert_int_eq(status, 0);
//   ck_assert_int_eq(result, -12345);
// }
// END_TEST

// START_TEST(test_from_decimal_to_int_large) {
//   s21_decimal num;
//   num.bits[0] = 0xFFFFFFFF;
//   num.bits[1] = 0xFFFFFFFF;
//   num.bits[2] = 0xFFFFFFFF;
//   s21_set_sign(&num, 0);
//   s21_set_scale(&num, 0);
//   int result;
//   int status = s21_from_decimal_to_int(num, &result);
//   ck_assert_int_eq(status, 1);  // Ожидаем ошибку
// }
// END_TEST

// // Тест для s21_from_decimal_to_float
// START_TEST(test_from_decimal_to_float_positive) {
//   s21_decimal num;
//   num.bits[0] = 123456;
//   num.bits[1] = 0;
//   num.bits[2] = 0;
//   s21_set_sign(&num, 0);
//   s21_set_scale(&num, 3);
//   float result;
//   int status = s21_from_decimal_to_float(num, &result);
//   ck_assert_int_eq(status, 0);
//   ck_assert_float_eq_tol(result, 123.456, 1e-6);
// }
// END_TEST

// START_TEST(test_from_decimal_to_float_negative) {
//   s21_decimal num;
//   num.bits[0] = 123456;
//   num.bits[1] = 0;
//   num.bits[2] = 0;
//   s21_set_sign(&num, 1);
//   s21_set_scale(&num, 3);
//   float result;
//   int status = s21_from_decimal_to_float(num, &result);
//   ck_assert_int_eq(status, 0);
//   ck_assert_float_eq_tol(result, -123.456, 1e-6);
// }
// END_TEST

START_TEST(test_is_less_positive) {
  s21_decimal num1, num2;
  s21_from_int_to_decimal(10, &num1);
  s21_from_int_to_decimal(20, &num2);
  ck_assert_int_eq(s21_is_less(num1, num2), 1);
  ck_assert_int_eq(s21_is_less(num2, num1), 0);
  ck_assert_int_eq(s21_is_less(num1, num1), 0);
}
END_TEST

START_TEST(test_is_less_negative) {
  s21_decimal num1, num2;
  s21_from_int_to_decimal(-10, &num1);
  s21_from_int_to_decimal(-20, &num2);
  ck_assert_int_eq(s21_is_less(num1, num2), 0);
  ck_assert_int_eq(s21_is_less(num2, num1), 1);
  ck_assert_int_eq(s21_is_less(num1, num1), 0);
}
END_TEST

START_TEST(test_is_less_mixed) {
  s21_decimal num1, num2;
  s21_from_int_to_decimal(-10, &num1);
  s21_from_int_to_decimal(10, &num2);
  ck_assert_int_eq(s21_is_less(num1, num2), 1);
  ck_assert_int_eq(s21_is_less(num2, num1), 0);
}
END_TEST

// Тест для s21_is_greater
START_TEST(test_is_greater_positive) {
  s21_decimal num1, num2;
  s21_from_int_to_decimal(10, &num1);
  s21_from_int_to_decimal(20, &num2);
  ck_assert_int_eq(s21_is_greater(num1, num2), 0);
  ck_assert_int_eq(s21_is_greater(num2, num1), 1);
  ck_assert_int_eq(s21_is_greater(num1, num1), 0);
}
END_TEST

START_TEST(test_is_greater_negative) {
  s21_decimal num1, num2;
  s21_from_int_to_decimal(-10, &num1);
  s21_from_int_to_decimal(-20, &num2);
  ck_assert_int_eq(s21_is_greater(num1, num2), 1);
  ck_assert_int_eq(s21_is_greater(num2, num1), 0);
  ck_assert_int_eq(s21_is_greater(num1, num1), 0);
}
END_TEST

START_TEST(test_is_greater_mixed) {
  s21_decimal num1, num2;
  s21_from_int_to_decimal(-10, &num1);
  s21_from_int_to_decimal(10, &num2);
  ck_assert_int_eq(s21_is_greater(num1, num2), 0);
  ck_assert_int_eq(s21_is_greater(num2, num1), 1);
}
END_TEST

// Тест для s21_is_less_or_equal
START_TEST(test_is_less_or_equal_positive) {
  s21_decimal num1, num2;
  s21_from_int_to_decimal(10, &num1);
  s21_from_int_to_decimal(20, &num2);
  ck_assert_int_eq(s21_is_less_or_equal(num1, num2), 1);
  ck_assert_int_eq(s21_is_less_or_equal(num2, num1), 0);
  ck_assert_int_eq(s21_is_less_or_equal(num1, num1), 1);
}
END_TEST

START_TEST(test_is_less_or_equal_negative) {
  s21_decimal num1, num2;
  s21_from_int_to_decimal(-10, &num1);
  s21_from_int_to_decimal(-20, &num2);
  ck_assert_int_eq(s21_is_less_or_equal(num1, num2), 0);
  ck_assert_int_eq(s21_is_less_or_equal(num2, num1), 1);
  ck_assert_int_eq(s21_is_less_or_equal(num1, num1), 1);
}
END_TEST

START_TEST(test_is_less_or_equal_mixed) {
  s21_decimal num1, num2;
  s21_from_int_to_decimal(-10, &num1);
  s21_from_int_to_decimal(10, &num2);
  ck_assert_int_eq(s21_is_less_or_equal(num1, num2), 1);
  ck_assert_int_eq(s21_is_less_or_equal(num2, num1), 0);
}
END_TEST

// Тест для s21_is_greater_or_equal
START_TEST(test_is_greater_or_equal_positive) {
  s21_decimal num1, num2;
  s21_from_int_to_decimal(10, &num1);
  s21_from_int_to_decimal(20, &num2);
  ck_assert_int_eq(s21_is_greater_or_equal(num1, num2), 0);
  ck_assert_int_eq(s21_is_greater_or_equal(num2, num1), 1);
  ck_assert_int_eq(s21_is_greater_or_equal(num1, num1), 1);
}
END_TEST

START_TEST(test_is_greater_or_equal_negative) {
  s21_decimal num1, num2;
  s21_from_int_to_decimal(-10, &num1);
  s21_from_int_to_decimal(-20, &num2);

  ck_assert_int_eq(s21_is_greater_or_equal(num1, num2), 1);
  ck_assert_int_eq(s21_is_greater_or_equal(num2, num1), 0);
  ck_assert_int_eq(s21_is_greater_or_equal(num1, num1), 1);
}
END_TEST

START_TEST(test_is_greater_or_equal_mixed) {
  s21_decimal num1, num2;
  s21_from_int_to_decimal(-10, &num1);
  s21_from_int_to_decimal(10, &num2);
  ck_assert_int_eq(s21_is_greater_or_equal(num1, num2), 0);
  ck_assert_int_eq(s21_is_greater_or_equal(num2, num1), 1);
}
END_TEST

// Тест для s21_is_equal
START_TEST(test_is_equal_positive) {
  s21_decimal num1, num2;
  s21_from_int_to_decimal(10, &num1);
  s21_from_int_to_decimal(10, &num2);
  ck_assert_int_eq(s21_is_equal(num1, num2), 1);
  ck_assert_int_eq(s21_is_equal(num1, num2), 1);
  s21_from_int_to_decimal(20, &num2);
  ck_assert_int_eq(s21_is_equal(num1, num2), 0);
}
END_TEST

START_TEST(test_is_equal_negative) {
  s21_decimal num1, num2;
  s21_from_int_to_decimal(-10, &num1);
  s21_from_int_to_decimal(-10, &num2);
  ck_assert_int_eq(s21_is_equal(num1, num2), 1);
  ck_assert_int_eq(s21_is_equal(num1, num2), 1);
  s21_from_int_to_decimal(-20, &num2);
  ck_assert_int_eq(s21_is_equal(num1, num2), 0);
}
END_TEST

START_TEST(test_is_equal_mixed) {
  s21_decimal num1, num2;
  s21_from_int_to_decimal(-10, &num1);
  s21_from_int_to_decimal(10, &num2);
  ck_assert_int_eq(s21_is_equal(num1, num2), 0);
  s21_from_int_to_decimal(-10, &num2);
  ck_assert_int_eq(s21_is_equal(num1, num2), 1);
}
END_TEST

// Тест для s21_is_not_equal
START_TEST(test_is_not_equal_positive) {
  s21_decimal num1, num2;
  s21_from_int_to_decimal(10, &num1);
  s21_from_int_to_decimal(10, &num2);
  ck_assert_int_eq(s21_is_not_equal(num1, num2), 0);
  s21_from_int_to_decimal(20, &num2);
  ck_assert_int_eq(s21_is_not_equal(num1, num2), 1);
}
END_TEST

START_TEST(test_is_not_equal_negative) {
  s21_decimal num1, num2;
  s21_from_int_to_decimal(-10, &num1);
  s21_from_int_to_decimal(-10, &num2);
  ck_assert_int_eq(s21_is_not_equal(num1, num2), 0);
  s21_from_int_to_decimal(-20, &num2);
  ck_assert_int_eq(s21_is_not_equal(num1, num2), 1);
}
END_TEST

START_TEST(test_is_not_equal_mixed) {
  s21_decimal num1, num2;
  s21_from_int_to_decimal(-10, &num1);
  s21_from_int_to_decimal(10, &num2);
  ck_assert_int_eq(s21_is_not_equal(num1, num2), 1);
  s21_from_int_to_decimal(-10, &num2);
  ck_assert_int_eq(s21_is_not_equal(num1, num2), 0);
}
END_TEST

// START_TEST(test_add_positive) {
//   s21_decimal num1 = {{2147483647, 2147483647, 2147483647, 0}};
//   s21_decimal num2 = {{1, 0, 0, 0}};
//   s21_set_bit(&num1, 95, 1);
//   s21_set_bit(&num2, 95, 1);
//   s21_decimal result;
//   int status = s21_add(num1, num2, &result);
//   ck_assert_int_eq(status, 1);  // Ожидаем ошибку переполнения
// }
// END_TEST

START_TEST(test_add_negative) {
  s21_decimal num1 = {{2147483647, 2147483647, 2147483647, 0}};
  s21_set_sign(&num1, S21_NEGATIVE);
  s21_decimal num2 = {{1, 0, 0, 0}};
  s21_decimal result;
  s21_zero_decimal(&result);
  int status = s21_add(num1, num2, &result);
  ck_assert_int_eq(status, 0);  // Ожидаем успешное выполнение
  ck_assert_int_eq(s21_get_sign(result), S21_NEGATIVE);
}
END_TEST

START_TEST(test_add_mixed) {
  s21_decimal num1 = {{2147483647, 2147483647, 2147483647, 0}};
  s21_decimal num2 = {{2147483647, 2147483647, 2147483647, 0}};
  s21_set_sign(&num2, S21_NEGATIVE);
  s21_decimal result;
  int status = s21_add(num1, num2, &result);
  ck_assert_int_eq(status, 0);  // Ожидаем успешное выполнение
  ck_assert_int_eq(result.bits[0], 0);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
  ck_assert_int_eq(s21_get_sign(result), S21_POSITIVE);
}
END_TEST

// Тест для s21_sub
// START_TEST(test_sub_positive) {
//   s21_decimal num1 = {{2147483647, 2147483647, 2147483647, 0}};
//   s21_decimal num2 = {{1, 0, 0, 0}};
//   s21_decimal result;
//   int status = s21_sub(num1, num2, &result);
//   ck_assert_int_eq(status, 0);  // Ожидаем успешное выполнение
//   ck_assert_int_eq(result.bits[0], 2147483646);
//   ck_assert_int_eq(result.bits[1], 2147483647);
//   ck_assert_int_eq(result.bits[2], 2147483647);
//   ck_assert_int_eq(s21_get_sign(result), S21_POSITIVE);
// }
// END_TEST

// START_TEST(test_sub_negative) {
//   s21_decimal num1 = {{1, 0, 0, 0}};
//   s21_decimal num2 = {{2147483647, 2147483647, 2147483647, 0}};
//   s21_decimal result;
//   int status = s21_sub(num1, num2, &result);
//   ck_assert_int_eq(status, 0);  // Ожидаем успешное выполнение
//   ck_assert_int_eq(result.bits[0], 2147483646);
//   ck_assert_int_eq(result.bits[1], 2147483647);
//   ck_assert_int_eq(result.bits[2], 2147483647);
//   ck_assert_int_eq(s21_get_sign(result), S21_NEGATIVE);
// }
// END_TEST

START_TEST(test_sub_mixed) {
  s21_decimal num1 = {{2147483647, 2147483647, 2147483647, 0}};
  s21_decimal num2 = {{2147483647, 2147483647, 2147483647, 0}};
  s21_set_sign(&num2, S21_NEGATIVE);
  s21_decimal result;
  int status = s21_sub(num1, num2, &result);
  ck_assert_int_eq(status, 0);
}
END_TEST

// Тест для s21_mul
START_TEST(test_mul_int_int) {
  printf("\n\ntest1:\n\n");
  s21_decimal num1 = {{45, 0, 0, 0}};
  s21_decimal num2 = {{6, 0, 0, 0}};
  s21_decimal result;

  printf("a:\n");
  print_decimal(num1);

  printf("b:\n");
  print_decimal(num2);

  int status = s21_mul(num1, num2, &result);
  ck_assert_int_eq(status, 0);  // Ожидаем успешное выполнение

  printf("res:\n");
  print_decimal(result);

  ck_assert_int_eq(result.bits[0], 270);  // 45 * 6 = 270
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
  ck_assert_int_eq(s21_get_sign(result), S21_POSITIVE);
}
END_TEST

START_TEST(test_mul_int_float_no_swap) {
  printf("\n\ntest2:\n\n");
  s21_decimal num1 = {{10, 0, 0, 0}};
  s21_decimal num2 = {{25, 0, 0, 0x00010000}};
  s21_decimal result;

  printf("a:\n");
  print_decimal(num1);

  printf("b:\n");
  print_decimal(num2);

  int status = s21_mul(num1, num2, &result);
  ck_assert_int_eq(status, 0);  // Ожидаем успешное выполнение

  printf("res:\n");
  print_decimal(result);

  ck_assert_int_eq(result.bits[0], 25);  // 10 * 2,5 = 25
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
  ck_assert_int_eq(s21_get_sign(result), S21_POSITIVE);
}
END_TEST

START_TEST(test_mul_int_float_with_swap) {
  printf("\n\ntest3:\n\n");
  s21_decimal num1 = {{25, 0, 0, 0x00010000}};
  s21_decimal num2 = {{10, 0, 0, 0}};
  s21_decimal result;

  printf("a:\n");
  print_decimal(num1);

  printf("b:\n");
  print_decimal(num2);

  int status = s21_mul(num1, num2, &result);
  ck_assert_int_eq(status, 0);  // Ожидаем успешное выполнение

  printf("res:\n");
  print_decimal(result);

  ck_assert_int_eq(result.bits[0], 25);  // 2,5 * 10 = 25
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
  ck_assert_int_eq(s21_get_sign(result), S21_POSITIVE);
}
END_TEST

START_TEST(test_mul_float_float) {
  printf("\n\ntest4:\n\n");
  s21_decimal num1 = {{11233, 0, 0, 0x00030000}};
  s21_decimal num2 = {{19, 0, 0, 0x00010000}};

  s21_decimal result;

  printf("a:\n");
  print_decimal(num1);

  printf("b\n");
  print_decimal(num2);

  int status = s21_mul(num1, num2, &result);
  ck_assert_int_eq(status, 0);  // Ожидаем успешное выполнение

  printf("res:\n");
  print_decimal(result);

  ck_assert_int_eq(result.bits[0], 213427);  // 11,233 * 1,9 = 21,3427
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
  ck_assert_int_eq(s21_get_sign(result), S21_POSITIVE);
}
END_TEST

Suite *suite_s21_conversion(void) {
  Suite *s;
  TCase *tc_core;

  s = suite_create("s21_conversion");

  /* Core test case */
  tc_core = tcase_create("Core");

  // Тесты для s21_from_int_to_decimal
  tcase_add_test(tc_core, test_from_int_to_decimal_positive);
  tcase_add_test(tc_core, test_from_int_to_decimal_negative);

  // // Тесты для s21_from_float_to_decimal
  // tcase_add_test(tc_core, test_from_float_to_decimal_positive);
  // tcase_add_test(tc_core, test_from_float_to_decimal_negative);
  // tcase_add_test(tc_core, test_from_float_to_decimal_large);
  // tcase_add_test(tc_core, test_from_float_to_decimal_small);

  // // Тесты для s21_from_decimal_to_int
  // tcase_add_test(tc_core, test_from_decimal_to_int_positive);
  // tcase_add_test(tc_core, test_from_decimal_to_int_negative);
  // tcase_add_test(tc_core, test_from_decimal_to_int_large);

  // // Тесты для s21_from_decimal_to_float
  // tcase_add_test(tc_core, test_from_decimal_to_float_positive);
  // tcase_add_test(tc_core, test_from_decimal_to_float_negative);

  suite_add_tcase(s, tc_core);

  return s;
}
Suite *suite_s21_comparison(void) {
  Suite *s;
  TCase *tc_core;

  s = suite_create("s21_comparison");

  /* Core test case */
  tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_is_less_positive);
  tcase_add_test(tc_core, test_is_less_negative);
  tcase_add_test(tc_core, test_is_less_mixed);
  tcase_add_test(tc_core, test_is_greater_positive);
  tcase_add_test(tc_core, test_is_greater_negative);
  tcase_add_test(tc_core, test_is_greater_mixed);
  tcase_add_test(tc_core, test_is_less_or_equal_positive);
  tcase_add_test(tc_core, test_is_less_or_equal_negative);
  tcase_add_test(tc_core, test_is_less_or_equal_mixed);
  tcase_add_test(tc_core, test_is_greater_or_equal_positive);
  tcase_add_test(tc_core, test_is_greater_or_equal_negative);
  tcase_add_test(tc_core, test_is_greater_or_equal_mixed);
  tcase_add_test(tc_core, test_is_equal_positive);
  tcase_add_test(tc_core, test_is_equal_negative);
  tcase_add_test(tc_core, test_is_equal_mixed);
  tcase_add_test(tc_core, test_is_not_equal_positive);
  tcase_add_test(tc_core, test_is_not_equal_negative);
  tcase_add_test(tc_core, test_is_not_equal_mixed);

  suite_add_tcase(s, tc_core);

  return s;
}
Suite *suite_s21_arithmetic(void) {
  Suite *s;
  TCase *tc_core;

  s = suite_create("s21_arithmetic");

  /* Core test case */
  tc_core = tcase_create("Core");

  // Тесты для s21_add
  // tcase_add_test(tc_core, test_add_positive);
  tcase_add_test(tc_core, test_add_negative);
  tcase_add_test(tc_core, test_add_mixed);

  // Тесты для s21_sub
  // tcase_add_test(tc_core, test_sub_positive);
  // tcase_add_test(tc_core, test_sub_negative);
  tcase_add_test(tc_core, test_sub_mixed);

  // Тесты для s21_mul
  tcase_add_test(tc_core, test_mul_int_int);
  tcase_add_test(tc_core, test_mul_int_float_no_swap);
  tcase_add_test(tc_core, test_mul_int_float_with_swap);
  tcase_add_test(tc_core, test_mul_float_float);

  suite_add_tcase(s, tc_core);

  return s;
}

int main(void) {
  int number_failed;
  Suite *s_conversion, *s_comparison, *s_arithmetic;
  SRunner *sr;

  // Создаем наборы тестов
  s_conversion = suite_s21_conversion();
  s_comparison = suite_s21_comparison();
  s_arithmetic = suite_s21_arithmetic();
  // Создаем SRunner и добавляем наборы тестов
  sr = srunner_create(s_conversion);
  srunner_add_suite(sr, s_comparison);
  srunner_add_suite(sr, s_arithmetic);

  // Запускаем тесты
  srunner_run_all(sr, CK_NORMAL);
  number_failed = srunner_ntests_failed(sr);
  srunner_free(sr);

  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}