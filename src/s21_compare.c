#include "s21_decimal.h"
/*Функция s21_comapre возвращает 3 значения: S21_EQ S21_BIGGER S21_LESS которые
 * уже передаются в функции сравнения с помощью тернарных операций */
int s21_compare(s21_decimal value1, s21_decimal value2) {
  s21_normalize_scale(&value1, &value2);
  int code = s21_sign_handle(value1, value2);

  if (code == S21_EQ) {
    code = s21_value_handle(value1, value2);
    if (code == S21_EQ) {
      code = s21_scale_handle(value1, value2);
    }
  }
  if (s21_get_sign(value1) == S21_NEGATIVE &&
      s21_get_sign(value2) == S21_NEGATIVE) {
    code = s21_reverse_sign(code);
  }

  return code;
}
int s21_reverse_sign(int code) {
  if (code != S21_EQ) {
    if (code == S21_BIGGER) {
      code = S21_LESS;
    } else if (code == S21_LESS) {
      code = S21_BIGGER;
    }
  }
  return code;
}
int s21_scale_handle(s21_decimal value1, s21_decimal value2) {
  int code;
  int scale1 = s21_get_scale(value1);
  int scale2 = s21_get_scale(value2);
  if (scale1 == scale2) {
    code = S21_EQ;
  } else if (scale1 < scale2) {
    code = S21_BIGGER;
  } else {
    code = S21_LESS;
  }
  return code;
}
int s21_sign_handle(s21_decimal value1, s21_decimal value2) {
  int code = S21_EQ;
  int sign1 = s21_get_sign(value1);
  int sign2 = s21_get_sign(value2);
  if (sign1 < sign2) {
    code = S21_BIGGER;
  } else if (sign1 > sign2) {
    code = S21_LESS;
  }
  return code;
}

int s21_value_handle(s21_decimal value1, s21_decimal value2) {
  int code;
  if (value1.bits[0] == value2.bits[0] && value1.bits[1] == value2.bits[1] &&
      value1.bits[2] == value2.bits[2]) {
    code = S21_EQ;
  } else if (value1.bits[2] > value2.bits[2] ||
             value1.bits[1] > value2.bits[1] ||
             value1.bits[0] > value2.bits[0]) {
    code = S21_BIGGER;
  } else if ((value1.bits[2] < value2.bits[2] ||
              value1.bits[1] < value2.bits[1] ||
              value1.bits[0] < value2.bits[0])) {
    code = S21_LESS;
  }
  return code;
}
int s21_is_less(s21_decimal value1, s21_decimal value2) {
  return (s21_compare(value1, value2) == S21_LESS) ? 1 : 0;
}
int s21_is_less_or_equal(s21_decimal value1, s21_decimal value2) {
  return (s21_compare(value1, value2) == S21_LESS) ||
                 (s21_compare(value1, value2) == S21_EQ)
             ? 1
             : 0;
}
int s21_is_greater(s21_decimal d1, s21_decimal d2) {
  return (s21_compare(d1, d2) == S21_BIGGER) ? 1 : 0;
}
int s21_is_greater_or_equal(s21_decimal d1, s21_decimal d2) {
  return (s21_compare(d1, d2) == S21_BIGGER) || (s21_compare(d1, d2) == S21_EQ)
             ? 1
             : 0;
}
int s21_is_equal(s21_decimal d1, s21_decimal d2) {
  return (s21_compare(d1, d2) == S21_EQ) ? 1 : 0;
}
int s21_is_not_equal(s21_decimal d1, s21_decimal d2) {
  return (s21_compare(d1, d2) == S21_EQ) ? 0 : 1;
}

// (3 << 2) & 1 => 00011 << 2 = 01100 => 01100 & 00001 = 00000
// (3 << 2) % 32
