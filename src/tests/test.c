#include <check.h>

#include "../s21_decimal.h"

Suite *suite_s21_comparison(void);
Suite *suite_s21_conversion(void);
Suite *suite_s21_arithmetic(void);
Suite *suite_s21_other(void);

START_TEST(test_from_int_to_decimal_positive) {
  s21_decimal result;
  s21_zero_decimal(&result);
  int src = 12345;
  int status = s21_from_int_to_decimal(src, &result);
  ck_assert_int_eq(status, S21_OK);
  ck_assert_int_eq(result.bits[0], 12345);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
  ck_assert_int_eq(s21_get_sign(result), S21_POSITIVE);
  ck_assert_int_eq(s21_get_scale(result), 0);
}
END_TEST

START_TEST(test_from_int_to_decimal_negative) {
  s21_decimal result;
  s21_zero_decimal(&result);
  int src = -12345;
  int status = s21_from_int_to_decimal(src, &result);
  ck_assert_int_eq(status, S21_OK);
  ck_assert_int_eq(result.bits[0], 12345);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
  ck_assert_int_eq(s21_get_sign(result), S21_NEGATIVE);
  ck_assert_int_eq(s21_get_scale(result), 0);
}
END_TEST

// START_TEST(test_from_float_to_decimal_positive) {
//   s21_decimal result;
//   s21_zero_decimal(&result);
//   float src = 12345;
//   int status = s21_from_float_to_decimal(src, &result);
//   ck_assert_int_eq(status, S21_OK);
//   ck_assert_int_eq(result.bits[0], 12345);
//   ck_assert_int_eq(result.bits[1], 0);
//   ck_assert_int_eq(result.bits[2], 0);
//   ck_assert_int_eq(s21_get_sign(result), S21_POSITIVE);
//   ck_assert_int_eq(s21_get_scale(result), 0);
// }
// END_TEST

// START_TEST(test_from_float_to_decimal_negative) {
//   s21_decimal result;
//   s21_zero_decimal(&result);
//   float src = -12345;
//   int status = s21_from_float_to_decimal(src, &result);
//   ck_assert_int_eq(status, S21_OK);
//   ck_assert_int_eq(result.bits[0], 12345);
//   ck_assert_int_eq(result.bits[1], 0);
//   ck_assert_int_eq(result.bits[2], 0);
//   ck_assert_int_eq(s21_get_sign(result), S21_NEGATIVE);
//   ck_assert_int_eq(s21_get_scale(result), 0);
// }
// END_TEST

// START_TEST(test_from_float_to_decimal_large) {
//   s21_decimal result;
//   s21_zero_decimal(&result);
//   float src = FLT_MAX;
//   int status = s21_from_float_to_decimal(src, &result);
//   ck_assert_int_eq(status, S21_OK);
//   ck_assert_int_eq(result.bits[0], FLT_MAX);
//   ck_assert_int_eq(result.bits[1], 0);
//   ck_assert_int_eq(result.bits[2], 0);
//   ck_assert_int_eq(s21_get_sign(result), S21_NEGATIVE);
//   ck_assert_int_eq(s21_get_scale(result), 0);
// }
// END_TEST

// START_TEST(test_from_float_to_decimal_small) {
//   s21_decimal result;
//   s21_zero_decimal(&result);
//   float src = FLT_MIN;
//   int status = s21_from_float_to_decimal(src, &result);
//   ck_assert_int_eq(status, S21_OK);
//   ck_assert_int_eq(result.bits[0], FLT_MIN);
//   ck_assert_int_eq(result.bits[1], 0);
//   ck_assert_int_eq(result.bits[2], 0);
//   ck_assert_int_eq(s21_get_sign(result), S21_NEGATIVE);
//   ck_assert_int_eq(s21_get_scale(result), 0);
// }
// END_TEST

START_TEST(test_from_decimal_to_int_positive) {
  int result;
  s21_decimal src = {{12345, 0, 0, 0}};
  int status = s21_from_decimal_to_int(src, &result);
  ck_assert_int_eq(status, S21_OK);
  ck_assert_int_eq(result, 12345);
}
END_TEST

START_TEST(test_from_decimal_to_int_negative) {
  int result;
  s21_decimal src = {{12345, 0, 0, 0}};
  s21_set_sign(&src, S21_NEGATIVE);
  int status = s21_from_decimal_to_int(src, &result);
  ck_assert_int_eq(status, S21_OK);
  ck_assert_int_eq(result, -12345);
}
END_TEST

START_TEST(test_from_decimal_to_int_large) {
  int result;
  s21_decimal src = {{INT_MAX, 0, 0, 0}};
  int status = s21_from_decimal_to_int(src, &result);
  ck_assert_int_eq(status, S21_OK);
  ck_assert_int_eq(result, INT_MAX);
}
END_TEST

START_TEST(test_from_decimal_to_int_small) {
  int result;
  s21_decimal src = {{INT_MIN, 0, 0, 0}};
  int status = s21_from_decimal_to_int(src, &result);
  ck_assert_int_eq(status, S21_OK);
  ck_assert_int_eq(result, INT_MIN);
}
END_TEST

START_TEST(test_from_decimal_to_float_positive) {
  float result;
  s21_decimal src = {{12345, 0, 0, 0}};
  int status = s21_from_decimal_to_float(src, &result);
  ck_assert_int_eq(status, S21_OK);
  ck_assert_float_eq(result, 12345.0);
}
END_TEST

// START_TEST(test_from_decimal_to_float_negative) {
//   float result;
//   s21_decimal src = {{12345, 0, 0, 0}};
//   s21_set_sign(&src, S21_NEGATIVE);
//   int status = s21_from_decimal_to_float(src, &result);
//   ck_assert_int_eq(status, S21_OK);
//   ck_assert_float_eq(result, -12345.0);
// }
// END_TEST

START_TEST(test_is_less_positive) {
  s21_decimal num1, num2;
  s21_zero_decimal(&num1);
  s21_zero_decimal(&num2);
  s21_from_int_to_decimal(10, &num1);
  s21_from_int_to_decimal(20, &num2);
  ck_assert_int_eq(s21_is_less(num1, num2), 1);
  ck_assert_int_eq(s21_is_less(num2, num1), 0);
  ck_assert_int_eq(s21_is_less(num1, num1), 0);
}
END_TEST

START_TEST(test_is_less_negative) {
  s21_decimal num1, num2;
  s21_zero_decimal(&num1);
  s21_zero_decimal(&num2);
  s21_from_int_to_decimal(-10, &num1);
  s21_from_int_to_decimal(-20, &num2);
  ck_assert_int_eq(s21_is_less(num1, num2), 0);
  ck_assert_int_eq(s21_is_less(num2, num1), 1);
  ck_assert_int_eq(s21_is_less(num1, num1), 0);
}
END_TEST

START_TEST(test_is_less_mixed) {
  s21_decimal num1, num2;
  s21_zero_decimal(&num1);
  s21_zero_decimal(&num2);
  s21_from_int_to_decimal(-10, &num1);
  s21_from_int_to_decimal(10, &num2);
  ck_assert_int_eq(s21_is_less(num1, num2), 1);
  ck_assert_int_eq(s21_is_less(num2, num1), 0);
}
END_TEST

START_TEST(test_is_greater_positive) {
  s21_decimal num1, num2;
  s21_zero_decimal(&num1);
  s21_zero_decimal(&num2);
  s21_from_int_to_decimal(10, &num1);
  s21_from_int_to_decimal(20, &num2);
  ck_assert_int_eq(s21_is_greater(num1, num2), 0);
  ck_assert_int_eq(s21_is_greater(num2, num1), 1);
  ck_assert_int_eq(s21_is_greater(num1, num1), 0);
}
END_TEST

START_TEST(test_is_greater_negative) {
  s21_decimal num1, num2;
  s21_zero_decimal(&num1);
  s21_zero_decimal(&num2);
  s21_from_int_to_decimal(-10, &num1);
  s21_from_int_to_decimal(-20, &num2);
  ck_assert_int_eq(s21_is_greater(num1, num2), 1);
  ck_assert_int_eq(s21_is_greater(num2, num1), 0);
  ck_assert_int_eq(s21_is_greater(num1, num1), 0);
}
END_TEST

START_TEST(test_is_greater_mixed) {
  s21_decimal num1, num2;
  s21_zero_decimal(&num1);
  s21_zero_decimal(&num2);
  s21_from_int_to_decimal(-10, &num1);
  s21_from_int_to_decimal(10, &num2);
  ck_assert_int_eq(s21_is_greater(num1, num2), 0);
  ck_assert_int_eq(s21_is_greater(num2, num1), 1);
}
END_TEST

START_TEST(test_is_less_or_equal_positive) {
  s21_decimal num1, num2;
  s21_zero_decimal(&num1);
  s21_zero_decimal(&num2);
  s21_from_int_to_decimal(10, &num1);
  s21_from_int_to_decimal(20, &num2);
  ck_assert_int_eq(s21_is_less_or_equal(num1, num2), 1);
  ck_assert_int_eq(s21_is_less_or_equal(num2, num1), 0);
  ck_assert_int_eq(s21_is_less_or_equal(num1, num1), 1);
}
END_TEST

START_TEST(test_is_less_or_equal_negative) {
  s21_decimal num1, num2;
  s21_zero_decimal(&num1);
  s21_zero_decimal(&num2);
  s21_from_int_to_decimal(-10, &num1);
  s21_from_int_to_decimal(-20, &num2);
  ck_assert_int_eq(s21_is_less_or_equal(num1, num2), 0);
  ck_assert_int_eq(s21_is_less_or_equal(num2, num1), 1);
  ck_assert_int_eq(s21_is_less_or_equal(num1, num1), 1);
}
END_TEST

START_TEST(test_is_less_or_equal_mixed) {
  s21_decimal num1, num2;
  s21_zero_decimal(&num1);
  s21_zero_decimal(&num2);
  s21_from_int_to_decimal(-10, &num1);
  s21_from_int_to_decimal(10, &num2);
  ck_assert_int_eq(s21_is_less_or_equal(num1, num2), 1);
  ck_assert_int_eq(s21_is_less_or_equal(num2, num1), 0);
}
END_TEST

START_TEST(test_is_greater_or_equal_positive) {
  s21_decimal num1, num2;
  s21_zero_decimal(&num1);
  s21_zero_decimal(&num2);
  s21_from_int_to_decimal(10, &num1);
  s21_from_int_to_decimal(20, &num2);
  ck_assert_int_eq(s21_is_greater_or_equal(num1, num2), 0);
  ck_assert_int_eq(s21_is_greater_or_equal(num2, num1), 1);
  ck_assert_int_eq(s21_is_greater_or_equal(num1, num1), 1);
}
END_TEST

START_TEST(test_is_greater_or_equal_negative) {
  s21_decimal num1, num2;
  s21_zero_decimal(&num1);
  s21_zero_decimal(&num2);
  s21_from_int_to_decimal(-10, &num1);
  s21_from_int_to_decimal(-20, &num2);
  ck_assert_int_eq(s21_is_greater_or_equal(num1, num2), 1);
  ck_assert_int_eq(s21_is_greater_or_equal(num2, num1), 0);
  ck_assert_int_eq(s21_is_greater_or_equal(num1, num1), 1);
}
END_TEST

START_TEST(test_is_greater_or_equal_mixed) {
  s21_decimal num1, num2;
  s21_zero_decimal(&num1);
  s21_zero_decimal(&num2);
  s21_from_int_to_decimal(-10, &num1);
  s21_from_int_to_decimal(10, &num2);
  ck_assert_int_eq(s21_is_greater_or_equal(num1, num2), 0);
  ck_assert_int_eq(s21_is_greater_or_equal(num2, num1), 1);
}
END_TEST

START_TEST(test_is_equal_positive) {
  s21_decimal num1, num2;
  s21_zero_decimal(&num1);
  s21_zero_decimal(&num2);
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
  s21_zero_decimal(&num1);
  s21_zero_decimal(&num2);
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
  s21_zero_decimal(&num1);
  s21_zero_decimal(&num2);
  s21_from_int_to_decimal(-10, &num1);
  s21_from_int_to_decimal(10, &num2);
  ck_assert_int_eq(s21_is_equal(num1, num2), 0);
  s21_from_int_to_decimal(-10, &num2);
  ck_assert_int_eq(s21_is_equal(num1, num2), 1);
}
END_TEST

START_TEST(test_is_not_equal_positive) {
  s21_decimal num1, num2;
  s21_zero_decimal(&num1);
  s21_zero_decimal(&num2);
  s21_from_int_to_decimal(10, &num1);
  s21_from_int_to_decimal(10, &num2);
  ck_assert_int_eq(s21_is_not_equal(num1, num2), 0);
  s21_from_int_to_decimal(20, &num2);
  ck_assert_int_eq(s21_is_not_equal(num1, num2), 1);
}
END_TEST

START_TEST(test_is_not_equal_negative) {
  s21_decimal num1, num2;
  s21_zero_decimal(&num1);
  s21_zero_decimal(&num2);
  s21_from_int_to_decimal(-10, &num1);
  s21_from_int_to_decimal(-10, &num2);
  ck_assert_int_eq(s21_is_not_equal(num1, num2), 0);
  s21_from_int_to_decimal(-20, &num2);
  ck_assert_int_eq(s21_is_not_equal(num1, num2), 1);
}
END_TEST

START_TEST(test_is_not_equal_mixed) {
  s21_decimal num1, num2;
  s21_zero_decimal(&num1);
  s21_zero_decimal(&num2);
  s21_from_int_to_decimal(-10, &num1);
  s21_from_int_to_decimal(10, &num2);
  ck_assert_int_eq(s21_is_not_equal(num1, num2), 1);
  s21_from_int_to_decimal(-10, &num2);
  ck_assert_int_eq(s21_is_not_equal(num1, num2), 0);
}
END_TEST

START_TEST(test_add_positive) {
  s21_decimal num1 = {{2147483647, 2147483647, 2147483647, 0}};
  s21_decimal num2 = {{1, 0, 0, 0}};
  s21_set_bit(&num1, 95, 1);
  s21_set_bit(&num2, 95, 1);
  s21_decimal result;
  int status = s21_add(num1, num2, &result);
  ck_assert_int_eq(status, S21_OVERFLOW);
}
END_TEST













START_TEST(test_add_negative) {
  s21_decimal num1 = {{2147483647, 2147483647, 2147483647, 0}};
  s21_set_sign(&num1, S21_NEGATIVE);
  s21_decimal num2 = {{1, 0, 0, 0}};
  s21_decimal result;
  s21_zero_decimal(&result);
  int status = s21_add(num1, num2, &result);
  ck_assert_int_eq(status, S21_OK);
  ck_assert_int_eq(s21_get_sign(result), S21_NEGATIVE);
}
END_TEST

START_TEST(test_add_mixed) {
  s21_decimal num1 = {{2147483647, 2147483647, 2147483647, 0}};
  s21_decimal num2 = {{2147483647, 2147483647, 2147483647, 0}};
  s21_set_sign(&num2, S21_NEGATIVE);
  s21_decimal result;
  int status = s21_add(num1, num2, &result);
  ck_assert_int_eq(status, S21_OK);
  ck_assert_int_eq(result.bits[0], 0);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
  ck_assert_int_eq(s21_get_sign(result), S21_POSITIVE);
}
END_TEST

START_TEST(test_add_mixed_2) {
  s21_decimal num1 = {{2147483646, 2147483647, 2147483647, 0}};
  s21_decimal num2 = {{2147483647, 2147483647, 2147483647, 0}};
  s21_set_sign(&num2, S21_NEGATIVE);
  s21_decimal result;
  int status = s21_add(num1, num2, &result);
  ck_assert_int_eq(status, S21_OK);
  ck_assert_int_eq(result.bits[0], 1);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
  ck_assert_int_eq(s21_get_sign(result), S21_NEGATIVE);
}
END_TEST

START_TEST(test_sub_positive) {
  s21_decimal num1 = {{2147483647, 2147483647, 2147483647, 0}};
  s21_decimal num2 = {{1, 0, 0, 0}};
  s21_decimal result;
  int status = s21_sub(num1, num2, &result);
  ck_assert_int_eq(status, S21_OK);
  ck_assert_int_eq(result.bits[0], 2147483646);
  ck_assert_int_eq(result.bits[1], 2147483647);
  ck_assert_int_eq(result.bits[2], 2147483647);
  ck_assert_int_eq(s21_get_sign(result), S21_POSITIVE);
}
END_TEST

START_TEST(test_sub_negative) {
  s21_decimal num1 = {{1, 0, 0, 0}};
  s21_decimal num2 = {{2147483647, 2147483647, 2147483647, 0}};
  s21_decimal result;
  int status = s21_sub(num1, num2, &result);
  ck_assert_int_eq(status, S21_OK);
  ck_assert_int_eq(result.bits[0], 2147483646);
  ck_assert_int_eq(result.bits[1], 2147483647);
  ck_assert_int_eq(result.bits[2], 2147483647);
  ck_assert_int_eq(s21_get_sign(result), S21_NEGATIVE);
}
END_TEST

START_TEST(test_sub_mixed) {
  s21_decimal num1 = {{2147483647, 2147483647, 2147483647, 0}};
  s21_decimal num2 = {{2147483647, 2147483647, 2147483647, 0}};
  s21_set_sign(&num2, S21_POSITIVE);
  s21_decimal result;
  int status = s21_sub(num1, num2, &result);
  // printf("status: %d\n\n", status);
  ck_assert_int_eq(status, S21_OK);
}
END_TEST

START_TEST(test_mul_int_int) {
  s21_decimal num1 = {{45, 0, 0, 0}};
  s21_decimal num2 = {{6, 0, 0, 0}};
  s21_decimal result;
  int status = s21_mul(num1, num2, &result);
  ck_assert_int_eq(status, S21_OK);
  ck_assert_int_eq(result.bits[0], 270);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
  ck_assert_int_eq(s21_get_sign(result), S21_POSITIVE);
}
END_TEST

START_TEST(test_mul_int_float_no_swap) {
  s21_decimal num1 = {{10, 0, 0, 0}};
  s21_decimal num2 = {{25, 0, 0, 0}};
  s21_decimal result;
  int status = s21_mul(num1, num2, &result);
  ck_assert_int_eq(status, S21_OK);
  ck_assert_int_eq(result.bits[0], 250);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
  ck_assert_int_eq(s21_get_sign(result), S21_POSITIVE);
}
END_TEST

START_TEST(test_mul_int_float_with_swap) {
  s21_decimal num1 = {{25, 0, 0, 0}};
  s21_decimal num2 = {{10, 0, 0, 0}};
  s21_decimal result;
  int status = s21_mul(num1, num2, &result);
  ck_assert_int_eq(status, S21_OK);
  ck_assert_int_eq(result.bits[0], 250);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
  ck_assert_int_eq(s21_get_sign(result), S21_POSITIVE);
}
END_TEST

START_TEST(test_mul_float_float) {
  s21_decimal num1 = {{11233, 0, 0, 0}};
  s21_decimal num2 = {{19, 0, 0, 0}};
  s21_decimal result;
  int status = s21_mul(num1, num2, &result);
  ck_assert_int_eq(status, S21_OK);
  ck_assert_int_eq(result.bits[0], 213427);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
  ck_assert_int_eq(s21_get_sign(result), S21_POSITIVE);
}
END_TEST

START_TEST(test_div_by_zero) {
  s21_decimal num1 = {{123, 0, 0, 0}};
  s21_decimal num2 = {{0, 0, 0, 0}};
  s21_decimal result;
  int err = s21_div(num1, num2, &result);
  ck_assert_int_eq(err, S21_DIV_BY_ZERO);
  ck_assert_int_eq(result.bits[0], 0);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
}
END_TEST

START_TEST(test_div_by_one) {
  s21_decimal num1 = {{250, 0, 0, 0}};
  s21_set_scale(&num1, 1);
  s21_decimal num2 = {{1, 0, 0, 0}};
  s21_decimal result;
  s21_zero_decimal(&result);
  int err = s21_div(num1, num2, &result);
  ck_assert_int_eq(err, 0);
  ck_assert_int_eq(result.bits[0], 25);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
}
END_TEST

START_TEST(test_div_quotient_by_quotient) {
  s21_decimal num1 = {{1234, 0, 0, 0}};
  s21_decimal num2 = {{10, 0, 0, 0}};
  s21_decimal result;
  int err = s21_div(num1, num2, &result);
  ck_assert_int_eq(err, 0);
  ck_assert_int_eq(result.bits[0], 1234);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
}
END_TEST

START_TEST(test_div_quotient_by_quotient_2) {
  s21_decimal num1 = {{25, 0, 0, 0}};
  s21_decimal num2 = {{10, 0, 0, 0}};
  s21_decimal result;
  int err = s21_div(num1, num2, &result);
  ck_assert_int_eq(err, 0);
  ck_assert_int_eq(result.bits[0], 25);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
}
END_TEST

START_TEST(test_div_overflow) {
  s21_decimal num1 = {{
    0b11111111111111111111111111111111,
    0b11111111111111111111111111111111,
    0b11111111111111111111111111111111,
    0
  }};
  s21_decimal num2 = {{1, 0, 0, 0}};
  // s21_set_sign(&num2, S21_NEGATIVE);
  s21_set_scale(&num2, 10);

  s21_decimal result;
  int err = s21_div(num1, num2, &result);
  ck_assert_int_eq(err, S21_OVERFLOW);
}
END_TEST

START_TEST(test_truncate_positive) {
  s21_decimal num1 = {{1234, 0, 0, 0}};
  s21_set_scale(&num1, 2);
  s21_decimal result;
  s21_zero_decimal(&result);
  int error = s21_truncate(num1, &result);
  ck_assert_int_eq(error, 0);
  ck_assert_int_eq(result.bits[0], 12);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
}
END_TEST

START_TEST(test_truncate_negative) {
  s21_decimal num1 = {{1234, 0, 0, 0}};
  s21_set_sign(&num1, S21_NEGATIVE);
  s21_set_scale(&num1, 2);
  s21_decimal result;
  s21_zero_decimal(&result);
  int error = s21_truncate(num1, &result);
  ck_assert_int_eq(error, 0);
  ck_assert_int_eq(result.bits[0], 12);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
}
END_TEST

START_TEST(test_s21_negate_1) {
  s21_decimal result, num = {{623548, 524614, 654812458, 0}};
  int status = s21_negate(num, &result);
  ck_assert_int_eq(status, S21_OK);
  ck_assert_int_eq(result.bits[0], 623548);
  ck_assert_int_eq(result.bits[1], 524614);
  ck_assert_int_eq(result.bits[2], 654812458);
  ck_assert_int_eq(s21_get_scale(result), 0);
  ck_assert_int_eq(s21_get_sign(result), S21_NEGATIVE);
}
END_TEST

START_TEST(test_s21_negate_2) {
  s21_decimal result, num = {{623548, 524614, 654812458, 0}};
  s21_set_sign(&num, S21_NEGATIVE);
  int status = s21_negate(num, &result);
  ck_assert_int_eq(status, S21_OK);
  ck_assert_int_eq(result.bits[0], 623548);
  ck_assert_int_eq(result.bits[1], 524614);
  ck_assert_int_eq(result.bits[2], 654812458);
  ck_assert_int_eq(s21_get_scale(result), 0);
  ck_assert_int_eq(s21_get_sign(result), S21_POSITIVE);
}
END_TEST

START_TEST(test_s21_round_zero) {
  s21_decimal result, num = {{0, 0, 0, 0}};
  int status = S21_OK;

  // round(0) = 0;
  status = s21_round(num, &result);
  ck_assert_int_eq(status, S21_OK);
  ck_assert_int_eq(result.bits[0], 0);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
  ck_assert_int_eq(s21_get_scale(result), 0);
  ck_assert_int_eq(s21_get_sign(result), S21_POSITIVE);

  // round(-0) = 0;
  s21_from_int_to_decimal(0, &num);
  s21_set_sign(&num, S21_NEGATIVE);

  status = s21_round(num, &result);
  ck_assert_int_eq(status, S21_OK);
  ck_assert_int_eq(result.bits[0], 0);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
  ck_assert_int_eq(s21_get_scale(result), 0);
  ck_assert_int_eq(s21_get_sign(result), S21_POSITIVE);
}
END_TEST

START_TEST(test_s21_round_pos_small) {
  s21_decimal result, num = {{1, 0, 0, 0}};
  int status = 0;

  // round(0.1) = 0;
  s21_set_scale(&num, 1);

  status = s21_round(num, &result);
  ck_assert_int_eq(status, S21_OK);
  ck_assert_int_eq(result.bits[0], 0);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
  ck_assert_int_eq(s21_get_scale(result), 0);
  ck_assert_int_eq(s21_get_sign(result), S21_POSITIVE);

  // round(0.5) = 1;
  s21_zero_decimal(&num);
  s21_zero_decimal(&result);
  s21_from_int_to_decimal(5, &num);
  s21_set_scale(&num, 1);

  status = s21_round(num, &result);
  ck_assert_int_eq(status, 0);
  ck_assert_int_eq(result.bits[0], 1);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
  ck_assert_int_eq(s21_get_scale(result), 0);
  ck_assert_int_eq(s21_get_sign(result), S21_POSITIVE);
}
END_TEST

START_TEST(test_s21_round_neg_small) {
  s21_decimal result, num;
  int status = 0;

  // round(-0.1) = 0;
  s21_from_int_to_decimal(-1, &num);
  s21_set_scale(&num, 1);

  status = s21_round(num, &result);
  ck_assert_int_eq(status, S21_OK);
  ck_assert_int_eq(result.bits[0], 0);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
  ck_assert_int_eq(s21_get_scale(result), 0);
  ck_assert_int_eq(s21_get_sign(result), S21_POSITIVE);

  // round(-0.5) = -1;
  s21_from_int_to_decimal(-5, &num);
  s21_set_scale(&num, 1);

  status = s21_round(num, &result);
  ck_assert_int_eq(status, S21_OK);
  ck_assert_int_eq(result.bits[0], 1);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
  ck_assert_int_eq(s21_get_scale(result), 0);
  ck_assert_int_eq(s21_get_sign(result), S21_NEGATIVE);
}
END_TEST

START_TEST(test_s21_round_pos_big) {
  s21_decimal result, num;
  int status = 0;

  // round(99.99999) = 100;
  s21_zero_decimal(&num);
  s21_zero_decimal(&result);
  s21_from_int_to_decimal(9999999, &num);
  s21_set_scale(&num, 5);

  status = s21_round(num, &result);
  ck_assert_int_eq(status, S21_OK);
  ck_assert_int_eq(result.bits[0], 100);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
  ck_assert_int_eq(s21_get_scale(result), 0);
  ck_assert_int_eq(s21_get_sign(result), S21_POSITIVE);

  // round(876769869.2687576576) = 876769869;
  // 01111001101011010001111110000001 11111011010011001011111000000000
  s21_decimal num2 = {{
    0b11111011010011001011111000000000,
    0b01111001101011010001111110000001,
    0,
    0
  }};
  s21_set_scale(&num2, 10);

  status = s21_round(num2, &result);
  ck_assert_int_eq(status, S21_OK);
  ck_assert_int_eq(result.bits[0], 876769869);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
  ck_assert_int_eq(s21_get_scale(result), 0);
  ck_assert_int_eq(s21_get_sign(result), S21_POSITIVE);

  // round(7,9228162514264337593543950335) = 8;
  s21_decimal num1 = {{0b11111111111111111111111111111111,
                       0b11111111111111111111111111111111,
                       0b11111111111111111111111111111111, 0}};
  s21_set_scale(&num1, 28);

  status = s21_round(num1, &result);
  ck_assert_int_eq(status, S21_OK);
  ck_assert_int_eq(result.bits[0], 8);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
  ck_assert_int_eq(s21_get_scale(result), 0);
  ck_assert_int_eq(s21_get_sign(result), S21_POSITIVE);
}
END_TEST

START_TEST(test_s21_round_neg_big) {
  s21_decimal result, num;
  int status = 0;

  // round(-99.99999) = -100;
  s21_zero_decimal(&num);
  s21_zero_decimal(&result);
  s21_from_int_to_decimal(-9999999, &num);
  s21_set_scale(&num, 5);

  status = s21_round(num, &result);
  ck_assert_int_eq(status, 0);
  ck_assert_int_eq(result.bits[0], 100);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
  ck_assert_int_eq(s21_get_scale(result), 0);
  ck_assert_int_eq(s21_get_sign(result), S21_NEGATIVE);

  // round(-7,9228162514264337593543950335) = -8;
  s21_decimal num2 = {{0b11111111111111111111111111111111,
                       0b11111111111111111111111111111111,
                       0b11111111111111111111111111111111, 0}};
  s21_set_scale(&num2, 28);
  s21_set_sign(&num2, S21_NEGATIVE);

  status = s21_round(num2, &result);
  ck_assert_int_eq(status, S21_OK);
  ck_assert_int_eq(result.bits[0], 8);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
  ck_assert_int_eq(s21_get_scale(result), 0);
  ck_assert_int_eq(s21_get_sign(result), S21_NEGATIVE);
}
END_TEST

START_TEST(test_s21_floor_zero) {
  s21_decimal result, num = {{0, 0, 0, 0}};
  int status = S21_OK;

  // floor(0) = 0;
  status = s21_floor(num, &result);
  ck_assert_int_eq(status, S21_OK);
  ck_assert_int_eq(result.bits[0], 0);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
  ck_assert_int_eq(s21_get_scale(result), 0);
  ck_assert_int_eq(s21_get_sign(result), S21_POSITIVE);

  // floor(-0) = 0;
  s21_from_int_to_decimal(0, &num);
  s21_set_sign(&num, S21_NEGATIVE);

  status = s21_floor(num, &result);
  ck_assert_int_eq(status, S21_OK);
  ck_assert_int_eq(result.bits[0], 0);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
  ck_assert_int_eq(s21_get_scale(result), 0);
  ck_assert_int_eq(s21_get_sign(result), S21_POSITIVE);
}
END_TEST

START_TEST(test_s21_floor_pos_small) {
  s21_decimal result, num = {{0, 0, 0, 0}};
  int status = 0;

  // floor(0.1) = 0;
  s21_from_int_to_decimal(1, &num);
  s21_set_scale(&num, 1);

  status = s21_floor(num, &result);
  ck_assert_int_eq(status, S21_OK);
  ck_assert_int_eq(result.bits[0], 0);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
  ck_assert_int_eq(s21_get_scale(result), 0);
  ck_assert_int_eq(s21_get_sign(result), S21_POSITIVE);

  // floor(0.5) = 0;
  s21_from_int_to_decimal(5, &num);
  s21_set_scale(&num, 1);

  status = s21_floor(num, &result);
  ck_assert_int_eq(status, 0);
  ck_assert_int_eq(result.bits[0], 0);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
  ck_assert_int_eq(s21_get_scale(result), 0);
  ck_assert_int_eq(s21_get_sign(result), S21_POSITIVE);
}
END_TEST

START_TEST(test_s21_floor_neg_small) {
  s21_decimal result, num = {{0, 0, 0, 0}};
  int status = 0;

  // floor(-0.1) = -1;
  s21_from_int_to_decimal(-1, &num);
  s21_set_scale(&num, 1);

  status = s21_floor(num, &result);
  ck_assert_int_eq(status, 0);
  ck_assert_int_eq(result.bits[0], 1);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
  ck_assert_int_eq(s21_get_scale(result), 0);
  ck_assert_int_eq(s21_get_sign(result), S21_NEGATIVE);

  // floor(-0.5) = -1;
  s21_from_int_to_decimal(-5, &num);
  s21_set_scale(&num, 1);

  status = s21_floor(num, &result);
  ck_assert_int_eq(status, S21_OK);
  ck_assert_int_eq(result.bits[0], 1);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
  ck_assert_int_eq(s21_get_scale(result), 0);
  ck_assert_int_eq(s21_get_sign(result), S21_NEGATIVE);
}
END_TEST

START_TEST(test_s21_floor_pos_big) {
  s21_decimal result, num = {{0, 0, 0, 0}};
  int status = 0;

  // floor(99.99999) = 99;
  s21_from_int_to_decimal(9999999, &num);
  s21_set_scale(&num, 5);

  status = s21_floor(num, &result);
  ck_assert_int_eq(status, S21_OK);
  ck_assert_int_eq(result.bits[0], 99);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
  ck_assert_int_eq(s21_get_scale(result), 0);
  ck_assert_int_eq(s21_get_sign(result), S21_POSITIVE);
}
END_TEST

START_TEST(test_s21_floor_neg_big) {
  s21_decimal result, num = {{0, 0, 0, 0}};
  int status = 0;

  // floor(-99.99999) = -100;
  s21_from_int_to_decimal(-9999999, &num);
  s21_set_scale(&num, 5);

  status = s21_floor(num, &result);
  ck_assert_int_eq(status, S21_OK);
  ck_assert_int_eq(result.bits[0], 100);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
  ck_assert_int_eq(s21_get_scale(result), 0);
  ck_assert_int_eq(s21_get_sign(result), S21_NEGATIVE);

  // floor(-99) = -99;
  s21_from_int_to_decimal(-99, &num);

  status = s21_floor(num, &result);
  ck_assert_int_eq(status, S21_OK);
  ck_assert_int_eq(result.bits[0], 99);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
  ck_assert_int_eq(s21_get_scale(result), 0);
  ck_assert_int_eq(s21_get_sign(result), S21_NEGATIVE);

  // floor(-7,9228162514264337593543950335) = -8;
  s21_decimal num1 = {{0b11111111111111111111111111111111,
                       0b11111111111111111111111111111111,
                       0b11111111111111111111111111111111, 0}};

  s21_set_scale(&num1, 28);
  s21_set_sign(&num1, S21_NEGATIVE);

  status = s21_floor(num1, &result);
  ck_assert_int_eq(status, S21_OK);
  ck_assert_int_eq(result.bits[0], 8);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
  ck_assert_int_eq(s21_get_scale(result), 0);
  ck_assert_int_eq(s21_get_sign(result), S21_NEGATIVE);
}
END_TEST

START_TEST(test_s21_from_int256) {
  // // s21_decimal num1 = {{0b11111111111111111111111111111111,
  // //                      0b11111111111111111111111111111111,
  // //                      0b11111111111111111111111111111111, 0}};

  // s21_int256 num = {{0}};
  // num.bits[0] = 528;
  // // num.bits[1] = 1;
  // s21_set_scale_256(&num, 1);

  // print_int256(num);
  // printf("ok\n\n");

  // printf("%d", num.bits[0]);
  // divide_by_ten_256(&num);
  // printf(" : 10 = %d\n\n", num.bits[0]);

  // print_int256(num);
  // printf("int256\n\n");

}
END_TEST

Suite *suite_s21_conversion(void) {
  Suite *s;
  TCase *tc_core;
  s = suite_create("s21_conversion");
  tc_core = tcase_create("Core");
  tcase_add_test(tc_core, test_from_int_to_decimal_positive);
  tcase_add_test(tc_core, test_from_int_to_decimal_negative);

  // tcase_add_test(tc_core, test_from_float_to_decimal_positive);
  // tcase_add_test(tc_core, test_from_float_to_decimal_negative);
  // tcase_add_test(tc_core, test_from_float_to_decimal_large);
  // tcase_add_test(tc_core, test_from_float_to_decimal_small);

  tcase_add_test(tc_core, test_from_decimal_to_int_positive);
  tcase_add_test(tc_core, test_from_decimal_to_int_negative);
  tcase_add_test(tc_core, test_from_decimal_to_int_large);
  tcase_add_test(tc_core, test_from_decimal_to_int_small);

  tcase_add_test(tc_core, test_from_decimal_to_float_positive);
  // tcase_add_test(tc_core, test_from_decimal_to_float_negative);

  suite_add_tcase(s, tc_core);

  return s;
}

Suite *suite_s21_comparison(void) {
  Suite *s;
  TCase *tc_core;
  s = suite_create("s21_comparison");
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
  tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_add_positive);
  tcase_add_test(tc_core, test_add_negative);
  tcase_add_test(tc_core, test_add_mixed);
  tcase_add_test(tc_core, test_add_mixed_2);
  tcase_add_test(tc_core, test_add_positive);

  tcase_add_test(tc_core, test_sub_mixed);
  tcase_add_test(tc_core, test_sub_negative);
  tcase_add_test(tc_core, test_sub_positive);

  tcase_add_test(tc_core, test_mul_int_int);
  tcase_add_test(tc_core, test_mul_int_float_no_swap);
  tcase_add_test(tc_core, test_mul_int_float_with_swap);
  tcase_add_test(tc_core, test_mul_float_float);

  tcase_add_test(tc_core, test_div_by_zero);
  tcase_add_test(tc_core, test_div_by_one);
  tcase_add_test(tc_core, test_div_quotient_by_quotient);
  tcase_add_test(tc_core, test_div_quotient_by_quotient_2);
  tcase_add_test(tc_core, test_div_overflow);

  suite_add_tcase(s, tc_core);

  return s;
}

Suite *suite_s21_other(void) {
  Suite *s;
  TCase *tc_core;
  s = suite_create("s21_other");
  tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_s21_negate_1);
  tcase_add_test(tc_core, test_s21_negate_2);

  tcase_add_test(tc_core, test_s21_round_zero);
  tcase_add_test(tc_core, test_s21_round_pos_big);
  tcase_add_test(tc_core, test_s21_round_neg_big);
  tcase_add_test(tc_core, test_s21_round_pos_small);
  tcase_add_test(tc_core, test_s21_round_neg_small);

  tcase_add_test(tc_core, test_s21_floor_zero);
  tcase_add_test(tc_core, test_s21_floor_pos_big);
  tcase_add_test(tc_core, test_s21_floor_neg_big);
  tcase_add_test(tc_core, test_s21_floor_pos_small);
  tcase_add_test(tc_core, test_s21_floor_neg_small);

  tcase_add_test(tc_core, test_s21_from_int256);

  tcase_add_test(tc_core, test_truncate_positive);
  tcase_add_test(tc_core, test_truncate_negative);

  suite_add_tcase(s, tc_core);

  return s;
}

int main(void) {
  int number_failed;
  Suite *s_conversion, *s_comparison, *s_arithmetic, *s_other;
  SRunner *sr;
  // Создаем наборы тестов
  s_conversion = suite_s21_conversion();
  s_comparison = suite_s21_comparison();
  s_arithmetic = suite_s21_arithmetic();
  s_other = suite_s21_other();
  // Создаем SRunner и добавляем наборы тестов
  sr = srunner_create(s_conversion);
  srunner_add_suite(sr, s_comparison);
  srunner_add_suite(sr, s_arithmetic);
  srunner_add_suite(sr, s_other);
  // Запускаем тесты
  srunner_run_all(sr, CK_NORMAL);
  number_failed = srunner_ntests_failed(sr);
  srunner_free(sr);
  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
