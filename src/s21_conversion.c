#include "s21_decimal.h"

int s21_from_int_to_decimal(int src, s21_decimal *dst) {
  int error = S21_OK;
  if (!dst) {
    error = S21_OVERFLOW;  
  } else {
    if (src < 0) {
      s21_set_sign(dst, S21_NEGATIVE);
      src = -src;
    }
  }
  dst->bits[0] = src;
  return error;
}

int s21_from_decimal_to_int(s21_decimal src, int *dst) {
  int error = S21_OK;
  if (src.bits[2] != 0 || src.bits[1] != 0) error = S21_OVERFLOW;
  if (dst == NULL) error = S21_OVERFLOW;

  int sign = s21_get_sign(src);

  int int_part = (int)src.bits[0];

  int exponent = s21_get_scale(src);

  for (int i = 0; i < exponent; i++) {
    int_part /= 10;
  }

  if (int_part > INT_MAX || int_part < INT_MIN) error = S21_OVERFLOW;

  *dst = (int)int_part;
  if (sign) *dst = -*dst;

  return error;
}

