#include "../s21_decimal.h"

// int s21_add(s21_decimal value1, s21_decimal value2, s21_decimal *result) {
//   int error = 0, sign_res = 0;
//   s21_zero_decimal(result);
//   int sign1 = s21_get_sign(value1), sign2 = s21_get_sign(value2);

//   if (sign1 == S21_NEGATIVE && sign2 == S21_NEGATIVE) {
//     sign_res = S21_NEGATIVE;
//     s21_set_sign(&value1, S21_POSITIVE);
//     s21_set_sign(&value2, S21_POSITIVE);
//   }

//   if (sign1 != sign2) {
//     s21_set_sign(&value1, S21_POSITIVE);
//     s21_set_sign(&value2, S21_POSITIVE);

//     if (s21_is_greater(value2, value1)) {
//       s21_sub(value2, value1, result);
//       if (sign2 == S21_NEGATIVE) {
//         s21_set_sign(result, S21_NEGATIVE);
//       }
//     } else {
//       s21_sub(value1, value2, result);
//       if (sign1 == S21_NEGATIVE) {
//         s21_set_sign(result, S21_NEGATIVE);
//       }
//     }
//   } else {
//     s21_int256 v1 = {0}, v2 = {0}, r = {0};
//     s21_normalize_scale(&value1, &value2);
//     int scale = s21_get_scale(value1);
//     s21_to_int256(value1, &v1);
//     s21_to_int256(value2, &v2);
//     s21_add_int256_man(v1, v2, &r);

//     s21_capacity_decrease(&scale, &r);

//     // print_int256(r);
//     s21_from_int256(r, result);
//     if (s21_check_overflow(r)) {
//       error = 1;
//     }
//     s21_set_scale(result, scale);
//   }
//   if (sign_res == S21_NEGATIVE) {
//     s21_set_sign(result, S21_NEGATIVE);
//   }
//   if (error == 1 && s21_get_sign(*result)) error = 2;

//   return error;
// }

int s21_add(s21_decimal value1, s21_decimal value2, s21_decimal *result) {
  s21_int256 val1_256, val2_256, res_256;
  s21_to_int256(value1, &val1_256);
  s21_to_int256(value2, &val2_256);

  s21_add_256(val1_256, val2_256, &res_256);

  return s21_from_int256(res_256, result);
}

void s21_add_256(s21_int256 value1, s21_int256 value2, s21_int256 *result) {
  int sign_res = 0;
  s21_zero_int256(result);
  int sign1 = s21_get_sign_256(value1), sign2 = s21_get_sign_256(value2);

  s21_set_sign_256(&value1, S21_POSITIVE);
  s21_set_sign_256(&value2, S21_POSITIVE);

  if (sign1 == S21_NEGATIVE && sign2 == S21_NEGATIVE) {
    sign_res = S21_NEGATIVE;
  }

  if (sign1 != sign2) {
    if (s21_is_greater_256(value2, value1)) {
      s21_sub_256(value2, value1, result);
      if (sign2 == S21_NEGATIVE) {
        s21_set_sign_256(result, S21_NEGATIVE);
      }
    } else {
      s21_sub_256(value1, value2, result);
      if (sign1 == S21_NEGATIVE) {
        s21_set_sign_256(result, S21_NEGATIVE);
      }
    }
  } else {
    s21_normalize_scale_256(&value1, &value2);
    int scale = s21_get_scale_256(value1);

    s21_add_int256_man(value1, value2, result);

    s21_set_scale_256(result, scale);
  }

  if (sign_res == S21_NEGATIVE) {
    s21_set_sign_256(result, S21_NEGATIVE);
  }
}

// сложение мантисс
void s21_add_int256_man(s21_int256 value_1, s21_int256 value_2,
                    s21_int256 *result) {
  int res = 0, ovf = 0;
  for (int i = 0; i < 32 * 7; i++) {
    res = s21_get_bit_256(value_1, i) + s21_get_bit_256(value_2, i) + ovf;
    ovf = res / 2;
    s21_set_bit_256(result, i, res % 2);
  }
}

// void s21_capacity_decrease(int *scale, s21_int256 *value) {
//   int temp = *scale;
//   for (int i = 0; i < temp; i++) {
//     if (s21_check_overflow(*value) == S21_OVERFLOW) {
//       s21_shift_int256_right(value, 1);
//       (*scale)--;
//     }
//   }
// }

int s21_check_overflow(s21_int256 value) {
  int error = S21_OK;
  for (int i = 3; i < 8 && error == S21_OK; i++) {
    if (value.bits[i] != 0) {
      error = S21_OVERFLOW;
    }
  }
  return error;
}

// void s21_from_int256(s21_int256 value, s21_decimal *result) {
//   result->bits[0] = value.bits[0];
//   result->bits[1] = value.bits[1];
//   result->bits[2] = value.bits[2];
//   result->bits[3] = 0;
// }

// int s21_sub(s21_decimal value1, s21_decimal value2, s21_decimal *result) {
//   int error = 0, sign_res = 0;
//   s21_zero_decimal(result);
//   int sign1 = s21_get_sign(value1), sign2 = s21_get_sign(value2);
//   if (sign1 == S21_NEGATIVE && sign2 == S21_NEGATIVE) {
//     sign_res = S21_NEGATIVE;
//     s21_set_sign(&value1, S21_POSITIVE);
//     s21_set_sign(&value2, S21_POSITIVE);
//     s21_decimal temp = value1;
//     value1 = value2;
//     value2 = temp;
//   }

//   if (sign1 != sign2) {
//     int sign = sign1;
//     s21_set_sign(&value1, S21_POSITIVE);
//     s21_set_sign(&value2, S21_POSITIVE);
//     error = sign ? s21_add(value2, value1, result)
//                  : s21_add(value1, value2, result);
//     if (sign) s21_set_sign(result, S21_NEGATIVE);
//   } else {
//     s21_int256 v1 = {0}, v2 = {0}, r = {0};
//     s21_normalize_scale(&value1, &value2);
//     s21_to_int256(value1, &v1);
//     s21_to_int256(value2, &v2);
//     if (s21_is_greater(value1, value2) == 0) {
//       sign_res = S21_NEGATIVE;
//       s21_int256 tmp = v1;
//       v1 = v2;
//       v2 = tmp;
//     }
//     s21_sub_int256_man(v1, v2, &r);
//     s21_from_int256(r, result);
//     s21_set_scale(result, s21_get_scale(value1));
//   }

//   if (sign_res == S21_NEGATIVE) {
//     s21_set_sign(result, S21_NEGATIVE);
//   }
//   if (s21_is_zero(*result)) {
//     s21_set_sign(result, S21_POSITIVE);
//   }
//   if (error == 1 && s21_get_sign(*result)) error = 2;

//   return error;
// }

int s21_sub(s21_decimal value1, s21_decimal value2, s21_decimal *result) {
  s21_int256 val1_256, val2_256, res_256;
  s21_to_int256(value1, &val1_256);
  s21_to_int256(value2, &val2_256);

  s21_sub_256(val1_256, val2_256, &res_256);

  return s21_from_int256(res_256, result);
}

void s21_sub_256(s21_int256 value1, s21_int256 value2, s21_int256 *result) {
  int sign = S21_POSITIVE;
  int sign1 = s21_get_sign_256(value1);
  int sign2 = s21_get_sign_256(value2);

  s21_zero_int256(result);

  s21_set_sign_256(&value1, S21_POSITIVE);
  s21_set_sign_256(&value2, S21_POSITIVE);

  if (sign1 == sign2) {
    s21_normalize_scale_256(&value1, &value2);
    if (s21_is_greater_256(value1, value2) == 1) {
      sign = sign1;
      s21_sub_int256_man(value1, value2, result);
    } else {
      sign = 1 - sign2;
      s21_sub_int256_man(value2, value1, result);
    }
    s21_set_scale_256(result, s21_get_scale_256(value1));
  } else {
    sign = sign1;
    s21_add_256(value2, value1, result);
  }

  s21_set_sign_256(result, sign);
  if (s21_is_zero_256(*result)) {
    s21_set_sign_256(result, S21_POSITIVE);
  }
}

// вычитание мантисс
void s21_sub_int256_man(s21_int256 value_1, s21_int256 value_2,
                    s21_int256 *result) {
  int tmp = 0, res = 0;
  for (int i = 0; i < 32 * 7; i++) {
    res = s21_get_bit_256(value_1, i) - s21_get_bit_256(value_2, i) - tmp;
    tmp = res < 0;
    res = abs(res);
    s21_set_bit_256(result, i, res % 2);
  }
}

// int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
//   int error = 0, sign_flag = 0;
//   s21_zero_decimal(result);
//   int sign1 = s21_get_sign(value_1), sign2 = s21_get_sign(value_2);

//   if ((sign1 == S21_POSITIVE && sign2 == S21_POSITIVE) ||
//       (sign1 == S21_NEGATIVE && sign2 == S21_NEGATIVE)) {
//     sign_flag = S21_POSITIVE;
//   } else if (sign1 != sign2) {
//     sign_flag = S21_NEGATIVE;
//   }

//   int scale1 = s21_get_scale(value_1), scale2 = s21_get_scale(value_2);
//   s21_decimal multiplier = value_2;

//   for (int i = 0; i < 96 && error == S21_OK; i++) {
//     if (s21_get_bit(value_1, i)) {
//       s21_decimal temp;
//       error = s21_add(*result, multiplier, &temp);
//       *result = temp;
//     }

//     if (i < 95) {
//       s21_shift_decimal_left(&multiplier, 1);
//     }
//   }

//   if (!error) {
//     s21_set_scale(result, scale1 + scale2);
//     s21_set_sign(result, sign_flag);
//   }

//   return error;
// }

int s21_mul(s21_decimal value1, s21_decimal value2, s21_decimal *result) {
  s21_int256 val1_256, val2_256, res_256;
  s21_to_int256(value1, &val1_256);
  s21_to_int256(value2, &val2_256);

  s21_mul_256(val1_256, val2_256, &res_256);

  return s21_from_int256(res_256, result);
}

void s21_mul_256(s21_int256 value1, s21_int256 value2, s21_int256 *result) {
  s21_zero_int256(result);

  int sign = 0;
  int sign1 = s21_get_sign_256(value1);
  int sign2 = s21_get_sign_256(value2);

  if (sign1 == sign2) {
    sign = S21_POSITIVE;
  } else {
    sign = S21_NEGATIVE;
  }

  int scale1 = s21_get_scale_256(value1);
  int scale2 = s21_get_scale_256(value2);
  s21_int256 multiplier = value2;

  for (int i = 0; i < 7 * 32; i++) {
    if (s21_get_bit_256(value1, i)) {
      s21_int256 temp;
      s21_add_256(*result, multiplier, &temp);
      *result = temp;
    }

    if (i < 7 * 32 - 1) {
      s21_shift_int256_left(&multiplier, 1);
    }
  }

  s21_set_scale_256(result, scale1 + scale2);
  s21_set_sign_256(result, sign);
}

// умножение мантисс
int s21_mul_int256_man(s21_int256 value_1, s21_int256 value_2, s21_int256 *result) {
  for (int i = 0; i < 8; i++) {
    result->bits[i] = 0;
  }

  for (int i = 0; i < 7; i++) {
    unsigned long long carry = 0;
    for (int j = 0; j < 7 - i; j++) {
      unsigned long long product =
          (unsigned long long)value_1.bits[i] * value_2.bits[j] +
          result->bits[i + j] + carry;
      result->bits[i + j] = (unsigned int)(product & 0xFFFFFFFF);
      carry = product >> 32;
    }
  }
  return S21_OK;
}

// int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
//   int error = S21_OK, sign_flag = 0;
//   error = s21_is_zero(value_2) ? S21_DIV_BY_ZERO : S21_OK;
//   s21_zero_decimal(result);

//   if (error == S21_OK) {
//     int sign1 = s21_get_sign(value_1), sign2 = s21_get_sign(value_2);
//     int scale1 = s21_get_scale(value_1), scale2 = s21_get_scale(value_2);
//     sign_flag = (sign1 == sign2) ? S21_POSITIVE : S21_NEGATIVE;
//     s21_set_sign(&value_1, S21_POSITIVE);
//     s21_set_sign(&value_2, S21_POSITIVE);
//     s21_normalize_scale(&value_1, &value_2);
//     s21_decimal dividend = value_1, divisor = value_2;
//     s21_decimal quotient;
//     s21_zero_decimal(&quotient);
//     // Вычисление целой части
//     error = s21_integer_division(&dividend, divisor, &quotient);
//     if (s21_is_zero(dividend)) {
//       *result = quotient;
//       s21_set_scale(result, scale1 - scale2);
//     } else {
//       // Вычисление дробной части
//       error = s21_fraction_division(dividend, divisor, quotient, result);
//     }
//   }
//   s21_set_sign(result, sign_flag);
//   return error;
// }

int s21_div(s21_decimal value1, s21_decimal value2, s21_decimal *result) {
  int error = s21_is_zero(value2) ? S21_DIV_BY_ZERO : S21_OK;
  s21_zero_decimal(result);

  if (error == S21_OK) {
    s21_int256 val1_256, val2_256, res_256;
    s21_to_int256(value1, &val1_256);
    s21_to_int256(value2, &val2_256);

    error = s21_div_256(val1_256, val2_256, &res_256);
    if (error == S21_OK) {
      error = s21_from_int256(res_256, result);
    }
  }

  return error;
}

int s21_div_256(s21_int256 value1, s21_int256 value2, s21_int256 *result) {
  int error = s21_is_zero_256(value2) ? S21_DIV_BY_ZERO : S21_OK;
  int sign = S21_POSITIVE;
  s21_zero_int256(result);

  if (error == S21_OK) {
    int sign1 = s21_get_sign_256(value1);
    int sign2 = s21_get_sign_256(value2);

    int scale1 = s21_get_scale_256(value1);
    int scale2 = s21_get_scale_256(value2);

    sign = (sign1 == sign2) ? S21_POSITIVE : S21_NEGATIVE;
    s21_set_sign_256(&value1, S21_POSITIVE);
    s21_set_sign_256(&value2, S21_POSITIVE);

    s21_normalize_scale_256(&value1, &value2);

    s21_int256 dividend = value1, divisor = value2;
    s21_int256 quotient;
    s21_zero_int256(&quotient);

    // Вычисление целой части
    error = s21_integer_division_256(&dividend, divisor, &quotient);

    s21_decimal tmp;
    error = s21_from_int256(quotient, &tmp);
    error = (error == S21_OVERFLOW && sign == S21_NEGATIVE) ? S21_UNDERFLOW : error;

    if (error == S21_OK) {
      if (s21_is_zero_256(dividend)) {
        *result = quotient;
        s21_set_scale_256(result, scale1 - scale2);
      } else {
        // Вычисление дробной части
        error = s21_fraction_division_256(dividend, divisor, quotient, result);
      }
    }
  }
  s21_set_sign_256(result, sign);
  return error;
}

// int s21_integer_division(s21_decimal *dividend, s21_decimal divisor,
//                          s21_decimal *quotient) {
//   int error = 0;
//   s21_zero_decimal(quotient);
//   // траблы с дивидендом
//   int i = 95, j = 95;
//   while (i >= 0 && !s21_get_bit(*dividend, i)) i--;
//   while (j >= 0 && !s21_get_bit(divisor, j)) j--;
//   int shift = (i >= j) ? i - j : 0;

//   s21_decimal temp_divisor = divisor;
//   if (shift > 0) {
//     s21_shift_decimal_left(&temp_divisor, shift);
//   }

//   s21_decimal temp_dividend = *dividend;
//   for (int k = shift; k >= 0 && !error; k--) {
//     if (s21_is_greater_or_equal(temp_dividend, temp_divisor)) {
//       error = s21_sub(temp_dividend, temp_divisor, &temp_dividend);
//       if (!error) {
//         s21_set_bit(quotient, k, 1);
//       }
//     }
//     s21_shift_decimal_right(&temp_divisor, 1);
//   }
//   *dividend = temp_dividend;

//   return error;
// }

int s21_integer_division(s21_decimal *dividend, s21_decimal divisor, s21_decimal *quotient) {
  s21_int256 val1_256, val2_256, res_256;
  s21_to_int256(*dividend, &val1_256);
  s21_to_int256(divisor, &val2_256);

  int error = s21_integer_division_256(&val1_256, val2_256, &res_256);
  if (error == S21_OK) {
    error = s21_from_int256(val1_256, dividend);
    if (error == S21_OK) {
      error = s21_from_int256(res_256, quotient);
    }
  }

  return error;
}

int s21_integer_division_256(s21_int256 *dividend, s21_int256 divisor,
  s21_int256 *quotient) {
  int error = s21_is_zero_256(divisor) ? S21_DIV_BY_ZERO : S21_OK;

  if (error == S21_OK) {
    s21_zero_int256(quotient);
    // траблы с дивидендом
    int i = 32 * 7 - 1, j = 32 * 7 - 1;
    while (i >= 0 && !s21_get_bit_256(*dividend, i)) i--;
    while (j >= 0 && !s21_get_bit_256(divisor, j)) j--;
    int shift = (i >= j) ? i - j : 0;

    s21_int256 temp_divisor = divisor;
    if (shift > 0) {
      s21_shift_int256_left(&temp_divisor, shift);
    }

    s21_int256 temp_dividend = *dividend;
    for (int k = shift; k >= 0; k--) {
      if (s21_is_greater_or_equal_256(temp_dividend, temp_divisor)) {
        s21_sub_256(temp_dividend, temp_divisor, &temp_dividend);
        // if (!error) {
          s21_set_bit_256(quotient, k, 1);
        // }
      }
      s21_shift_int256_right(&temp_divisor, 1);
    }
    *dividend = temp_dividend;
  }

  return error;
}

// int s21_fraction_division(s21_decimal dividend, s21_decimal divisor,
//                           s21_decimal quotient, s21_decimal *result) {
//   int error = S21_OK;
//   int scale1 = s21_get_scale(dividend), scale2 = s21_get_scale(divisor);
//   if (!error && !s21_is_zero(dividend)) {
//     s21_decimal remainder = dividend, fraction;
//     s21_zero_decimal(&fraction);
//     s21_decimal ten = {{10, 0, 0, 0}};
//     int frac_scale = 0;
//     while (frac_scale < 28 && !error && !s21_is_zero(remainder)) {
//       error = s21_mul(remainder, ten, &remainder);
//       if (!error) {
//         s21_decimal digit;
//         s21_zero_decimal(&digit);
//         while (s21_is_greater_or_equal(remainder, divisor) && !error) {
//           error = s21_sub(remainder, divisor, &remainder);
//           s21_decimal one = {{1, 0, 0, 0}};
//           error |= s21_add(digit, one, &digit);
//         }
//         s21_decimal temp;
//         error |= s21_mul(fraction, ten, &temp);
//         error |= s21_add(temp, digit, &fraction);
//         frac_scale++;
//       }
//     }
//     if (!error) {
//       s21_int256 big_quotient;
//       s21_zero_int256(&big_quotient);
//       s21_to_int256(quotient, &big_quotient);
//       for (int i = 0; i < frac_scale && !error; i++) {
//         s21_int256 ten = {{10, 0, 0, 0}};
//         error = s21_mul_int256_man(big_quotient, ten, &big_quotient);
//       }
//       s21_int256 big_fraction, sum;
//       s21_zero_int256(&big_fraction);
//       s21_zero_int256(&sum);
//       s21_to_int256(fraction, &big_fraction);
//       s21_add_int256_man(big_quotient, big_fraction, &sum);
//       s21_from_int256(sum, result);
//       s21_set_scale(result, frac_scale);
//     }
//   } else if (!error) {
//     *result = quotient;
//     s21_set_scale(result, scale1 - scale2);
//   }
//   return error;
// }

int s21_fraction_division(s21_decimal dividend, s21_decimal divisor,
                          s21_decimal quotient, s21_decimal *result) {
  s21_int256 val1_256, val2_256, quot_256, res_256;
  s21_to_int256(dividend, &val1_256);
  s21_to_int256(divisor, &val2_256);
  s21_to_int256(quotient, &quot_256);

  int error = s21_fraction_division_256(val1_256, val2_256, quot_256, &res_256);
  if (error == S21_OK) {
    error = s21_from_int256(res_256, result);
  }

  return error;
}

int s21_fraction_division_256(s21_int256 dividend, s21_int256 divisor, s21_int256 quotient, s21_int256 *result) {
  int error = s21_is_zero_256(divisor) ? S21_DIV_BY_ZERO : S21_OK;
  int scale1 = s21_get_scale_256(dividend);
  int scale2 = s21_get_scale_256(divisor);


  if (!error && !s21_is_zero_256(dividend)) {
    s21_int256 remainder = dividend, fraction;
    s21_zero_int256(&fraction);
    s21_int256 ten = {{10, 0, 0, 0, 0, 0, 0, 0}};
    int frac_scale = 0;

    while (frac_scale < MAX_EXPONENT + 5 && !s21_is_zero_256(remainder)) {
      s21_mul_256(remainder, ten, &remainder);
        s21_int256 digit;
        s21_zero_int256(&digit);
        while (frac_scale < MAX_EXPONENT + 5 && s21_is_greater_or_equal_256(remainder, divisor) && !error) {
          s21_sub_256(remainder, divisor, &remainder);
          s21_int256 one = {{1, 0, 0, 0, 0, 0, 0, 0}};
          s21_add_256(digit, one, &digit);
        }
        s21_int256 temp;
        s21_mul_256(fraction, ten, &temp);
        s21_add_256(temp, digit, &fraction);
        frac_scale++;
    }

    for (int i = 0; i < frac_scale; i++) {
      s21_int256 ten = {{10, 0, 0, 0, 0, 0, 0, 0}};
      error = s21_mul_int256_man(quotient, ten, &quotient);
    }

    s21_add_int256_man(quotient, fraction, result);
    s21_set_scale_256(result, frac_scale);
  } else if (!error) {
    *result = quotient;
    s21_set_scale_256(result, scale1 - scale2);
  }
  return error;
}
