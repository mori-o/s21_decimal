#ifndef S21_DECIMAL_H
#define S21_DECIMAL_H

#include <float.h>
#include <limits.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define SIGN_BIT 31
#define EXPONENT_BITS 8
#define EXPONENT_MASK 0xFF
#define MAX_EXPONENT 28
#define MAX_DECIMAL {{\
    0b11111111111111111111111111111111,\
    0b11111111111111111111111111111111,\
    0b11111111111111111111111111111111,\
    0\
  }}

typedef enum s21_error_code {
  S21_OK = 0,
  S21_OVERFLOW = 1,
  S21_UNDERFLOW = 2,
  S21_DIV_BY_ZERO = 3
} s21_error_code;

typedef union {
  float fl;
  unsigned int ui;
} floatbits;

// Для s21_compare
typedef enum s21_compare_result {
  S21_EQ = 0,
  S21_BIGGER = 1,
  S21_LESS = 2
} s21_compare_result;

// Перечисление знаков, где 1 = минус а 0 - +
typedef enum s21_sign { S21_POSITIVE = 0, S21_NEGATIVE = 1 } s21_sign;

typedef struct s21_decimal {
  unsigned int bits[4];
} s21_decimal;

typedef struct s21_int256 {
  unsigned int bits[8];
} s21_int256;

/*
compare.c
*/
int s21_compare(s21_decimal value1, s21_decimal value2);
int s21_compare_256(s21_int256 value1, s21_int256 value2);
int s21_scale_handle_256(s21_int256 value1, s21_int256 value2);
// int s21_sign_handle(s21_decimal value1, s21_decimal value2);
int s21_sign_handle_256(s21_int256 value1, s21_int256 value2);
int s21_value_handle(s21_decimal value1, s21_decimal value2);
int s21_value_handle_256(s21_int256 value1, s21_int256 value2);
int s21_is_less(s21_decimal value1, s21_decimal value2);
int s21_is_less_or_equal(s21_decimal value1, s21_decimal value2);
int s21_is_greater(s21_decimal d1, s21_decimal d2);
int s21_is_greater_256(s21_int256 d1, s21_int256 d2);
int s21_is_greater_or_equal(s21_decimal d1, s21_decimal d2);
int s21_is_greater_or_equal_256(s21_int256 d1, s21_int256 d2);
int s21_is_equal(s21_decimal d1, s21_decimal d2);
int s21_is_not_equal(s21_decimal d1, s21_decimal d2);
int s21_reverse_sign(int code);
int s21_truncate_comparison(s21_decimal value1, s21_decimal value2);

/*
helpers.c
*/
int divide_by_ten(s21_decimal *value);
// void divide_by_ten(s21_decimal *value);
void divide_by_ten_256(s21_int256 *value);
int s21_get_min_decimal(s21_decimal *value);
int s21_get_max_decimal(s21_decimal *value);
int s21_get_max_int(s21_decimal *value, int i);
void s21_set_bit_256(s21_int256 *dst, int index, int bit);
int s21_get_bit_256(s21_int256 dst, int index);
void s21_zero_int256(s21_int256 *dst);
int s21_find_highest_bit_int256(s21_int256 num);
void print_int256(s21_int256 num);
void print_binary(unsigned int num);
void print_decimal(s21_decimal num);
void s21_set_sign(s21_decimal *num, int sign);
void s21_set_scale(s21_decimal *num, int scale);
int s21_get_sign(s21_decimal num);
int s21_get_scale(s21_decimal num);
void s21_set_sign_256(s21_int256 *num, int sign);
void s21_set_scale_256(s21_int256 *num, int scale);
int s21_get_sign_256(s21_int256 num);
int s21_get_scale_256(s21_int256 num);
int s21_is_zero(s21_decimal num);
int s21_is_zero_256(s21_int256 num);
// void multiply_by_ten(s21_decimal *dec);
int multiply_by_ten(s21_decimal *dec);
void multiply_by_ten_256(s21_int256 *dec);
s21_decimal decimal_copy(s21_decimal source);
void s21_normalize_scale(s21_decimal *value1, s21_decimal *value2);
void s21_normalize_scale_256(s21_int256 *value1, s21_int256 *value2);
int s21_get_bit(s21_decimal dst, int index);
void s21_zero_decimal(s21_decimal *dst);
void s21_set_bit(s21_decimal *dst, int index, int bit);
void s21_shift_decimal_left(s21_decimal *dst, int num);
void s21_shift_decimal_right(s21_decimal *dst, int num);
void s21_shift_int256_right(s21_int256 *dst, int num);
void s21_shift_int256_left(s21_int256 *dst, int num);
// int s21_is_greater_int256(s21_int256 value1, s21_int256 value2);
void s21_set_value(s21_decimal *num, unsigned long long value);
void s21_to_int256(s21_decimal value1, s21_int256 *value2);
int s21_from_int256(s21_int256 value, s21_decimal *result);
/*
s21_conversion.c
*/
int s21_from_int_to_decimal(int src, s21_decimal *dst);
int s21_from_float_to_decimal(float src, s21_decimal *dst);
int s21_from_decimal_to_int(s21_decimal src, int *dst);
int s21_from_decimal_to_float(s21_decimal src, float *dst);
/*
arithmetic.c
*/
int s21_check_overflow(s21_int256 value);
int s21_sub(s21_decimal value1, s21_decimal value2, s21_decimal *result);
void s21_sub_256(s21_int256 value1, s21_int256 value2, s21_int256 *result);
int s21_add(s21_decimal value1, s21_decimal value2, s21_decimal *result);
void s21_add_256(s21_int256 value1, s21_int256 value2, s21_int256 *result);
// void s21_capacity_decrease(int *scale, s21_int256 *value);
int s21_mul(s21_decimal value1, s21_decimal value2, s21_decimal *result);
void s21_mul_256(s21_int256 value1, s21_int256 value2, s21_int256 *result);
int s21_div(s21_decimal value1, s21_decimal value2, s21_decimal *result);
int s21_div_256(s21_int256 value1, s21_int256 value2, s21_int256 *result);
int s21_integer_division(s21_decimal *dividend, s21_decimal divisor,
                         s21_decimal *quotient);
int s21_integer_division_256(s21_int256 *dividend, s21_int256 divisor,
                          s21_int256 *quotient);
int s21_fraction_division(s21_decimal dividend, s21_decimal divisor,
                          s21_decimal quotient, s21_decimal *result);
int s21_fraction_division_256(s21_int256 dividend, s21_int256 divisor, s21_int256 quotient, s21_int256 *result);
int s21_decimal_get_not_zero_bit(s21_decimal decimal);
void s21_add_int256_man(s21_int256 value_1, s21_int256 value_2, s21_int256 *result);
void s21_bankers_rounding(s21_int256 *value, int scale);
void s21_sub_int256_man(s21_int256 value_1, s21_int256 value_2, s21_int256 *result);
int s21_mul_int256_man(s21_int256 value_1, s21_int256 value_2, s21_int256 *result);
/*
other.c
*/
int s21_truncate(s21_decimal value, s21_decimal *result);
int s21_negate(s21_decimal value, s21_decimal *result);
int s21_round(s21_decimal value, s21_decimal *result);
int s21_floor(s21_decimal value, s21_decimal *result);

#endif
